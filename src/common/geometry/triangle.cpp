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

#include "../utilities/io.h"
#include "../../config_constants.h"
#include "./point3d.h"
#include "./triangle.h"



Triangle::Triangle(Point3D * a, Point3D * b, Point3D * c)
{
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;

	segments[0] = new Segment(a, b);
	segments[1] = new Segment(b, c);
	segments[2] = new Segment(c, a);

}

Triangle::~Triangle()
{
	for (int i = 0; i < 3; i++) {      	
      delete segments[i];
	}
}

Point3D * Triangle::getVertex(int i)
{	
	if (i < 0 || i > 3) {
      FATAL(errorMessage,"Impossible index when getting Vertex... index was '" + std::to_string(i) + "'");
	  return NULL;
	}
	return vertices[i];
}

double Triangle::getCircumradius()
{
	if (circumradius > 0)
		return circumradius;

	double a = segments[0]->getLength();
	double b = segments[1]->getLength();
	double c = segments[2]->getLength();
	double s = (a + b + c)*(b + c - a)*(c + a - b)*(a + b - c);
	circumradius = a*b*c / sqrt(s);
	return circumradius;
}

double Triangle::getAspectRatio()
{
	if (aspectRatio > 0)
		return aspectRatio;

	double minSegment = HUGE; 
	for (int i = 0; i < 3; i++) {
		if (segments[i]->getLength() < minSegment) {
			minSegment = segments[i]->getLength();
		}
	}
	aspectRatio = getCircumradius() / minSegment;
	return aspectRatio;
}

bool Triangle::setNeighbor(Triangle * t, int i,bool reciprocity) 
{
	if (i < 0 || i > 2) {
      FATAL(errorMessage,"Impossible index when Setting neighbor... index was '" + std::to_string(i) + "'");
	  return false;
	}
	neighbors[i] = t;
	
	// reciprocity	
	if (reciprocity && t != NULL)		
		t->setNeighbor(this,t->getEdgeIndexByPoints(segments[i]->start, segments[i]->end),false);
	

	return true;
}


Segment * Triangle::getSegment(int i)
{
	if (i < 0 || i > 2) {
      FATAL(errorMessage,"Impossible index when getting segment... index was '" + std::to_string(i) + "'");
	  return NULL;
	}
	return segments[i];
}


Triangle * Triangle::getNeighbor(int i)
{
	if (i < 0 || i > 2) {
      FATAL(errorMessage,"Impossible index when getting neighbor... index was '" + std::to_string(i) + "'");
	  return NULL;
	}
	return neighbors[i];
}


int Triangle::getEdgeIndexByPoints(Point3D * a, Point3D * b)
{	
	for (int i = 0; i < 3; i++) {
		if (a->isEqual(segments[i]->start) && b->isEqual(segments[i]->end)) {
			return i;
		}
		if (b->isEqual(segments[i]->start) && a->isEqual(segments[i]->end)) {
			return i;
		}
	}
	return -1;
}


size_t Triangle::setIndex(size_t i)
{
	index = i;
	return i;
}


size_t Triangle::getIndex()
{
	return index;
}

bool Triangle::setConstraint(int i)
{
	if (i < 0 || i > 2) {
      FATAL(errorMessage,"Impossible index when setting constraint... index was '" + std::to_string(i) + "'");
	  return false;
	}

	constraints[i] = true;
	
	// reciprocate
	Segment * s = segments[i];
	if (neighbors[i] != NULL) {
		int aux = neighbors[i]->getEdgeIndexByPoints(s->start, s->end);
		neighbors[i]->constraints[aux] = true;
	}
	

	return true;
}


bool Triangle::isContraint(int i)
{
	if (i < 0 || i > 2) {
      FATAL(errorMessage,"Impossible index when getting constraint... index was '" + std::to_string(i) + "'");
	  return false;
	}	
	return constraints[i];
}

Point3D Triangle::getCenter()
{
	double dX = 0;
	double dY = 0;
	double dZ = 0;
	for (int i = 0; i < 3; i++) {
dX += vertices[i]->getX();
dY += vertices[i]->getY();
dZ += vertices[i]->getZ();
	}
	return Point3D(dX / 3, dY / 3, dZ / 3);
}


Point3D Triangle::getCircumCenter()
{
	Vector3D ab = *vertices[1] - *vertices[0];
	Vector3D ac = *vertices[2] - *vertices[0];
	//Vector3D bc = *vertices[2] - *vertices[1];

	Vector3D abXac = ab%ac;

	Vector3D aCenter = ((abXac%ab)*ac.getSquaredLength() + ac%abXac * ab.getSquaredLength()) / (2 * abXac.getSquaredLength());

	return *vertices[0] + aCenter;

}

double Triangle::getArea()
{
	double a = segments[0]->getLength();
	double b = segments[1]->getLength();
	double c = segments[2]->getLength();

	return sqrt((c + b + a)*((c + b + a) / 2 - a)*((c + b + a) / 2 - b)*((c + b + a) / 2 - c) / 2);
}

void Triangle::print()
{
	for (int i = 0; i < 3; i++) {
		std::cerr << ".... Vertex " << i << ": ";
		vertices[i]->print();
	}
}

bool Triangle::testPoint(Point3D * p, int * code)
{
	/*
	Source: http://blackpawn.com/texts/pointinpoly/
	*/
	// get vertices
	Point3D * a = vertices[0];
	Point3D * b = vertices[1];
	Point3D * c = vertices[2];

	// Compute vectors        
	Vector3D v0 = *c - a;
	Vector3D v1 = *b - a;
	Vector3D v2 = *p - a;

	// Compute dot products
	double dot00 = v0 * v0;
	double dot01 = v0 * v1;
	double dot02 = v0 * v2;
	double dot11 = v1 * v1;
	double dot12 = v1 * v2;

	// Compute barycentric coordinates
	double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	double v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	double w = 1 - u - v;

	// Check if point is in triangle
	if (u >= EMP_MINUS_TINY && v >= EMP_MINUS_TINY && w >= EMP_MINUS_TINY) {
		// Somewhere in the triangle
		if (u <= EMP_TINY && v <= EMP_TINY) {
			*code = 0; // vertex a
		}
		else if (u <= EMP_TINY && w <= EMP_TINY) {
			*code = 1; // vertex b
		}
		else if (v <= EMP_TINY && w <= EMP_TINY) {
			*code = 2; // vertex c
		}
		else if (u <= EMP_TINY) {
			*code = 3; // edge AB
		}
		else if (w <= EMP_TINY) {
			*code = 4; // edge BC
		}
		else if (v <= EMP_TINY) {
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




bool Triangle::isEqual(Triangle * t)
{
  for (int i = 0; i < 3; i++) {
    if (!vertices[i]->isEqual(t->getVertex(i)))
      return false;    
  }
  return true;
}
