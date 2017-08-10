/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "./triangulate.h"
#include "./bbox.h"
#include "../utilities/io.h"

void Triangulation::addContraintsFromLoop(Loop * loop)
{
	// register exterior loop	
	size_t nPoints = loop->size();
	for (size_t i = 0; i < nPoints; i++) {
		// add segment as a constraint
		constraints.push_back(new Segment(loop->getVertexRef(i), loop->getVertexRef((i + 1) % nPoints)));
	}
}

Triangulation::Triangulation(Polygon3D * aPolygon)
{
	// initialize without points
	polygon = aPolygon;
	points = std::vector < Point3D * >();
	constraints = std::vector < Segment * >();
	triangles = std::vector < Triangle * >();

	// Fill constraints
	Loop * outerLoop = polygon->getOuterLoopRef();
	addContraintsFromLoop(outerLoop);
	
	for (size_t i = 0; i < polygon->countInnerLoops(); i++) {
		addContraintsFromLoop(polygon->getInnerLoopRef(i));
	}

	// Calculate the maximum and minimum X,Y,Z
	setSuperRectangle();	
}

Triangulation::~Triangulation()
{
	for (size_t i = 0; i < triangles.size();i++) {
		delete triangles[i];
	}
	for (size_t i = 0; i < constraints.size(); i++) {
		delete constraints[i];
	}
}

bool Triangulation::setSuperRectangle()
{
	BBox * bbox = new BBox(polygon);
	double min_x = bbox->min_x;
	double max_x = bbox->max_x;
	double min_y = bbox->min_y;
	double max_y = bbox->max_y;
	double min_z = bbox->min_z;
	double max_z = bbox->max_z;

	Loop * outerLoop = polygon->getOuterLoopRef();

	// Calculate 4 points that represent the superrectangle 
	Vector3D * normal = polygon->getNormal();
	double nx = normal->getX();
	double ny = normal->getY();
	double nz = normal->getZ();

	Point3D * firstPoint = outerLoop->getVertexRef(0);
	double cx = firstPoint->getX();
	double cy = firstPoint->getY();
	double cz = firstPoint->getZ();

	double d = nx*cx + ny*cy + nz*cz;
	if ( std::abs(nz) > TINY) {
		DEBUG_MSG("<<<<<<<<<<<<<<< NZ");
		points.push_back( new Point3D(min_x, min_y, (d - nx*min_x - ny*min_y) / nz) );				
		points.push_back( new Point3D(min_x, max_y, (d - nx*min_x - ny*max_y) / nz) );
		points.push_back( new Point3D(max_x, max_y, (d - nx*max_x - ny*max_y) / nz) );				
		points.push_back( new Point3D(max_x, min_y, (d - nx*max_x - ny*min_y) / nz) );
	}
	else if(std::abs(ny) > TINY) {
		DEBUG_MSG("<<<<<<<<<<<<<<< NY");
		points.push_back(new Point3D(min_x, (d - nx*min_x - nz*min_z) / ny, min_z));
		points.push_back(new Point3D(min_x, (d - nx*min_x - nz*max_z) / ny, max_z));
		points.push_back(new Point3D(max_x, (d - nx*max_x - nz*max_z) / ny, max_z));
		points.push_back(new Point3D(max_x, (d - nx*max_x - nz*min_z) / ny, min_z));
	}
	else if (std::abs(nx) > TINY) {
		DEBUG_MSG("<<<<<<<<<<<<<<< NX");
		points.push_back(new Point3D((d - nx*min_y - nz*min_z) / ny, min_y, min_z));
		points.push_back(new Point3D((d - nx*min_y - nz*max_z) / ny, min_y, max_z));
		points.push_back(new Point3D((d - nx*max_y - nz*max_z) / ny, max_y, max_z));
		points.push_back(new Point3D((d - nx*max_y - nz*min_z) / ny, max_y, min_z));
	}
	else {
		fatal("Normal is (0,0,0) when adding superrectangle");
		return false;
	}

	// set the neighborhood
	Triangle * lower = new Triangle(points[0], points[1], points[2]);
	Triangle * upper = new Triangle(points[0], points[2], points[3]);
	
	// add the two triangles
	addTriangle(lower);
	addTriangle(upper);

	lower->setNeighbor(upper, 2, true);
	upper->setNeighbor(lower, 0, true);

	return true;
}

bool Triangulation::doCDT()
{
	if (!doDT()) {
		fatal("Impossible to create constrained delaunay triangularion");
		return false;
	}
	warn("Starting constraints application");

	for (size_t i = 0; i < constraints.size(); i++) {
		warn("Applying constraint "+std::to_string(i));
		applyConstrain(constraints[i]);
	}
	return true;
}

bool Triangulation::doDT()
{
	DEBUG_MSG("Starting Delaunay Triangulation");
	// add outer loop
	Loop * outerLoop = polygon->getOuterLoopRef();
	for (size_t i = 0; i < outerLoop->size(); i++) {
		if (!addPoint(outerLoop->getVertexRef(i)))
			return false;
	}

	// add inner loops
	for (size_t i = 0; i < polygon->countInnerLoops(); i++) {
		Loop * loop = polygon->getInnerLoopRef(i);
		for (size_t j = 0; j < loop->size(); j++) {
			if (!addPoint(loop->getVertexRef(j)))
				return false;
		}
	}


	restoreDelaunay();

	DEBUG_MSG("ENDED Delaunay Triangulation");
	return true;
}

bool Triangulation::pointInTriangle(Triangle * t, Point3D * p, int * code)
{
	/*
		Source: http://blackpawn.com/texts/pointinpoly/
	*/
	// get vertices
	Point3D * a = t->getVertex(0);
	Point3D * b = t->getVertex(1);
	Point3D * c = t->getVertex(2);

	// Compute vectors        
	Vector3D v0 = *c - a;
	Vector3D v1 = *b - a;
	Vector3D v2 = *p - a;

		// Compute dot products
	double dot00 = v0 * &v0;
	double dot01 = v0 * &v1;
	double dot02 = v0 * &v2;
	double dot11 = v1 * &v1;
	double dot12 = v1 * &v2;

	// Compute barycentric coordinates
	double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	double v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	double w = 1 - u - v;

	// Check if point is in triangle
	if ( u >= MINUS_TINY && v >= MINUS_TINY && w >= MINUS_TINY ) {
		// Somewhere in the triangle
		if (u <= TINY && v <= TINY){
			*code = 0; // vertex a
		}
		else if (u <= TINY && w <= TINY){
			*code = 1; // vertex b
		}
		else if (v <= TINY && w <= TINY){ 
			*code = 2; // vertex c
		} 
		else if (u <= TINY){
			*code = 3; // edge AB
		}
		else if (w <= TINY){
			*code = 4; // edge BC
		}
		else if (v <= TINY){
			*code = 5; // edge AC
		}
		else {
			*code = 6; // inside the triangle
		}
		return true;
	} 
	*code = -1;
	return false;
}

bool Triangulation::addPointToTriangle(size_t index, Point3D * point, int code)
{
	if (triangles[index] == NULL) {
		fatal("Trying to add point into an obsolete triangle");
		return false;
	}

	if (code < 3) { // Point is a vertex 
					// do nothing... the point has already been added.
		return true;
	}
	else if (code < 6) { // Point in an edge				
		return splitEdge(index, point, code);

	}
	else { // Point inside the triangle (code == 6)
		return splitTriangle(index, point);
	}


}

bool Triangulation::addPoint(Point3D * point)
{

	DEBUG_MSG("Adding point ("+std::to_string(point->getX()) +"," + std::to_string(point->getY()) + "," + std::to_string(point->getZ()) + ")");
	// Iterate through triangles to check
	int code;
	for (size_t i = 0; i < triangles.size(); i++) {
		// skip triangle if it has been deleted
		if (triangles[i] == NULL)
			continue;
		
		if (pointInTriangle(triangles[i], point, &code)) {			
			if(!addPointToTriangle(i, point,code))
				return false;
		}
	}
	
}

void Triangulation::flipDiagonal(size_t index, int i, bool constraint)
{
	if (i < 0 || i > 2) {
		fatal("Impossible index when flipping digonal... index was '" + std::to_string(i) + "'");		
	}
	warn("Flipping diagonal of triangle " + std::to_string(index) + " with neighbor " + std::to_string(i));
	// get neighbor
	Triangle * neighbor = triangles[index]->getNeighbor(i);

	if (neighbor == NULL) {
		fatal("Trying to flip a diagonal with a NULL neighbor");
	}

	// get vertices
	Point3D * a = triangles[index]->getVertex(i % 3);
	Point3D * b = triangles[index]->getVertex((i + 1) % 3);
	Point3D * c = triangles[index]->getVertex((i + 2) % 3);

	// Get the oposite side
	Point3D * oposite = getOpositeVertex(triangles[index], i);

	// create new triangles
	Triangle * flipped1 = new Triangle(a, oposite, c);
	Triangle * flipped2 = new Triangle(c, oposite, b);

	// add triangles
	addTriangle(flipped1);
	addTriangle(flipped2);

	//set constraints if needed
	if (constraint) {
		flipped1->setConstraint(1);
		flipped2->setConstraint(0);
	}

	// Set neighbors
	warn("A ======================");
	int aux1 = neighbor->getEdgeIndexByPoints(oposite, a);
	Triangle * aux2 = neighbor->getNeighbor(aux1);
	if (aux2 != NULL) {
		size_t aux3 = aux2->getIndex();
		flipped1->setNeighbor(triangles[aux3], 0, true);
	}
	warn("B ======================");
	flipped1->setNeighbor(flipped2, 1,true);
	flipped1->setNeighbor(triangles[index]->getNeighbor((i+2)%3),2,true);


	// flipped2->setNeighbor(flipped2, 0,false); // THIS IS DONE BY RECIPROCITY... PUT HERE TO MAKE IT EXPLICIT

	warn("A======================");
	int aux12 = neighbor->getEdgeIndexByPoints(oposite, b);
	Triangle * aux22 = neighbor->getNeighbor(aux12);
	if (aux22 != NULL) {
		size_t aux3 = aux22->getIndex();
		flipped2->setNeighbor(triangles[aux3], 1, true);
	}
	warn("B======================");
	flipped2->setNeighbor(triangles[index]->getNeighbor((i + 1) % 3), 2, true);


	// delete obsolete triangles
	deleteTriangle(index);
	deleteTriangle(neighbor->getIndex());
	

}


bool Triangulation::isConvex(Point3D * a, Point3D * b, Point3D * c, Point3D * d)
{

	Vector3D s1 = *b - a;
	Vector3D s2 = *c - b;
	Vector3D n12 = s1%s2;
	
	if (n12.isZero())  // points are collinear
		return false;

	Vector3D s3 = *d - c;
	Vector3D n23 = s2%s3;

	if (n23.isZero()) // points are collinear
		return false;
	

	if (!n12.sameDirection(&n23)) 
		return false;
	
	Vector3D s4 = *a - d;
	Vector3D n34 = s3%s4;
	
	if (n34.isZero()) // points are collinear
		return false;
		
	if (!n12.sameDirection(&n34)) 
		return false;
	

	Vector3D n41= s4%s1;
	if (n41.isZero()) // points are collinear		
		return false;
	
	if (!n12.sameDirection(&n41)) 		
		return false;
	
	return true;
}

double Triangulation::getBestAspectRatio(Triangle * t, int i, bool * changed)
{
	if (i < 0 || i > 2) {
		fatal("Impossible index when getting best aspect ratio... index was '" + std::to_string(i) + "'");
		return -1;
	}

	// get neighbor
	Triangle * neighbor = t->getNeighbor(i);
	if (neighbor == NULL)
		return -1;
	
	warn("11");
	// get vertices
	Point3D * a = t->getVertex(i % 3);
	Point3D * b = t->getVertex((i + 1) % 3);
	Point3D * c = t->getVertex((i + 2) % 3);

	warn("12");
	warn("13");
	// Get the oposite side
	Point3D * oposite = getOpositeVertex(t, i);

	if (!isConvex(a, oposite, b, c)) {
		DEBUG_MSG("Not flipping because is not convex");
		return -1;
	}

	std::cerr << ">>>>>>>>>>>>>>>>>>>>>>> Point a: " << a->getX() << "  " << a->getY() << "  " << a->getZ() << std::endl;
	std::cerr << ">>>>>>>>>>>>>>>>>>>>>>> Point b: " << b->getX() << "  " << b->getY() << "  " << b->getZ() << std::endl;
	std::cerr << ">>>>>>>>>>>>>>>>>>>>>>> Point c: " << c->getX() << "  " << c->getY() << "  " << c->getZ() << std::endl;
	std::cerr << ">>>>>>>>>>>>>>>>>>>>>>> Point o: " << oposite->getX() << "  " << oposite->getY() << "  " << oposite->getZ() << std::endl;

	warn("14");
	// get current situation
	*changed = false;

	warn("15");
	double tAspect = t->getAspectRatio();

	warn("16");
	double nAspect = neighbor->getAspectRatio();

	warn("17");
	double currentMin = (tAspect > nAspect) ? tAspect : nAspect; // the max of the two

	warn("18");

	// get the other situation
	Triangle * flipped1 = new Triangle(a,oposite,c);
	warn("19");
	Triangle * flipped2 = new Triangle(oposite, b, c);
	warn("110");
	double f1Aspect = flipped1->getAspectRatio();

	warn("111");
	double f2Aspect = flipped2->getAspectRatio();
	warn("112");
	double flippedMin = (f1Aspect > f2Aspect) ? f1Aspect : f2Aspect;
	warn("114");

	delete flipped1;
	delete flipped2;

	if (flippedMin < currentMin) {
		*changed = true;
		return flippedMin;
	}

	return currentMin;
}

void Triangulation::restoreDelaunay()
{	
	DEBUG_MSG("Restoring Delaunay triangulation");
	
	for (size_t i = 0; i < nTriangles; i++) {		
		DEBUG_MSG(" >>>>>>>>>>>>> Checking triangle "+std::to_string(i) + " of "+ std::to_string(nTriangles));

		if (triangles[i] == NULL)
			continue;

		warn("Triangle is not null");
		double currentAspectRatio = triangles[i]->getAspectRatio();

		warn("Calculated Aspect Ratio");
		if (currentAspectRatio < MAX_ASPECT_RATIO)
			continue;
		
		DEBUG_MSG(" >>>>>>>>>>>>> Triangle is ugly " + std::to_string(i));
		int bestNeighbor = -1;
		int bestAspectRatio = HUGE;
		bool changed = false;
		// Found an ugly triangle
		for (int j = 0; j < 3; j++) { // Check three edges for flipping.		
			// calculate possible aspect ratio
			bool c;
			warn("checking for neighbor " + std::to_string(j));
			double ar = getBestAspectRatio(triangles[i], j, &c);
			warn("Aspect ratio is "+std::to_string(ar));
			if (ar == -1) // Null neighbor
				continue;
			
			warn("..... Moving on...");
			if (ar < currentAspectRatio &&  ar < bestAspectRatio) {
				bestNeighbor = j;
				bestAspectRatio = ar;
				changed = c;
			}
			warn("Current aspect ratio is " + std::to_string(currentAspectRatio) + " | Best aspect ratio is " + std::to_string(ar));

		}
		warn("Best neighbor is " + std::to_string(bestNeighbor));
		// Flip is needed.
		if (bestNeighbor != -1) { // if at least one of them is better than the current
			flipDiagonal(i, bestNeighbor,false);
		} // else... do nothing

	}
}

size_t Triangulation::getNumTriangles() 
{
	return nTriangles;
}

Triangle * Triangulation::getTriangleRef(size_t i)
{
	return triangles[i];
}


bool Triangulation::splitTriangle(size_t i, Point3D * p)
{	
	DEBUG_MSG("Splitting triangle");
	// get vertices
	Point3D * a = triangles[i]->getVertex(0);
	Point3D * b = triangles[i]->getVertex(1);
	Point3D * c = triangles[i]->getVertex(2);

	// add the new ones.
	Triangle * ab = new Triangle(a, b, p);
	Triangle * ac = new Triangle(a, p, c);
	Triangle * bc = new Triangle(b, c, p);

	// set neighbors of AB
	ab->setNeighbor(triangles[i]->getNeighbor(0), 0, true);
	ab->setNeighbor(bc, 1, true);
	ab->setNeighbor(ac, 2, true);

	// set neighbors of AC
	ac->setNeighbor(ab, 0, true);
	ac->setNeighbor(bc, 1, true);
	ac->setNeighbor(triangles[i]->getNeighbor(2), 2, true);

	// set neighbors of BC
	bc->setNeighbor(triangles[i]->getNeighbor(1), 0, true);
	bc->setNeighbor(ac, 1, true);
	bc->setNeighbor(ab, 2, true);

	// add the three new triangles
	addTriangle(ab);
	addTriangle(ac);
	addTriangle(bc);
	
	// delete the old one
	deleteTriangle(i);

	return true;
}


bool Triangulation::splitEdge(size_t i, Point3D * p, int code)
{

	DEBUG_MSG("Splitting edge "+std::to_string(code%3) + " of triangle "+std::to_string(i));
	// get vertices
	Point3D * a = triangles[i]->getVertex(code%3);
	Point3D * b = triangles[i]->getVertex((code+1) % 3);
	Point3D * c = triangles[i]->getVertex((code+2) % 3);
	
	// Get the oposite side
	Point3D * oposite = getOpositeVertex(triangles[i], code % 3);
	
	// retrieve neighbor (will be deleted)
	Triangle * neighbor = triangles[i]->getNeighbor(code % 3);
	
	// Create and add the new triangles.
	Triangle * ac = new Triangle(a, p, c);
	addTriangle(ac);
	Triangle * bc = new Triangle(p, b, c);
	addTriangle(bc);
	Triangle * ap = NULL;
	Triangle * bp = NULL;

	if (oposite != NULL) {		
		ap = new Triangle(a, oposite, p);
		addTriangle(ap);
		bp = new Triangle(oposite, b, p);
		addTriangle(bp);
	}
	
	ac->setNeighbor(ap, 0, true);
	ac->setNeighbor(bc, 1, true);
	ac->setNeighbor(triangles[i]->getNeighbor((code - 1) % 3), 2, true);

	
	bc->setNeighbor(bp, 0, true);
	bc->setNeighbor(triangles[i]->getNeighbor((code - 2)%3), 1, true);	

	// THIS IS DONE BY RECIPROCITY... I LEAVE IT 
	// HERE TO MAKE IT EXPLICIT
	//bc->setNeighbor(ac, 2, true);
	
	if (bp != NULL) {

		int aux1 = neighbor->getEdgeIndexByPoints(oposite, a);
		Triangle * aux2 = neighbor->getNeighbor(aux1);	
		if (aux2 != NULL) {
			size_t aux3 = aux2->getIndex();
			ap->setNeighbor(triangles[aux3], 0, true);
		}
		else {
			ap->setNeighbor(NULL, 0, true);
		}
		ap->setNeighbor(bp, 1, true);
		
		// THIS IS DONE BY RECIPROCITY... I LEAVE IT 
		// HERE TO MAKE IT EXPLICIT
		//ap->setNeighbor(ac, 2, true); 

		aux1= neighbor->getEdgeIndexByPoints(oposite, b);
		aux2 = neighbor->getNeighbor(aux1);
		if (aux2 != NULL) {
			size_t aux3 = aux2->getIndex();
			bp->setNeighbor(triangles[aux3], 0, true);
		}
		else {
			bp->setNeighbor(NULL, 0, true);
		}

		// THIS IS DONE BY RECIPROCITY... I LEAVE IT 
		// HERE TO MAKE IT EXPLICIT
		//bp->setNeighbor(bc, 1, true);

		// THIS IS DONE BY RECIPROCITY... I LEAVE IT 
		// HERE TO MAKE IT EXPLICIT
		//bp->setNeighbor(ap, 2, true);		
	}	

	deleteTriangle(i);

	if (neighbor != NULL) {
		deleteTriangle(neighbor->getIndex());
	}

	return true;
}

Point3D * Triangulation::getOpositeVertex(Triangle * t, int i) {
	
	Triangle * neighbor = t->getNeighbor(i);
	if (neighbor == NULL)
		return NULL;

	// find the vertex that is in neighbor but not in triangle.
	for (int i = 0; i < 3; i++) { // count in neighbor		
		bool found = false;
		for (int j = 0; j < 3; j++) { // count in this
			if (neighbor->getVertex(i) == t->getVertex(j)) {
				found = true;
				break;
			}
		}
		if (found)
			continue;

		return neighbor->getVertex(i);
	}
	return NULL;
}


size_t Triangulation::addTriangle(Triangle * t) {		
	t->setIndex(nTriangles);
	triangles.push_back(t);

#ifdef DEBUG
	double x1 = t->getVertex(0)->getX();
	double y1 = t->getVertex(0)->getY();
	double z1 = t->getVertex(0)->getZ();

	double x2 = t->getVertex(1)->getX();
	double y2 = t->getVertex(1)->getY();
	double z2 = t->getVertex(1)->getZ();

	double x3 = t->getVertex(2)->getX();
	double y3 = t->getVertex(2)->getY();
	double z3 = t->getVertex(2)->getZ();

	DEBUG_MSG("adding a triangle " + std::to_string(nTriangles) + " --- " + 
		"(" + std::to_string(x1) + "," + std::to_string(y1) + "," + std::to_string(z1) + ") | " + 
		"(" + std::to_string(x2) + "," + std::to_string(y2) + "," + std::to_string(z2) + ") | " + 
		"(" + std::to_string(x3) + "," + std::to_string(y3) + "," + std::to_string(z3) + ") | "
	);
#endif
	nTriangles += 1;
	return nTriangles;
}


void Triangulation::deleteTriangle(size_t i) {

#ifdef DEBUG
	DEBUG_MSG("Deleting triangle "+std::to_string(i));
#endif	
	delete triangles[i];
	triangles[i] = NULL;
}

void Triangulation::applyConstrain(Segment * s)
{
	int code;	
	for (size_t i = 0; i < nTriangles; i++) {
		warn("Testing triangle " + std::to_string(i));
		if (triangles[i] == NULL)
			continue;

		if (pointInTriangle(triangles[i], s->start, &code)) {
			warn("... Point is in Triangle tested");

			// If START is in a triangle,
			if (code < 0 || code > 2) {
				fatal("Trying to set a constraint that leads to an edge");
				return;
			}
			// Check where is END
			pointInTriangle(triangles[i], s->end, &code);

			
			if(code < 0){
				// END is outside the triangle, intersect and flip
				Point3D intersection(0, 0, 0);
				for (int j = 0; j < 3; j++) {
					warn("Intersecting witb " + std::to_string(j));

					if (triangles[i] == NULL)
						warn("NULL TRIANGLE");

					Segment * seg = triangles[i]->getSegment(j);					

					warn("segment retrieved");
					bool inter = s->intersect(seg, &intersection);
					warn("Inter done");
					if (inter) {
						warn("Id did intersect!");
						// The neighbor is J
						bool changed = false;
						if (getBestAspectRatio(triangles[i], j, &changed) > 0) {
							warn("Flipping!");
							// we have to flip... unless is not convex or does not make sense
							flipDiagonal(i, j, true);
						}
						return;
					}
				}
				warn("Did not instersect");
				// did not intersect? Wait for another one.
				return;
			}
			else if (code < 3) {
				// if it is on a vertex, then this edge is already a constrain
				triangles[i]->setConstraint(code);
				return;
			}
			else if (code < 6) {
				// This should not happen
				fatal("Trying to set a constraint that leads to an edge");
				return;
			}
			else if (code == 6) {
				// Point is inside the triangle, split it.
				fatal("Trying to set a constraint that leads to the middle of a triangle");
				return;
			}
			else {
				fatal("Impossible code given when applying constraint");
				return;
			}
		}
		warn("Point is not in Triangle");
	}
}