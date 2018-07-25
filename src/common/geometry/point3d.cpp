/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

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

#include "../utilities/io.h"
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
	return (std::abs(x - p.x) < EMP_TINY &&  std::abs(y - p.y) < EMP_TINY && std::abs(z - p.z) < EMP_TINY);
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

  
#ifdef _DEBUG
	// Check if they are normal... debugging
	if (std::abs(1 - i.getLength()) > EMP_TINY || std::abs(1 - j.getLength()) > EMP_TINY || std::abs(1 - k.getLength()) > EMP_TINY) {
		warn("Trying to transform a point with non normalized vectors");
		std::cerr << "i length: " << i.getLength() << " -- j length: " << j.getLength() << " -- k length: " << k.getLength() << std::endl;
	}

	if (std::abs(i*j) > EMP_TINY || std::abs(j*k) > EMP_TINY || std::abs(i*k) > EMP_TINY) {
		warn("Transforming a point with non orthogonal vectors");
		std::cerr << "   i: ";
		i.print(); 
		std::cerr << "   j: ";
		j.print();
		std::cerr << "   k: ";
		k.print();

        std::cerr << "|i| : " << i.getLength() <<std::endl;
        std::cerr << "|j| : " << j.getLength() <<std::endl;
        std::cerr << "|k| : " << k.getLength() <<std::endl;
        
		std::cerr << "   i*j : " << std::abs(i*j) << std::endl;
		std::cerr << "   j*k : " << std::abs(j*k) << std::endl;
		std::cerr << "   i*k : " << std::abs(i*k) << std::endl;
	}
#endif
  
	
	double rx = i.getX()*x + i.getY()*y + i.getZ()*z;
	double ry = j.getX()*x + j.getY()*y + j.getZ()*z;
	double rz = k.getX()*x + k.getY()*y + k.getZ()*z;

	return Point3D(rx, ry, rz);
}


Point3D Point3D::transform(Transform * t)
{
  
  Matrix4x4 * m = t->getMatrix();
  double xv = x*m->getElement(0, 0) + y*m->getElement(0,1) + z*m->getElement(0,2) + m->getElement(0,3);
  double yv = x*m->getElement(1, 0) + y*m->getElement(1, 1) + z*m->getElement(1, 2) + m->getElement(1, 3);
  double zv = x*m->getElement(2, 0) + y*m->getElement(2, 1) + z*m->getElement(2, 2) + m->getElement(2, 3);

  return Point3D(xv, yv, zv);
}
