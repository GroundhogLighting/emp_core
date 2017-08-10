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

#include <cmath>
#include "./vector.h"
#include "../../config_constants.h"
#include "../../common/utilities/io.h"

Vector3D::Vector3D(double new_x, double new_y, double new_z)
{
	x = new_x;
	y = new_y;
	z = new_z;
}

/*
Vector3D::~Vector3D()
{

}
*/

double Vector3D::getX()
{
	return x;
}

double Vector3D::getY()
{
	return y;
}

double Vector3D::getZ()
{
	return z;
}


double Vector3D::operator*(Vector3D * v)
{
	return x*v->getX() + y*v->getY() + z*v->getZ();
}

Vector3D Vector3D::operator%(Vector3D v)
{
	double dX = y*v.getZ() - z*v.getY();
	double dY = z*v.getX() - x*v.getZ();
	double dZ = x*v.getY() - y*v.getX();
	return Vector3D(dX, dY, dZ);
}

bool  Vector3D::isParallel(Vector3D * v)
{

	if (v->isZero() || isZero()){
		warn("Trying to compare Zero vectors in isParallel");
		return false;
	}

	// If they point in the same direction, K should be the same in all cases.
	double k;
	if(std::abs(v->getX()) > TINY)
		k = x / v->getX();

	// compare Y
	if (std::abs(v->getY()) < TINY) {
		if (std::abs(y) > TINY)
			return false;
	}
	else {
		if (std::abs(k - y/v->getY()) > TINY)
			return true;
	}
	
	// compare Z
	if (std::abs(v->getZ()) < TINY) {
		if (std::abs(z) > TINY)
			return false;
	}
	else {
		if (std::abs(k - z / v->getZ()) > TINY)
			return true;
	}

	return true;
}

bool Vector3D::sameDirection(Vector3D * v)
{
	if (!isParallel(v))
		return false;

	if (std::abs(v->getX()) > TINY) {
		return x / v->getX() > 0;
	}
	else if (std::abs(v->getY()) > TINY) {
		return y / v->getY() > 0;
	}
	else if (std::abs(v->getZ()) > TINY) {
		return z / v->getZ() > 0;
	}
	return false;

}

bool Vector3D::isZero()
{
	return (std::abs(x) < TINY &&  std::abs(y) < TINY && std::abs(z) < TINY);
}