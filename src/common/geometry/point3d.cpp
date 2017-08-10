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


double Point3D::squaredDistanceTo(Point3D * point) 
{
	double dx = (x - point->x)*(x - point->x);
	double dy = (y - point->y)*(y - point->y);
	double dz = (z - point->z)*(z - point->z);
	return dx + dy + dz;
}



double Point3D::distanceTo(Point3D * point) 
{	
	return sqrt(squaredDistanceTo(point));
}


Vector3D Point3D::operator-(Point3D * p)
{
	return Vector3D(x-p->getX(), y - p->getY(), z - p->getZ());
}