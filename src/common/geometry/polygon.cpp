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

#include "./polygon.h"
#include "config_constants.h"
#include "common/utilities/io.h"

Polygon3D::Polygon3D() 
{
	outerLoop = new Loop();
	innerLoops = std::vector< Loop * >();
};

Polygon3D::~Polygon3D() 
{

	//remove exterior loop
	delete outerLoop;

	//remove interior loops
	for (unsigned int i = 0; i < innerLoops.size(); i++) {
		delete innerLoops[i];
	}
	
}


Loop * Polygon3D::getOuterLoopRef()
{
	return outerLoop;
}

Loop * Polygon3D::addInnerLoop() 
{
	Loop * newLoop = new Loop();
	innerLoops.push_back(newLoop);
	return newLoop;
}

double Polygon3D::getArea() 
{
	return area;
}

void Polygon3D::setArea(double newArea) 
{
	area = newArea;
}


bool Polygon3D::hasInnerLoops() 
{
	return innerLoops.size() > 0;
}



size_t Polygon3D::countInnerLoops() 
{
	return innerLoops.size();
}


Loop * Polygon3D::getClosedLoop() 
{
	//get the number of interior loops
	size_t numInnerLoops = innerLoops.size();
	
	//initialize the loop by cloning the current outer loop
	Loop * loop = new Loop(outerLoop);

	std::vector <int> processedLoops;
	int loopID;
	int vertexID;

	// This is done once per inner loop
	for (int i = 0; i < numInnerLoops; i++) {
		// find the minimum distance 
		// from interior to exterior
		double minDistance = HUGE;
		int minInnerLoopID;
		int minExtVertexID;
		int minIntVertexID;

		size_t numExtVertex = loop->size();		
		for (int j = 0; j < numExtVertex; j++) {
			Point3D * extVertex = loop->getVertexRef(j);
			
			if (extVertex == NULL)
				continue;			

			for (int k = 0; k < numInnerLoops; k++) {

				// continue if already processed
				if (std::find(processedLoops.begin(), processedLoops.end(), k) != processedLoops.end()) {
					continue;
				}

				Loop * innerLoop = innerLoops[k];
				size_t numInnerVertices = innerLoop->size();
				for (int l = 0; l < numInnerVertices; l++) {

					Point3D * innerVertex = innerLoop->getVertexRef(l);

					if (innerVertex == NULL) 
						continue;

					// we work with squared distances... the result is
					// the same but the calculation is faster.
					double distance = extVertex->squaredDistanceTo(innerVertex);
					
					if (distance < minDistance) {
						minDistance = distance;
						minExtVertexID = j;
						minIntVertexID = l;
						minInnerLoopID = k;
						loopID = k;
						vertexID = l;
					}
				} //end iterating inner vertices
			} // end iterating inner loops		
		} // end iterating exterior vertices

		
		// Now, pass the inner loop to the exterior loop
		// by connecting them
		// (this comes from the SketchUp version... check normal, 
		// to see if the loops are in the appropriate order)
		Loop * aux = new Loop();
		for (int i = 0; i < numExtVertex; i++) {

			Point3D * extVertex = loop->getVertexRef(i);

			if (extVertex == NULL)
				continue;

			aux->addVertex(new Point3D(extVertex));
			
			if ( i == minExtVertexID) {			
				// add the loop
				size_t numInnerLoopVertices =  innerLoops[minInnerLoopID]->size();
				for (size_t j = 0; j < numInnerLoopVertices; j++) {					
					int vertexToAdd = (vertexID++ % numInnerLoopVertices);
					if (innerLoops[minInnerLoopID]->getVertexRef(vertexToAdd) == NULL) 
						continue;
					
					Point3D * newVertex = new Point3D(innerLoops[minInnerLoopID]->getVertexRef(vertexToAdd));					
					aux->addVertex(newVertex);
				}
				//add the first vertex again
				aux->addVertex(new Point3D(innerLoops[minInnerLoopID]->getVertexRef(minIntVertexID)));
				//return to exterior loop
				aux->addVertex(new Point3D(extVertex));
			}
		}
		
		delete loop;		
		loop = aux;
		// flag loop as processed (instead of deleting it)
		processedLoops.push_back(loopID);		
	} // end iterating inner loops

	return loop;
} //end of close face function

void Polygon3D::clean() {
	for (size_t i = 0; i < innerLoops.size(); i++) {
		innerLoops[i]->clean();			
	}

	outerLoop->clean();
}


Loop * Polygon3D::getInnerLoopRef(size_t i) 
{
	return innerLoops[i];
}


void Polygon3D::setNormal(Vector3D newNormal)
{
	normal = newNormal;
}

Vector3D Polygon3D::getNormal()
{
	return normal;
}


bool Polygon3D::testPoint(Point3D p)
{		
	if (normal.isZero()) {
		FATAL(errorMessage,"Trying to test a point in a polygon without normal");		
		return false;
	}
	Loop * outerLoop = getOuterLoopRef();
	if (!outerLoop->testPoint(p, &normal))
		return false;

	for (size_t i = 0; i < countInnerLoops(); i++) {
		// if it lies inside any of the holes, return false
		if (getInnerLoopRef(i)->testPoint(p, &normal))			
			return false;		
	}
	return true;
}


Polygon3D * Polygon3D::get2DPolygon()
{
	Vector3D i = Vector3D(0, 0, 0); 
	Vector3D j = Vector3D(0, 0, 0);
	Vector3D k = Vector3D(0, 0, 0);

	if (!getAuxiliarAxes(normal,&i, &j, &k))
		return NULL;


	// Create returning object
	Polygon3D * ret = new Polygon3D();

	// Add outer loop
	Loop * ol = ret->getOuterLoopRef();
	for (size_t p = 0; p < outerLoop->size(); p++) {
      Point3D * v = outerLoop->getVertexRef(p);
      if (v == NULL)
        continue;

	  ol->addVertex(new Point3D(v->transform(i, j, k)));
	}

	// Add inner loops
	for (size_t p = 0; p < countInnerLoops(); p++) {
		Loop * il = ret->addInnerLoop();
		
		for (size_t q = 0; q < getInnerLoopRef(p)->size(); q++) {
			il->addVertex(new Point3D(getInnerLoopRef(p)->getVertexRef(q)->transform(i, j, k)));
		}
	}

	return ret;
}


Polygon3D * Polygon3D::restore3DPolygon(Vector3D normal)
{	
	Vector3D i = Vector3D(0, 0, 0);
	Vector3D j = Vector3D(0, 0, 0);
	Vector3D k = Vector3D(0, 0, 0);
	if (!getInverseAuxiliarAxes(normal, &i, &j, &k))
		return NULL;
	
	// Create returning object
	Polygon3D * ret = new Polygon3D();

	// Add outer loop
	Loop * ol = ret->getOuterLoopRef();
	for (size_t p = 0; p < outerLoop->size(); p++) {
		ol->addVertex(new Point3D(outerLoop->getVertexRef(p)->transform(i, j, k)));
	}

	// Add inner loops
	for (size_t p = 0; p < countInnerLoops(); p++) {
		Loop * il = ret->addInnerLoop();

		for (size_t q = 0; q < getInnerLoopRef(p)->size(); q++) {
			il->addVertex(new Point3D(getInnerLoopRef(p)->getVertexRef(q)->transform(i, j, k)));
		}
	}

	return ret;
}


bool Polygon3D::getInverseAuxiliarAxes(Vector3D normal, Vector3D * auxi, Vector3D * auxj, Vector3D * auxk)
{
	Vector3D i = Vector3D(0, 0, 0);
	Vector3D j = Vector3D(0, 0, 0);
	Vector3D k = Vector3D(0, 0, 0);

	if (!getAuxiliarAxes(normal, &i, &j, &k))
		return false;

	double det = i.getX()*(j.getY()*k.getZ() - j.getZ()*k.getY())
		- i.getY()*(j.getX()*k.getZ() - j.getZ()*k.getX())
		+ i.getZ()*(j.getX()*k.getY() - j.getY()*k.getX());

	if (abs(det) < GLARE_TINY) {
		FATAL(errorMessage,"Determinant is zero when trying to ");
		normal.print();
		return NULL;
	}

	// CALCULATE INVERSE MATRIX
	*auxi = Vector3D(
		j.getY()*k.getZ() - j.getZ()*k.getY(),
		i.getZ()*k.getY() - i.getY()*k.getZ(),
		i.getY()*j.getZ() - i.getZ()*j.getY()
	) / det;

	*auxj = Vector3D(
		j.getZ()*k.getX() - j.getX()*k.getZ(),
		i.getX()*k.getZ() - i.getZ()*k.getX(),
		i.getZ()*j.getX() - i.getX()*j.getZ()
	) / det;
	*auxk = Vector3D(
		j.getX()*k.getY() - j.getY()*k.getX(),
		i.getY()*k.getX() - i.getX()*k.getY(),
		i.getX()*j.getY() - i.getY()*j.getX()
	) / det;

	return true;
}

bool Polygon3D::getAuxiliarAxes(Vector3D normal, Vector3D * auxi, Vector3D * auxj, Vector3D * auxk)
{
	if (normal.isZero()) {
      FATAL(errorMessage,"Trying to get auxiliar axes with a Zero normal");
	  return false;
	}
		
	Vector3D k = normal;
	k.normalize();
	Vector3D i = Vector3D(0, 0, 0);
	Vector3D j = i;

	double nx = k.getX();
	double ny = k.getY();
	double nz = k.getZ();	
	
	if (abs(nz) < GLARE_TINY) {
		// Perfectly vertical planes	
		i = Vector3D(0, 0, 1);
		j = k%i;	
	}
	else {
		// All other workplanes
		if (abs(nx) > GLARE_TINY) {
          i = Vector3D(-ny/nx,1,0);
          i.normalize();
          /*
			i = Vector3D(1, 0, -nz / nx);
			i.normalize();
          */
			j = k%i;
		}
		else if (abs(ny) > GLARE_TINY) {
          j = Vector3D(1, -nx / ny, 0);
          j.normalize();
          /*
            j = Vector3D(0, 1, -nz / ny);
			j.normalize();
          */
			i = j%k;
		}else if(abs(nx) < GLARE_TINY && abs(ny) < GLARE_TINY){
            i = Vector3D(1, 0, 0);
			j = k%i;
		}
		else {
			FATAL(errorMessage,"Not considered situation when calculating auxiliar axes of polygon");
			normal.print();
			return false;
		}
	}
	
	// set values
	*auxi = i; *auxj = j; *auxk = k;
	
    if (i.getLength() < GLARE_TINY || j.getLength() < GLARE_TINY || k.getLength() < GLARE_TINY) {
      i.print();
      j.print();
      k.print();
      std::cout << "normal " << std::endl;
      normal.print();
      FATAL(errmsg, "Auxiliar axes are bad defined");      
    }

	return true;
}