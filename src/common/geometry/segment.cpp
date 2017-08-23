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

#include <string>
#include "../utilities/io.h"

#include "../../config_constants.h"
#include "./segment.h"

Segment::Segment(Point3D * a, Point3D * b) 
{
	start = a;
	end = b;
}

bool Segment::isEqual(Segment * anotherSegment) 
{
	if (start->isEqual(anotherSegment->start) && end->isEqual(anotherSegment->end)) {
		return true;
	}
	else if (end->isEqual(anotherSegment->start) && start->isEqual(anotherSegment->end)) {
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
	Vector3D a = *end - start;
	Vector3D b = *input->end - input->start;
	Vector3D normal = a%b;

	// check if coplanar
	Vector3D delta = *start - input->start;
	if ((delta % normal).isZero())
		return false;

	double tA;
	double tB;
	double det;

	// Check for intersection.
	if (std::abs(normal.getZ()) > GLARE_TINY) {
		det = a.getY()*b.getX() - a.getX()*b.getY();
		tA = (b.getY()*delta.getX() - b.getX()*delta.getY()) / det;
		tB = (a.getY()*delta.getX() - a.getX()*delta.getY()) / det;
	} 
	else if (std::abs(normal.getX()) > GLARE_TINY) {
		det = a.getY()*b.getZ() - a.getZ()*b.getY();
		tA = (b.getY()*delta.getZ() - b.getZ()*delta.getY()) / det;
		tB = (a.getY()*delta.getZ() - a.getZ()*delta.getY()) / det;
	}
	else if (std::abs(normal.getY()) > GLARE_TINY) {
		det = a.getX()*b.getZ() - a.getZ()*b.getX();
		tA = (b.getX()*delta.getZ() - b.getZ()*delta.getX()) / det;
		tB = (a.getX()*delta.getZ() - a.getZ()*delta.getX()) / det;
	}

	if (tA >= 0 && tA <= 1 && tB >= 0 && tB <= 1) {
		if (output != NULL) {
			*output = Point3D(start->getX() + tA * a.getX() , start->getY() + tA * a.getY(), start->getZ() + tA * a.getZ());
		}
		return true;
	}
	return false;
}


Point3D Segment::midpoint() {
	double dX = start->getX() + end->getX();
	double dY = start->getY() + end->getY();
	double dZ = start->getZ() + end->getZ();
	return Point3D(dX/2,dY/2,dZ/2);
}