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
#include<cmath>
#include <iostream>

#include "../../common/utilities/io.h"
#include "../../config_constants.h"
#include "./point3d.h"

Point3D::Point3D(double new_x, double new_y, double new_z) 
{
	x = new_x;
	y = new_y;
	z = new_z;
}


Point3D::Point3D(Point3D * point) 
{ // clone implementation
	x = point->getX();
	y = point->getY();
	z = point->getZ();
}

double Point3D::getX() 
{
	return x;
}

double Point3D::getY() 
{
	return y;
}

double Point3D::getZ() {
	return z;
}


double Point3D::squaredDistanceTo(Point3D point) 
{
	double dx = (x - point.x)*(x - point.x);
	double dy = (y - point.y)*(y - point.y);
	double dz = (z - point.z)*(z - point.z);
	return dx + dy + dz;
}



double Point3D::distanceTo(Point3D point) 
{	
	return sqrt(squaredDistanceTo(point));
}


Vector3D Point3D::operator-(Point3D p)
{
	return Vector3D(x-p.x, y - p.y, z - p.z);
}

Point3D Point3D::operator+(Vector3D v)
{
	return Point3D(x+v.getX(), y + v.getY(), z + v.getZ());
}

bool Point3D::isEqual(Point3D p)
{
	return (std::abs(x - p.x) < TINY &&  std::abs(y - p.y) < TINY && std::abs(z - p.z) < TINY);
}


Vector3D Point3D::position()
{
	return Vector3D(x, y, z);
}

void Point3D::print()
{
	std::cerr << "Point3D(";
	std::cerr << x << ",";
	std::cerr << y << ",";
	std::cerr << z << ")" << std::endl;
}

Point3D Point3D::transform(Vector3D i, Vector3D j, Vector3D k)
{

#ifdef DEBUG
	// Check if they are normal... debugging
	if (std::abs(1 - i.getLength()) > TINY || std::abs(1 - j.getLength()) > TINY || std::abs(1 - k.getLength()) > TINY) {
		warn("Trying to transform a point with non normalized vectors");
		std::cerr << "i length: " << i.getLength() << " -- j length: " << j.getLength() << " -- k length: " << k.getLength() << std::endl;
	}

	if (std::abs(i*j) > TINY || std::abs(j*k) > TINY || std::abs(i*k) > TINY) {
		warn("Transforming a point with non orthogonal vectors");
		std::cerr << "   i: ";
		i.print(); 
		std::cerr << "   j: ";
		j.print();
		std::cerr << "   k: ";
		k.print();

		std::cerr << "   i*j: " << std::abs(i*j) << std::endl;
		std::cerr << "   j*k: " << std::abs(j*k) << std::endl;
		std::cerr << "   i*k: " << std::abs(i*k) << std::endl;
	}
#endif
	
	double rx = i.getX()*x + i.getY()*y + i.getZ()*z;
	double ry = j.getX()*x + j.getY()*y + j.getZ()*z;
	double rz = k.getX()*x + k.getY()*y + k.getZ()*z;

	return Point3D(rx, ry, rz);
}