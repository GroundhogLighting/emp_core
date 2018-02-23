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

#include <cmath>
#include "./loop.h"
#include "../../common/utilities/io.h"
#include "../../config_constants.h"
#include "./segment.h"

Loop::Loop() 
{
	vertices = std::vector < Point3D * >();
}

Loop::Loop(Loop * loop) 
{
	vertices = std::vector < Point3D * >();
	size_t numVertices = loop->size();
	for (int i = 0; i < numVertices; i++) {
		if (loop->getVertexRef(i) == NULL)
			continue;

		addVertex(new Point3D(loop->getVertexRef(i)));
	}
}

Loop::~Loop() 
{

	for (unsigned i = 0; i < vertices.size(); i++) {
		delete vertices[i];
	}

}

void Loop::addVertex(Point3D * point) 
{
	vertices.push_back(point);
}


size_t Loop::size() 
{
	return vertices.size();
}


size_t Loop::realSize()
{
	size_t n = 0;
	for (size_t i = 0; i < size(); i++) {
		if (vertices[i] != NULL)
			n++;
	}
	return n;
}


Point3D * Loop::getVertexRef(size_t i) 
{
	return vertices[i];
}

void Loop::clean() {	
	
	// Checks for colinear points
	size_t nVertex = size();
	
	Point3D * prevPoint;
	Point3D * thisPoint;
	Point3D * nextPoint;

	size_t aux = 1; // how many vertex before is the previous non-NULL one? 
	for (size_t i = 1; i < nVertex+1; i++) {
		
		// This may be re-cleaning.
		if (vertices[i%nVertex] == NULL) {
			aux += 1;
			continue;
		}

		prevPoint = vertices[(i-aux)%nVertex];
		thisPoint = vertices[(i)%nVertex];
		nextPoint = vertices[(i+1) % nVertex];

		size_t j = (i + 1) % nVertex;
		while (nextPoint == NULL) {			
			nextPoint = vertices[++j % nVertex];
		}

		Vector3D a = *thisPoint - *prevPoint;
		Vector3D b = *nextPoint - *thisPoint;

		bool areCollinear = (a%b).isZero();		

		if (areCollinear) {
			delete vertices[i%nVertex];
			vertices[i%nVertex] = NULL; 
			aux += 1;
		}
		else {
			aux = 1; // reset aux
		}
	}

}


bool Loop::testPoint(Point3D p, Vector3D * normal)
{
	
	Point3D * firstPoint = vertices[0];
	// in case firstPoint is NULL
	size_t nVertex = size();
	size_t j = 0;
	while (j < nVertex) {
		if (vertices[j] != NULL) {
			firstPoint = vertices[j];
			break;
		}
		j++;
	}

	// Get the relative axes
	normal->normalize();
	Vector3D xp = *firstPoint-p;
	xp.normalize();
	Vector3D yp = *normal % xp; 
	// yp and normal are unit vectors, thus yp also

	// Check if coplanar
	if (std::abs(*normal*xp) > EMP_TINY) {
		warn("Testing a Point3D on Loop: Point3D is not coplanar with Loop");
		return false;
	}
	

	/*
		Alciatore, D., & Miranda, R. (1995). 
		A winding number and point-in-polygon algorithm. 
		Glaxo Virtual Anatomy Project Research Report, 
		Department of Mechanical Engineering, Colorado State University.
	*/

	// 1. Initialize the winding number to zero
	double wNumber = 0; 
	double previousY = 0;
	double previousX = xp*(*firstPoint - p);
	
	// 2. For each segment of the polygon
	for (size_t i = j+1; i < nVertex+1; i++) {
		Point3D * vertex = getVertexRef(i%nVertex);

		// in case it was cleaned
		if (vertex == NULL)
			continue;

		// 2.1 Determine whether the segment crosses 'dir'	
		Vector3D v = *vertex - p;

		// determine x and y
		double y = v*yp;
		double x = v*xp;
		
		if (std::abs(y)<EMP_TINY && std::abs(previousY)<EMP_TINY) {
			continue; // no rotation! Moving along the X-axis
		}
		else if (previousY*y < EMP_MINUS_TINY) {
			// Crosses the axis
			double r = previousX + previousY*(x - previousX) / (previousY-y);
			if (r > 0) { // if crosses on the correct side
				if (previousY < 0) {
					// crosses positive
					wNumber += 1;
				}
				else {
					wNumber -= 1;
				}
			}
		}
		else if (std::abs(previousY) < EMP_TINY && previousX > 0) {
			// previous V was on the positive X-axis
			if (y > 0) {
				wNumber += 0.5;
			}
			else {
				wNumber -= 0.5;
			}
		}
		else if (std::abs(y) < EMP_TINY && x > 0) {
			// current V is on positive X-axis
			if (previousY < 0) {
				wNumber += 0.5;
			}
			else {
				wNumber -= 0.5;
			}
		}
		previousY = y;
		previousX = x;
	}

	return wNumber != 0;
}



Loop * Loop::transform(Vector3D i, Vector3D j, Vector3D k)
{
	Loop * ret = new Loop();
	for (size_t v = 0; v < size(); v++) {
		Point3D p = vertices[v]->transform(i, j, k);
		ret->addVertex(new Point3D(&p));
	}
	return ret;
}
