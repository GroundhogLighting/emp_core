/*****************************************************************************
	Emp

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
#include "common/utilities/io.h"
#include "config_constants.h"

#define MPE_POLY2TRI_IMPLEMENTATION
#include "fast-poly2tri/MPE_fastpoly2tri.h"

Triangulation::Triangulation()
{

}

Triangulation::Triangulation(Polygon3D * aPolygon)
{
	// initialize without points
	polygon = aPolygon;
	triangles = std::vector < Triangle * >();	    
}

Triangulation::~Triangulation()
{
	for (size_t i = 0; i < nTriangles;i++) {
		if (triangles[i] == NULL)
			continue;

		delete triangles[i];
	}
}


size_t Triangulation::addTriangle(Triangle * t) {
	t->setIndex(nTriangles);
	
	triangles.push_back(t);
	
	nTriangles += 1;
	return nTriangles;
}


size_t Triangulation::getNumTriangles()
{
	return nTriangles;
}


Triangle * Triangulation::getTriangleRef(size_t i)
{
	return triangles[i];
}

bool Triangulation::addPointToTriangle(size_t index, Point3D * point, int code)
{
	if (triangles[index] == NULL) {
		FATAL(errorMessage,"Trying to add point into an obsolete triangle");
		return false;
	}

	if (code < 3) { // Point is a vertex 
					// do nothing... the point has already been added.
		return true;
	}
	else if (code < 6) { // Point in an edge			
		splitEdge(index, point, code);
		return true;
	}
	else { // Point inside the triangle (code == 6)
		return splitTriangle(index, point);
	}
}

bool Triangulation::addPoint(Point3D * point, bool warn)
{
	// Iterate through triangles to check
	int code;
	for (size_t i = 0; i < triangles.size(); i++) {
		// skip triangle if it has been deleted
		if (triangles[i] == NULL)
			continue;
		
		if (triangles[i]->testPoint( point, &code))
			return addPointToTriangle(i, point, code);
					
	}

    if (warn) {
	  FATAL(errorMessage,"Point is not in any triangle");
    }
	
    return false; // triangle not found
}


void Triangulation::flipDiagonal(size_t index, int edge)
{
	if (edge < 0 || edge > 2) {
      FATAL(errorMessage,"Impossible index when flipping digonal... index was '" + std::to_string(edge) + "'");	  
	}
	
	// get neighbor
	Triangle * neighbor = triangles[index]->getNeighbor(edge);

	if (neighbor == NULL) {
      FATAL(errorMessage,"Trying to flip a diagonal with a NULL neighbor");
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

	

	// Set neighbors
	int aux1 = neighbor->getEdgeIndexByPoints(oposite, a);
	if (aux1 < 0) {
      FATAL(errorMessage,"Inconsistent neighborhood when flipping edge");
	  return;
	}
	Triangle * aux2 = neighbor->getNeighbor(aux1);
	if (aux2 != NULL) {
		size_t aux3 = aux2->getIndex();
		flipped1->setNeighbor(triangles[aux3], 0, true);
	}

	flipped1->setNeighbor(flipped2, 1,true);
	flipped1->setNeighbor(triangles[index]->getNeighbor((edge+2)%3),2,true);
	// flipped2->setNeighbor(flipped2, 0,false); // THIS IS DONE BY RECIPROCITY... PUT HERE TO MAKE IT EXPLICIT

	
	int aux12 = neighbor->getEdgeIndexByPoints(oposite, b);
	if (aux12 < 0) {
      FATAL(errorMessage,"Inconsistent neighborhood when flipping edge -- Line");
	  return;
	}
	
	Triangle * aux22 = neighbor->getNeighbor(aux12);
	
	if (aux22 != NULL) {
		size_t aux3 = aux22->getIndex();
		flipped2->setNeighbor(triangles[aux3], 1, true);
	}

	flipped2->setNeighbor(triangles[index]->getNeighbor((edge + 1) % 3), 2, true);

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


double Triangulation::getBestAspectRatio(Triangle * t, int i)
{
	if (i < 0 || i > 2) {
      FATAL(errorMessage,"Impossible index when getting best aspect ratio... index was '" + std::to_string(i) + "'");
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
		return flippedMin;
	}

	return currentMin;
}


void Triangulation::restoreDelaunay()
{	
	
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
		for (int j = 0; j < 3; j++) { // Check three edges for flipping 
			// calculate possible aspect ratio		
			double ar = getBestAspectRatio(triangles[i], j);
			if (ar == -1) // Null neighbor or constraint
				continue;
			
			if ( (currentAspectRatio - ar) > EMP_TINY  &&  (bestAspectRatio-ar) > EMP_TINY) {
				bestNeighbor = j;
				bestAspectRatio = ar;
			}		
		}

		// Flip if needed.
		if (bestNeighbor >= 0 ) { 
			// if at least one of them is better than the current
			flipDiagonal(i, bestNeighbor);
		} // else... do nothing

	}
}


bool Triangulation::splitTriangle(size_t i, Point3D * p)
{	

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

	// Inherit constraint category
	if (triangles[i]->isContraint(0)) {
		ab->setConstraint(0);
	}
	
	if (triangles[i]->isContraint(1)) {
		bc->setConstraint(0);
	}
	
	if (triangles[i]->isContraint(2)) {
		bc->setConstraint(2);
	}

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
	if (triangles[i] == NULL) {
      std::string e = "Trying to split edge of NULL triangle " + std::to_string(i);
      warn(&e[0]);
	  return false;
	}

	// get vertices
	Point3D * a = triangles[i]->getVertex(code%3);
	Point3D * b = triangles[i]->getVertex((code+1) % 3);
	Point3D * c = triangles[i]->getVertex((code+2) % 3);

	// Get the oposite side
	Point3D * oposite = getOpositeVertex(triangles[i], code % 3);
	
	// retrieve neighbor (will be deleted)
	Triangle * neighbor = triangles[i]->getNeighbor(code % 3);
	//Triangle * neighbor = NULL;

	
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
	
	

	// inherit constraint category
	if (triangles[i]->isContraint(code % 3)) {
		ac->setConstraint(0);
		bc->setConstraint(0);
	}

	if (neighbor != NULL) {

		int aux1 = neighbor->getEdgeIndexByPoints(oposite, a); 
		if (aux1 < 0) {
			FATAL(errorMessage,"Inconsistent neighborhood when splitting edge!");
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
			FATAL(errorMessage,"Inconsistent neighborhood when splitting edge!");
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

Point3D * Triangulation::getOpositeVertex(Triangle * t, int nei) {
	
	Triangle * neighbor = t->getNeighbor(nei);
	if (neighbor == NULL)
		return NULL;

	// find the vertex that is in neighbor but not in triangle.
	for (int i = 0; i < 3; i++) { // count in neighbor		
		bool found = false;
		for (int j = 0; j < 3; j++) { // count in this
			if (neighbor->getVertex(i)->isEqual(t->getVertex(j))) {
				found = true;
				break;
			}
		}
		if (!found)
			return neighbor->getVertex(i);
	}
	return NULL;
}



void Triangulation::deleteTriangle(size_t i) {

	if (triangles[i] == NULL) {
		FATAL(errorMessage,"Trying to delete an already deleted triangle");
	}
	delete triangles[i];
	triangles[i] = NULL;
}

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

void Triangulation::refine(double maxArea, double maxAspectRatio)
{	
	for (size_t i = 0; i < nTriangles; i++) {
		if (triangles[i] == NULL)
			continue;

		double area = triangles[i]->getArea();

		if (area < 9e-3)
			continue;

		// find the longest edge
		Segment * s = triangles[i]->getSegment(0);
		int longestSegmentIndex= 0;
		for (int j = 1; j < 3; j++) {
			if (s->getLength() < triangles[i]->getSegment(j)->getLength()) {
				longestSegmentIndex = j;
				s = triangles[i]->getSegment(j);
			}
		}

		if (triangles[i]->getAspectRatio() > maxAspectRatio) {
			// add midpoint
			double dX = s->start->getX() + s->end->getX(); 
			double dY = s->start->getY() + s->end->getY(); 
			double dZ = s->start->getZ() + s->end->getZ();
			addPointToTriangle(i,new Point3D(dX/2,dY/2,dZ/2), longestSegmentIndex + 3);
			restoreDelaunay();
		} 
		else if (area > maxArea) { 
			// if it is a skinny triangle, try to add the circumcenter
			Point3D c = triangles[i]->getCircumCenter();
			// Since the circumcenter may be in another triangle, we need 
			// to search for it.
			addPoint(new Point3D(c.getX(), c.getY(), c.getZ()),false);		
			restoreDelaunay();			
		}
	}
}


bool Triangulation::mesh(double maxArea, double maxAspectRatio)
{	

	// Create a constrained delaunay triangulation
  if (!doCDT())
    return false;

  refine(maxArea, maxAspectRatio);
  return true;
}


bool Triangulation::doCDT() {

	// Create a 2D version of this polygon
	Polygon3D * polygon2D = polygon->get2DPolygon();

	// Prepare data for restoring 3D
	Vector3D i = Vector3D(0, 0, 0);
	Vector3D j = Vector3D(0, 0, 0);
	Vector3D k = Vector3D(0, 0, 0);

	if (!polygon2D->getInverseAuxiliarAxes(polygon->getNormal(), &i, &j, &k)) {
		FATAL(errorMessage,"Impossible to triangulate because of an error in calculating inverse of auxiliar axes");
		return false;
	}
    

	// The maximum number of points you expect to need
	// This value is used by the library to calculate
	// working memory required
    uint32_t MaxPointCount;
    if (polygon2D->getOuterLoopRef()->size() == 3 || ! polygon2D->hasInnerLoops() ) {
      MaxPointCount = 3;
    }
    else {
      MaxPointCount = EMP_MAX_POINTS_IN_WORKPLANE;
    } 

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

			if (p == nullptr)
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
			MPEPolyPoint* Hole = MPE_PolyPushPointArray(&PolyContext, static_cast<u32>(innerLoop->size()));

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

				if (polyneighbor != nullptr && (polyneighbor->Flags) < 256)
					t->setConstraint(static_cast<u32>(i));
			}
			addTriangle(t);
		}
	}

	// delete this.
	delete polygon2D;

	resetNeighborhoods();
    return true;
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

Polygon3D * Triangulation::getPolygon()
{
  return polygon;
}

size_t Triangulation::realSize()
{
  size_t count = 0;
  for (auto triangle : triangles) {
    if (triangle == nullptr)
      continue;

    count++;
  }
  return count;
}

void Triangulation::purge()
{
  size_t realN = realSize();
  std::vector<Triangle *> realTriangles = std::vector<Triangle *>(realN);
  size_t count = 0;
  for (auto triangle : triangles) {
    // Skip if nullptr
    if (triangle == nullptr)
      continue;

    realTriangles[count] = triangle;

    count++;
  }
  // Update
  nTriangles = realN;
  triangles.resize(realN);
  triangles = std::vector<Triangle *>(realTriangles);
}
