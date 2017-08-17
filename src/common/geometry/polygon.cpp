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

#include "../utilities/io.h"

Polygon3D::Polygon3D() 
{
	DEBUG_MSG("Creating polygon");
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

	DEBUG_MSG("Destroying polygon");
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

	// if this happens, there is no need to close
	// and the standard method should be used.
	// Should not fail, though... just loose performance.
	if (numInnerLoops == 0) {
		return loop;
	}

	std::vector <int> processedLoops;
	int loopID;
	int vertexID;

	// This is done once per inner loop
	for (int i = 0; i < numInnerLoops; i++) {
		// find the minimum distance 
		// from interior to exterior
		double minDistance = 1e19;
		int minInnerLoopID;
		int minExtVertexID;
		int minIntVertexID;

		size_t numExtVertex = loop->size();		
		for (int j = 0; j < numExtVertex; j++) {
			Point3D * extVertex = loop->getVertexRef(j);
			
			for (int k = 0; k < numInnerLoops; k++) {
				// continue if already processed
				if (std::find(processedLoops.begin(), processedLoops.end(), k) != processedLoops.end()) {
					continue;
				}

				Loop * innerLoop = innerLoops[k];
				size_t numInnerVertices = innerLoop->size();
				for (int l = 0; l < numInnerVertices; l++) {					
					Point3D * innerVertex = innerLoop->getVertexRef(l);

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
			aux->addVertex(new Point3D(extVertex));
			
			if ( i == minExtVertexID) {			
				// add the loop
				size_t numInnerLoopVertices =  innerLoops[minInnerLoopID]->size();
				for (size_t j = 0; j < numInnerLoopVertices; j++) {				
					int vertexToAdd = (vertexID-- % numInnerLoopVertices);					
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
		loop = new Loop(aux);
		delete aux;
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
		fatal("Trying to test a point in a polygon without normal", __LINE__, __FILE__);
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