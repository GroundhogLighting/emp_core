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

#include "../../common/utilities/io.h"
#include "../../config_constants.h"
#include "./point3d.h"
#include "./triangle.h"

Segment::Segment(Point3D * a, Point3D * b) 
{
	start = a;
	end = b;
}

bool Segment::isEqual(Segment * anotherSegment) 
{
	if (start == anotherSegment->start && end == anotherSegment->end) {
		return true;
	}
	else if (end == anotherSegment->start && start == anotherSegment->end) {
		return true;
	}
	return false;
}

double Segment::getLength()
{	
	if (length < 0) { //has not been calculated
		length = start->distanceTo(end);
	}
	return length;
}

bool Segment::intersect(Segment * input, Point3D * output)
{
	warn("1");
	Vector3D a = *end - start;
	Vector3D b = *input->end - input->start;
	Vector3D normal = a%b;

	warn("2");
	// check if coplanar
	Vector3D delta = *start - input->start;
	if ((delta % normal).isZero())
		return false;

	warn("3");
	double tA;
	double tB;
	double det;
	// Check for intersection.
	if (std::abs(normal.getZ()) > TINY) {
		det = a.getY()*b.getX() - a.getX()*b.getY();
		warn("det Z" + std::to_string(det));
		tA = (b.getY()*delta.getX() - b.getX()*delta.getY()) / det;
		tB = (a.getY()*delta.getX() - a.getX()*delta.getY()) / det;
	} 
	else if (std::abs(normal.getX()) > TINY) {
		det = a.getY()*b.getZ() - a.getZ()*b.getY();
		warn("det X" + std::to_string(det));
		tA = (b.getY()*delta.getZ() - b.getZ()*delta.getY()) / det;
		tB = (a.getY()*delta.getZ() - a.getZ()*delta.getY()) / det;
	}
	else if (std::abs(normal.getY()) > TINY) {
		det = a.getX()*b.getZ() - a.getZ()*b.getX();
		warn("det Y" + std::to_string(det));
		tA = (b.getX()*delta.getZ() - b.getZ()*delta.getX()) / det;
		tB = (a.getX()*delta.getZ() - a.getZ()*delta.getX()) / det;
	}

	warn("4");
	if (tA >= 0 && tA <= 1 && tB >= 0 && tB <= 1) {
		*output = Point3D(start->getX() + tA * a.getX() , start->getY() + tA * a.getY(), start->getZ() + tA * a.getZ());
		return true;
	}
	return false;
}


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
		fatal("Impossible index when getting Vertex... index was '" + std::to_string(i) + "'");
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
	warn("ASPECT RATIO TO BE CALC");
	warn("Aspect ratio = " + std::to_string(aspectRatio));
	if (aspectRatio > 0)
		return aspectRatio;

	double minSegment = HUGE; 
	for (int i = 0; i < 3; i++) {
		if (segments[i]->getLength() < minSegment) {
			minSegment = segments[i]->getLength();
		}
	}
	warn("Minimum length of triangle is " + std::to_string(minSegment));
	warn("circumradius is " + std::to_string(getCircumradius()));
	warn("Aspect ratio is " + std::to_string(getCircumradius() / minSegment));

	aspectRatio = getCircumradius() / minSegment;
	return aspectRatio;
}

bool Triangle::setNeighbor(Triangle * t, int i,bool reciprocity) 
{
	if (i < 0 || i > 2) {
		fatal("Impossible index when Setting neighbor... index was '" + std::to_string(i) + "'");
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
		fatal("Impossible index when getting segment... index was '" + std::to_string(i) + "'");
		return NULL;
	}
	return segments[i];
}


Triangle * Triangle::getNeighbor(int i)
{
	if (i < 0 || i > 2) {
		fatal("Impossible index when getting neighbor... index was '" + std::to_string(i) + "'");
		return NULL;
	}
	return neighbors[i];
}


int Triangle::getEdgeIndexByPoints(Point3D * a, Point3D * b)
{	
	warn("Looking for points"); 
	warn("....." + std::to_string(a->getX()) + "," + std::to_string(a->getY()) + "," + std::to_string(a->getZ()));
	warn("....." + std::to_string(b->getX()) + "," + std::to_string(b->getY()) + "," + std::to_string(b->getZ()));
	for (int i = 0; i < 3; i++) {
		if (a == segments[i]->start && b == segments[i]->end) {
			return i;
		}
		if (b == segments[i]->start && a == segments[i]->end) {
			return i;
		}
	}
	fatal("Segment not found!");
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
		fatal("Impossible index when setting constraint... index was '" + std::to_string(i) + "'");
		return false;
	}
	constraints[i] = true;
}