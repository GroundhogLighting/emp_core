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




#include "./triangulation.h"
#include "./bbox.h"
#include "../utilities/io.h"

#define MPE_POLY2TRI_IMPLEMENTATION
#include "../../3rdparty/fast-poly2tri/MPE_fastpoly2tri.h"


Triangulation::Triangulation(Polygon3D * aPolygon)
{
	// initialize without points
	polygon = aPolygon;
	triangles = std::vector < Triangle * >();	
}

Triangulation::~Triangulation()
{
	for (size_t i = 0; i < triangles.size();i++) {
		if (triangles[i] == NULL)
			continue;

		delete triangles[i];
	}
}

/*
bool Triangulation::setSuperRectangle()
{
	BBox * bbox = new BBox(polygon);
	double min_x = bbox->min_x;
	double max_x = bbox->max_x;
	double min_y = bbox->min_y;
	double max_y = bbox->max_y;
	double min_z = bbox->min_z;
	double max_z = bbox->max_z;

	if (min_x == max_x && min_y == max_y && min_z == max_z ) {
		fatal("Trying to set superRectangle of singular polygon", __LINE__, __FILE__);
		return false;
	}

	Loop * outerLoop = polygon->getOuterLoopRef();

	// Calculate 4 points that represent the superrectangle 
	Vector3D normal = polygon->getNormal();
	double nx = normal.getX();
	double ny = normal.getY();
	double nz = normal.getZ();

	Point3D * firstPoint = outerLoop->getVertexRef(0);
	double cx = firstPoint->getX();
	double cy = firstPoint->getY();
	double cz = firstPoint->getZ();

	double d = nx*cx + ny*cy + nz*cz;
	if ( std::abs(nz) > TINY) {
		points.push_back( new Point3D(min_x, min_y, (d - nx*min_x - ny*min_y) / nz) );				
		points.push_back( new Point3D(min_x, max_y, (d - nx*min_x - ny*max_y) / nz) );
		points.push_back( new Point3D(max_x, max_y, (d - nx*max_x - ny*max_y) / nz) );				
		points.push_back( new Point3D(max_x, min_y, (d - nx*max_x - ny*min_y) / nz) );
	}
	else if(std::abs(ny) > TINY) {
		points.push_back(new Point3D(min_x, (d - nx*min_x - nz*min_z) / ny, min_z));
		points.push_back(new Point3D(min_x, (d - nx*min_x - nz*max_z) / ny, max_z));
		points.push_back(new Point3D(max_x, (d - nx*max_x - nz*max_z) / ny, max_z));
		points.push_back(new Point3D(max_x, (d - nx*max_x - nz*min_z) / ny, min_z));
	}
	else if (std::abs(nx) > TINY) {
		points.push_back(new Point3D((d - nx*min_y - nz*min_z) / ny, min_y, min_z));
		points.push_back(new Point3D((d - nx*min_y - nz*max_z) / ny, min_y, max_z));
		points.push_back(new Point3D((d - nx*max_y - nz*max_z) / ny, max_y, max_z));
		points.push_back(new Point3D((d - nx*max_y - nz*min_z) / ny, max_y, min_z));
	}
	else {
		fatal("Normal is (0,0,0) when adding superrectangle", __LINE__, __FILE__);
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
*/
/*
bool Triangulation::doCDT()
{
	DEBUG_MSG("Starting Constrained Delaunay Triangulation");
	
	// add outer loop
	Loop * outerLoop = polygon->getOuterLoopRef();
	for (size_t i = 0; i < constraints.size(); i++) {
		warn(".................Applying constraint " + std::to_string(i));
		applyConstraint(constraints[i]);
	}

	restoreDelaunay();

	DEBUG_MSG("ENDED Constrained Delaunay Triangulation");
		
	// delete triangles that are not part of the polygon	
	//clean();

	return true;
}
*/

/*
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
*/






bool Triangulation::addPointToTriangle(size_t index, Point3D * point, int code)
{
	warn("........... Adding point");
	point->print();

	if (triangles[index] == NULL) {
		fatal("Trying to add point into an obsolete triangle", __LINE__, __FILE__);
		return false;
	}

	if (code < 3) { // Point is a vertex 
					// do nothing... the point has already been added.
		return true;
	}
	else if (code < 6) { // Point in an edge			
		splitEdge(index, point, code,false);
		return true;
	}
	else { // Point inside the triangle (code == 6)
		return splitTriangle(index, point);
	}
}

bool Triangulation::addPoint(Point3D * point)
{
	warn("..... adding point "); point->print();
	// Iterate through triangles to check
	int code;
	for (size_t i = 0; i < triangles.size(); i++) {
		// skip triangle if it has been deleted
		if (triangles[i] == NULL)
			continue;
		
		if (triangles[i]->testPoint( point, &code))
			addPointToTriangle(i, point, code);
					
	}
	fatal("Point is not in any triangle",__LINE__,__FILE__);
	return true; // triangle not found
}

void Triangulation::flipDiagonal(size_t index, int edge, bool constraint)
{
	if (edge < 0 || edge > 2) {
		fatal("Impossible index when flipping digonal... index was '" + std::to_string(edge) + "'", __LINE__, __FILE__);
	}
	
	// get neighbor
	DEBUG_MSG("..... will look for neighbor in flipDiagonal");
	Triangle * neighbor = triangles[index]->getNeighbor(edge);

	if (neighbor == NULL) {
		fatal("Trying to flip a diagonal with a NULL neighbor", __LINE__, __FILE__);
		return;
	}

	// get vertices
	Point3D * a = triangles[index]->getVertex(edge % 3);
	Point3D * b = triangles[index]->getVertex((edge + 1) % 3);
	Point3D * c = triangles[index]->getVertex((edge + 2) % 3);

	// Get the oposite side
	Point3D * oposite = getOpositeVertex(triangles[index], edge);

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
	DEBUG_MSG(".... line 313");
	std::cout << "TRIANGLE " << std::endl;
	triangles[index]->print();
	std::cout << "Neighbor " << std::endl;
	neighbor->print();
	std::cout << "Point OPOSITE = ";
	oposite->print();
	warn("ABCACBACBCABC");
	int aux1 = neighbor->getEdgeIndexByPoints(oposite, a);
	warn("ABCACBACBCABC ----- 2");
	if (aux1 < 0) {
		fatal("Inconsistent neighborhood when flipping edge", __LINE__, __FILE__);
		return;
	}
	warn("ABCACBACBCABC------------- 3");
	Triangle * aux2 = neighbor->getNeighbor(aux1);
	warn("ABCACBACBCABC---------- 4 ");
	if (aux2 != NULL) {
		size_t aux3 = aux2->getIndex();
		flipped1->setNeighbor(triangles[aux3], 0, true);
	}

	warn("ABCACBACBCABC --------- 5");
	flipped1->setNeighbor(flipped2, 1,true);

	warn("ABCACBACBCABC ---------- 6");
	flipped1->setNeighbor(triangles[index]->getNeighbor((edge+2)%3),2,true);

	warn("ABCACBACBCABC ------------ 7");
	// flipped2->setNeighbor(flipped2, 0,false); // THIS IS DONE BY RECIPROCITY... PUT HERE TO MAKE IT EXPLICIT

	
	DEBUG_MSG(".... line 327");
	int aux12 = neighbor->getEdgeIndexByPoints(oposite, b);
	warn("........ Flipping 1.1");
	if (aux12 < 0) {
		fatal("Inconsistent neighborhood when flipping edge -- Line", __LINE__, __FILE__);
		return;
	}
	warn("........ Flipping 1.2");

	Triangle * aux22 = neighbor->getNeighbor(aux12);

	warn("........ Flipping 1.3");
	if (aux22 != NULL) {
		size_t aux3 = aux22->getIndex();
		warn(" ................... AUX3 = " + std::to_string(aux3));
		flipped2->setNeighbor(triangles[aux3], 1, true);
	}

	warn("........ Flipping 1.4");
	
	flipped2->setNeighbor(triangles[index]->getNeighbor((edge + 1) % 3), 2, true);

	warn("........ Flipping 1.5");

	// delete obsolete triangles
	deleteTriangle(index);

	warn("........ Flipping 1.6");
	deleteTriangle(neighbor->getIndex());

	warn("........ Flipping 1.7");
	

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
	

	if (!n12.sameDirection(n23)) 
		return false;

	Vector3D s4 = *a - d;
	Vector3D n34 = s3%s4;

	if (n34.isZero()) // points are collinear
		return false;

	if (!n12.sameDirection(n34)) 
		return false;
	

	Vector3D n41= s4%s1;
	if (n41.isZero()) // points are collinear		
		return false;

	if (!n12.sameDirection(n41)) 		
		return false;

	return true;
}

double Triangulation::getBestAspectRatio(Triangle * t, int i, bool * changed)
{
	if (i < 0 || i > 2) {
		fatal("Impossible index when getting best aspect ratio... index was '" + std::to_string(i) + "'", __LINE__, __FILE__);
		return -1;
	}

	// do not modify constraints
	if (t->isContraint(i))
		return -1;

	// get neighbor
	Triangle * neighbor = t->getNeighbor(i);
	if (neighbor == NULL)
		return -1;
	
	// get vertices
	Point3D * a = t->getVertex(i % 3);
	Point3D * b = t->getVertex((i + 1) % 3);
	Point3D * c = t->getVertex((i + 2) % 3);
	
	// Get the oposite side
	Point3D * oposite = getOpositeVertex(t, i);

	if (!isConvex(a, oposite, b, c)) {
		return -1;
	}

	// get current situation
	*changed = false;

	double tAspect = t->getAspectRatio();
	double nAspect = neighbor->getAspectRatio();
	double currentMin = (tAspect > nAspect) ? tAspect : nAspect; // the max of the two

	// get the other situation
	Triangle flipped1 = Triangle(a,oposite,c);	
	Triangle flipped2 = Triangle(oposite, b, c);	
	double f1Aspect = flipped1.getAspectRatio();
	double f2Aspect = flipped2.getAspectRatio();
	double flippedMin = (f1Aspect > f2Aspect) ? f1Aspect : f2Aspect;
	
	if (flippedMin < currentMin) {
		*changed = true;
		return flippedMin;
	}

	return currentMin;
}

/*
bool Triangulation::mergeIfPossible(Triangle * tri, int neighbor)
{
	if (neighbor < 0 || neighbor > 2) {
		fatal("Impossible index trying to merge triangles... index was '" + std::to_string(neighbor) + "'", __LINE__, __FILE__);
		return false;
	}
	Triangle * nei = tri->getNeighbor(neighbor);
	warn("merge 1");
	if (nei == NULL)
		return false;

	Segment * s = tri->getSegment(neighbor);
	warn("merge 2");
	Point3D * a = s->start;
	Point3D * b = s->end;
	warn("merge 3");
	Point3D * o = getOpositeVertex(tri, neighbor);
	warn(" === Point o:");
	o->print();
	warn("merge 4");
	int abIndex = nei->getEdgeIndexByPoints(a, b);
	if (abIndex == -1) {
		fatal(" <<<<<<<<<<<<<<<<<<<< abIndex == -1",__LINE__,__FILE__);
		warn("==== A:"); a->print();
		warn("==== B:"); b->print();
		nei->print();
		warn("==============================");
		return false;
	}
	warn("merge 5");
	Point3D * c = getOpositeVertex(nei, abIndex);
	warn("==== Point C:"); c->print();

	warn("merge 6");

	// A, B and Oposite are collinear
	warn("==== Point B:"); b->print();
	warn("==== Point A:"); a->print();
	Vector3D ca = *a - *c;
	warn("merge 6.1");
	
	Vector3D ao = *o - *a;
	warn("merge 6.2");
	Point3D * m = b;
	Point3D * notM = a;
	// Try to merge via CAO
	if (!(ca%ao).isZero()) { 
		// CAO not colinear... try to merge via CBO
		Vector3D cb = *b - *c;
		Vector3D bo = *o - *b;

		if (!(cb%bo).isZero()) {
			// CBO non colinear... Impossible to merge
			return false;
		}
		m = a;
		notM = b;
	}
		

	warn("merge 7");
	// check if there is something 'below' 	
	int cNotMIndex = tri->getEdgeIndexByPoints(c,notM);
	if (cNotMIndex == -1) {
		c->print();
		notM->print();
		tri->print();
	}
	warn("merge 7.0.1");
	Triangle * triLowerNeighbor = tri->getNeighbor(cNotMIndex);
	warn("merge 7.0.2");
	int notMOIndex = nei->getEdgeIndexByPoints(o, notM);
	warn("merge 7.0.3");
	Triangle * neiLowerNeighbor = nei->getNeighbor(notMOIndex);
	warn("merge 7.0.4");
	Point3D * opp = NULL;

	warn("merge 7.1");
	if (triLowerNeighbor != NULL) {
		if (neiLowerNeighbor == NULL) {
			return false; // cannot merge this situation
		}
		opp = getOpositeVertex(tri, cNotMIndex);
		if (neiLowerNeighbor->getEdgeIndexByPoints(opp,notM) < 0) {
			return false; // Cannot merge this situation either
		}
	}
	else {
		if (neiLowerNeighbor != NULL)
			return false;
	}

	warn("merge 8");
	// Create new triangle
	Triangle * merge = new Triangle(c, o, m);
	addTriangle(merge);

	Triangle * merge2 = NULL;
	if (opp != NULL) {
		merge2 = new Triangle(c, opp, o);
		addTriangle(merge2);
	}	

	// add it
	warn("merge 8.1");


	warn("merge 9");
	warn("merge 10");

	// neighbors and constraints
	if (merge2 != NULL) {
		merge->setNeighbor(merge2, 0, true); 

		warn("merge 10.1");
		int coppIndex = triLowerNeighbor->getEdgeIndexByPoints(c, opp);
		if (coppIndex == -1) {
			warn("COPP INDEX == -1");
			c->print(); opp->print(); triLowerNeighbor->print();
		}
		warn("merge 10.2");
		merge2->setNeighbor(triLowerNeighbor->getNeighbor(coppIndex), 0,true);

		warn("merge 10.3");
		int ooppIndex = neiLowerNeighbor->getEdgeIndexByPoints(o, opp);

		warn("merge 10.4");
		merge2->setNeighbor(neiLowerNeighbor->getNeighbor(ooppIndex), 1, true);

		warn("merge 10.5");
		// constraints
		if (triLowerNeighbor->isContraint(coppIndex))
			merge2->setConstraint(0);

		if (neiLowerNeighbor->isContraint(ooppIndex))
			merge2->setConstraint(1);

	}
	warn("merge 11");

	int omIndex = nei->getEdgeIndexByPoints(m,o);
	warn("merge 12");
	
	warn("merge 13");
	o->print(); c->print();
	merge->setNeighbor(nei->getNeighbor(omIndex), 1, true);
	warn("merge 14");


	int mcIndex = tri->getEdgeIndexByPoints(m, c);
	merge->setNeighbor(tri->getNeighbor(mcIndex), 2, true);

	warn("merge 15");

	
	//int oNotMIndex = nei->getEdgeIndexByPoints(notM, o);
	//if (tri->isContraint(cNotMIndex) && nei->isContraint(oNotMIndex)) {
	//	merge->setConstraint(0);
	//}
	//warn("merge 16");
	

	if (nei->isContraint(omIndex))
		merge->setConstraint(1);

	if (tri->isContraint(mcIndex))
		merge->setConstraint(2);



	warn("merge 17");

	warn("merge 18");

	// delete old triangles	
	deleteTriangle(nei->getIndex());
	deleteTriangle(tri->getIndex());

	return true;
}
*/

void Triangulation::restoreDelaunay()
{	
	DEBUG_MSG("Restoring Delaunay triangulation");
	
	for (size_t i = 0; i < nTriangles; i++) {		
	
		if (triangles[i] == NULL)
			continue;

		double currentAspectRatio = triangles[i]->getAspectRatio();

		if (currentAspectRatio < MAX_ASPECT_RATIO)
			continue;
		
		// Check if it is worth flipping
		int bestNeighbor = -1;
		double bestAspectRatio = HUGE;
		// Found an ugly triangle
		for (int j = 0; j < 3; j++) { // Check three edges for flipping or merging						
			// calculate possible aspect ratio
			bool c;
			double ar = getBestAspectRatio(triangles[i], j, &c);
			if (ar == -1) // Null neighbor or constraint
				continue;
			
			if (ar < currentAspectRatio &&  ar < bestAspectRatio) {
				bestNeighbor = j;
				bestAspectRatio = ar;
			}		
		}

		// Flip if needed.
		if (bestNeighbor >= 0 ) { 
			// if at least one of them is better than the current
			flipDiagonal(i, bestNeighbor, false);
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

	warn(".............ST1");
	// get vertices
	Point3D * a = triangles[i]->getVertex(0);
	Point3D * b = triangles[i]->getVertex(1);
	Point3D * c = triangles[i]->getVertex(2);

	warn(".............ST2");
	// add the new ones.
	Triangle * ab = new Triangle(a, b, p);
	Triangle * ac = new Triangle(a, p, c);
	Triangle * bc = new Triangle(b, c, p);

	warn(".............ST3");
	// set neighbors of AB
	ab->setNeighbor(triangles[i]->getNeighbor(0), 0, true);
	ab->setNeighbor(bc, 1, true);
	ab->setNeighbor(ac, 2, true);

	warn(".............ST4");
	// set neighbors of AC
	ac->setNeighbor(ab, 0, true);
	ac->setNeighbor(bc, 1, true);
	ac->setNeighbor(triangles[i]->getNeighbor(2), 2, true);

	warn(".............ST5");
	// set neighbors of BC
	bc->setNeighbor(triangles[i]->getNeighbor(1), 0, true);
	bc->setNeighbor(ac, 1, true);
	bc->setNeighbor(ab, 2, true);

	warn(".............ST6");
	// Inherit constraint category
	if (triangles[i]->isContraint(0)) {
		ab->setConstraint(0);
	}
	warn(".............ST7");
	if (triangles[i]->isContraint(1)) {
		bc->setConstraint(0);
	}
	warn(".............ST8");
	if (triangles[i]->isContraint(2)) {
		bc->setConstraint(2);
	}

	warn(".............ST9");
	// add the three new triangles
	addTriangle(ab);
	addTriangle(ac);
	addTriangle(bc);

	warn(".............ST10");
	// delete the old one
	deleteTriangle(i);


	return true;
}



bool Triangulation::splitEdge(size_t i, Point3D * p, int code, bool constraint)
{	
	if (triangles[i] == NULL)
		warn("Trying to split edge of NULL triangle "+std::to_string(i));

	// get vertices
	Point3D * a = triangles[i]->getVertex(code%3);
	Point3D * b = triangles[i]->getVertex((code+1) % 3);
	Point3D * c = triangles[i]->getVertex((code+2) % 3);

	// Get the oposite side
	Point3D * oposite = getOpositeVertex(triangles[i], code % 3);
	
	// retrieve neighbor (will be deleted)
	//Triangle * neighbor = triangles[i]->getNeighbor(code % 3);
	Triangle * neighbor = NULL;

	// Create and add the new triangles.
	Triangle * ac = new Triangle(a, p, c);
	addTriangle(ac);

	Triangle * bc = new Triangle(p, b, c);
	addTriangle(bc);

	Triangle * ap = NULL;
	Triangle * bp = NULL;

	if (neighbor != NULL) {		
		ap = new Triangle(a, oposite, p);
		addTriangle(ap);
		bp = new Triangle(oposite, b, p);
		addTriangle(bp);
	}

	ac->setNeighbor(ap, 0, true);
	ac->setNeighbor(bc, 1, true);
	ac->setNeighbor(triangles[i]->getNeighbor((code +5 ) % 3), 2, true);	

	bc->setNeighbor(bp, 0, true);
	bc->setNeighbor(triangles[i]->getNeighbor((code + 4)%3), 1, true);	
	// THIS IS DONE BY RECIPROCITY... I LEAVE IT 
	// HERE TO MAKE IT EXPLICIT
	//bc->setNeighbor(ac, 2, true);
	
	// Set constraints
	if (constraint) {
		ac->setConstraint(1);
		bc->setConstraint(2);
	}

	// inherit constraint category
	if (triangles[i]->isContraint(code % 3)) {
		ac->setConstraint(0);
		bc->setConstraint(0);
	}

	if (neighbor != NULL) {

		int aux1 = neighbor->getEdgeIndexByPoints(oposite, a); 
		if (aux1 < 0) {
			fatal("Inconsistent neighborhood when splitting edge!", __LINE__, __FILE__);
		}

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
		if (aux1 < 0) {
			fatal("Inconsistent neighborhood when splitting edge!", __LINE__, __FILE__);
		}
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

		// inherit constraint category
		if (triangles[i]->isContraint(code % 3)) {
			ap->setConstraint(2);
			bp->setConstraint(1);
		}
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
		if (!found)
			return neighbor->getVertex(i);
	}
	return NULL;
}


size_t Triangulation::addTriangle(Triangle * t) {		
	warn("adding triangle "+std::to_string(triangles.size()));
	t->setIndex(nTriangles);
	triangles.push_back(t);
	
	t->print();
/*	
#ifdef DEBUG
	
#endif
*/
	nTriangles += 1;
	return nTriangles;
}


void Triangulation::deleteTriangle(size_t i) {

#ifdef DEBUG
	DEBUG_MSG("Deleting triangle "+std::to_string(i));
#endif	
	if (triangles[i] == NULL) {
		fatal("Trying to delete an already deleted triangle", __LINE__, __FILE__);
	}
	delete triangles[i];
	triangles[i] = NULL;
}
/*
void Triangulation::applyConstraint(Segment * s)
{
	addPoint(s->start);
	addPoint(s->end);

	for (size_t i = 0; i < nTriangles; i++) {

		warn("............ Checking triangle " + std::to_string(i));
		if (triangles[i] == NULL)
			continue;

		applyConstraintToTriangle(triangles[i], s);		
	}
}
*/

void Triangulation::clean()
{
	for (size_t i = 0; i < nTriangles; i++) {
		if (triangles[i] == NULL)
			continue;

		Point3D center = triangles[i]->getCenter();
		if (!polygon->testPoint(center))
			deleteTriangle(i);
	}
}

void Triangulation::refine(double maxArea)
{	
	for (size_t i = 0; i < nTriangles; i++) {
		if (triangles[i] == NULL)
			continue;

		double area = triangles[i]->getArea();

		if (area < 9e-3)
			continue;

		// find the longest edge
		Segment * s = triangles[i]->getSegment(0);
		for (int j = 1; j < 3; j++) {
			if (s->getLength() < triangles[i]->getSegment(j)->getLength())
				s = triangles[i]->getSegment(j);
		}

		if (triangles[i]->getAspectRatio() > 1 || s->getLength() > 0.2) {			
			// add midpoint
			double dX = s->start->getX() + s->end->getX(); 
			double dY = s->start->getY() + s->end->getY(); 
			double dZ = s->start->getZ() + s->end->getZ();
			addPoint(new Point3D(dX/2,dY/2,dZ/2));
			restoreDelaunay();
		} 
		else if (area > maxArea) { 
			// if it is a skinny triangle, try to add the circumcenter
			Point3D c = triangles[i]->getCircumCenter();
			addPoint(new Point3D(c.getX(), c.getY(), c.getZ()));		
			restoreDelaunay();			
		}
	}
}


bool Triangulation::mesh(double maxArea)
{	

	// Create a constrained delaunay triangulation
	poly2tri();
	refine(maxArea);
	return true;
}

/*
bool Triangulation::applyConstraintToTriangle(Triangle * tri, Segment * constraint)
{
	// Identify where is the triangle intersected
	Point3D * i1 = NULL;
	Point3D * i2 = NULL;

	for (int i = 0; i < 3; i++) {
		Point3D inter = Point3D(0, 0, 0);
		if (tri->getSegment(i)->intersect(constraint,&inter)) {
			if (i1 == NULL) {
				i1 = new Point3D(inter);				
			}
			else if(i2 == NULL) {
				i2 = new Point3D(inter);
			}
		}
	}

	if (i1 == NULL || i2 == NULL) {
		// Does not intersect
		return true;
	}

	// Check where do they intersect
	int code1 = -1;
	tri->testPoint(i1, &code1);
	int code2 = -1;
	tri->testPoint(i2, &code2);

	// Split
	if (code1 < 3 && code2 < 3) {
		// Both are vertices... transform such edge in constraint
		int minCode = code2 > code1 ? code1 : code2;
		int maxCode = code2 < code1 ? code1 : code2;
		int c;
		if (minCode == 0 && maxCode == 1) {
			c = 0;
		}
		else if (minCode == 1 && maxCode == 2) {
			c = 1;
		}
		else if (minCode == 0 && maxCode == 2) {
			c = 2;
		}
		else {
			return true;
			//fatal("Impossible combination of code1 (" + std::to_string(code1) + ") and code2 " + "(" + std::to_string(code2) + ")", __LINE__, __FILE__);
		}
		return tri->setConstraint(c);
	}
	else if (code1 >= 3 && code2 < 3) {
		//i1 is on an edge, i2 on a vertex... split edge 
		splitEdge(tri->getIndex(), i1, code1,true);		
	}
	else if (code2 >= 3 && code1 < 3) {
		//i2 is on an edge, i1 on a vertex... split edge
		splitEdge(tri->getIndex(), i2, code2, true);
	}
	else if (code1 >= 3 && code2 >= 3) {
		// both are on edges... split edge twice
		addPoint(i1);
		addPoint(i2);
		Segment aux = Segment(i1, i2);
		applyConstraint(&aux);
	}
	else {
		fatal("Something wrong when applying constraint to triangle", __LINE__, __FILE__);
		return false;
	}

	return true;
}
*/


void Triangulation::poly2tri() {

	// Create a 2D version of this polygon
	Polygon3D * polygon2D = polygon->get2DPolygon();

	// Prepare data for restoring 3D
	Vector3D i = Vector3D(0, 0, 0);
	Vector3D j = Vector3D(0, 0, 0);
	Vector3D k = Vector3D(0, 0, 0);

	if (!polygon2D->getInverseAuxiliarAxes(polygon->getNormal(), &i, &j, &k)) {
		fatal("Impossible to triangulate because of an error in calculating inverse of auxiliar axes", __LINE__, __FILE__);
		return;
	}

	// The maximum number of points you expect to need
	// This value is used by the library to calculate
	// working memory required
	uint32_t MaxPointCount = MAX_POINTS_IN_WORKPLANE;

	// Request how much memory (in bytes) you should
	// allocate for the library
	size_t MemoryRequired = MPE_PolyMemoryRequired(MaxPointCount);

	// Allocate a void* memory block of size MemoryRequired
	// IMPORTANT: The memory must be zero initialized
	void* Memory = calloc(MemoryRequired, 1);

	// Initialize the poly context by passing the memory pointer,
	// and max number of points from before
	MPEPolyContext PolyContext;
	if (MPE_PolyInitContext(&PolyContext, Memory, MaxPointCount))
	{

		// Add exterior loop
		Loop * outerLoop = polygon2D->getOuterLoopRef();

		// This value is lost in translation... so store it here
		double z = outerLoop->getVertexRef(0)->getZ();

		for (size_t i = 0; i < outerLoop->size(); i++)
		{
			Point3D * p = outerLoop->getVertexRef(i);

			if (p == NULL)
				continue;

			// TRANSFORM TO 2D;
			// for now we assume the plane is on the XZ plane
			MPEPolyPoint* Point = MPE_PolyPushPoint(&PolyContext);
			Point->X = p->getX();
			Point->Y = p->getY();
		}


		// Add the polyline for the edge. This will consume all points added so far.
		MPE_PolyAddEdge(&PolyContext);

		// add holes to the shape 
		for (size_t i = 0; i < polygon2D->countInnerLoops(); i++)
		{
			Loop * innerLoop = polygon2D->getInnerLoopRef(i);
			MPEPolyPoint* Hole = MPE_PolyPushPointArray(&PolyContext, innerLoop->size());

			for (size_t j = 0; j < innerLoop->size(); j++) {
				Point3D * p = innerLoop->getVertexRef(j);
				if (p == NULL)
					continue;
				Hole[j].X = p->getX();
				Hole[j].Y = p->getY();
			}

			MPE_PolyAddHole(&PolyContext);
		}

		// Triangulate the shape
		MPE_PolyTriangulate(&PolyContext);

		// The resulting triangles can be used like so
		for (uxx TriangleIndex = 0; TriangleIndex < PolyContext.TriangleCount; ++TriangleIndex)
		{
			MPEPolyTriangle* polytriangle = PolyContext.Triangles[TriangleIndex];
			MPEPolyPoint* PointA = polytriangle->Points[0];
			MPEPolyPoint* PointB = polytriangle->Points[1];
			MPEPolyPoint* PointC = polytriangle->Points[2];

			// add a Triangle, transformed back into 3D			
			Point3D a2d = Point3D(PointA->X, PointA->Y, z);
			Point3D b2d = Point3D(PointB->X, PointB->Y, z);
			Point3D c2d = Point3D(PointC->X, PointC->Y, z);
			Point3D * a = new Point3D(a2d.transform(i, j, k));
			Point3D * b = new Point3D(b2d.transform(i, j, k));
			Point3D * c = new Point3D(c2d.transform(i, j, k));
			Triangle * t = new Triangle(a, b, c);

			// Set constraints.... this was reversed engineered; so I am not sure 
			// it will work ALL the time.
			for (size_t i = 0; i < 3; i++) {
				MPEPolyTriangle * polyneighbor = polytriangle->Neighbors[(i + 2) % 3];
				// If there is no neighbor
				if (polyneighbor != NULL && (polyneighbor->Flags) < 256)
					t->setConstraint(i);
			}
			addTriangle(t);
		}
	}

	// delete this.
	delete polygon2D;

	resetNeighborhoods();
}


void Triangulation::resetNeighborhoods()
{

	if (nTriangles == 1)
		return;

	// iterate all triangles
	for (size_t i = 0; i < nTriangles; i++) {
		// get this triangle
		Triangle * iTriangle = triangles[i];

		// Check if this triangle is already populated
		if (iTriangle->getNeighbor(0) != NULL && iTriangle->getNeighbor(1) != NULL && iTriangle->getNeighbor(2) != NULL)
			break;

		// Iterate all other triangles. Reciprocity should take care
		// of the rest.
		for (size_t j = i + 1; j < nTriangles; j++) {
			// get the other triangle
			Triangle * jTriangle = triangles[j];

			// Check if the Triangles share an edge, and set neighborhood if needed	
			bool aux = false;
			for (int iEdge = 0; iEdge < 3; iEdge++) {

				if (aux)
					break;

				Segment * iSegment = triangles[i]->getSegment(iEdge);

				for (int jEdge = 0; jEdge < 3; jEdge++) {
					Segment * jSegment = triangles[j]->getSegment(jEdge);
					// if they share an edge
					if (iSegment->isEqual(jSegment)) {
						// set neighborhood
						iTriangle->setNeighbor(jTriangle, iEdge, false);
						jTriangle->setNeighbor(iTriangle, jEdge, false);
						aux = true;
						break;
					}
				}
			}
		}
	}
}

