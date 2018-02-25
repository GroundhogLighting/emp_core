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
#include "./vector.h"
#include "../../config_constants.h"
#include "../utilities/io.h"
//#include "api/common.h"

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


double Vector3D::operator*(Vector3D v)
{
	return x*v.x + y*v.y + z*v.z;
}

Vector3D Vector3D::operator*(double s)
{
	return Vector3D(x*s,y*s,z*s);
}

Vector3D Vector3D::operator/(double s)
{
	return Vector3D(x / s, y / s, z / s);
}

Vector3D Vector3D::operator+(Vector3D v)
{
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator-(Vector3D v)
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator%(Vector3D v)
{
	double dX = y*v.z - z*v.y;
	double dY = z*v.x - x*v.z;
	double dZ = x*v.y - y*v.x;
	return Vector3D(dX, dY, dZ);
}

bool  Vector3D::isParallel(Vector3D v)
{

	if (v.isZero() || isZero()){
		warn("Trying to compare Zero vectors in isParallel");
		return false;
	}

	// If they point in the same direction, K should be the same in all cases.
	double k;
    if (std::abs(v.x) > EMP_TINY) {
		k = x / v.x;
    }
    else {
      if (std::abs(v.y) > EMP_TINY) {
        k = y / v.y;
      }
      else {
        if (std::abs(v.z) > EMP_TINY) {
          k = z / v.z;
        }
      }
    }

	// compare Y
	if (std::abs(v.y) < EMP_TINY) {
		if (std::abs(y) > EMP_TINY)
			return false;
	}
	else {
		if (std::abs(k - y/v.y) > EMP_TINY)
			return true;
	}
	
	// compare Z
	if (std::abs(v.z) < EMP_TINY) {
		if (std::abs(z) > EMP_TINY)
			return false;
	}
	else {
		if (std::abs(k - z / v.z) > EMP_TINY)
			return true;
	}

	return true;
}

bool Vector3D::sameDirection(Vector3D v)
{
	if (!isParallel(v))
		return false;

	if (std::abs(v.x) > EMP_TINY) {
		return x / v.x > 0;
	}
	else if (std::abs(v.y) > EMP_TINY) {
		return y / v.y > 0;
	}
	else if (std::abs(v.z) > EMP_TINY) {
		return z / v.z > 0;
	}
	return false;

}

bool Vector3D::isZero()
{
	return (std::abs(x) < EMP_TINY &&  std::abs(y) < EMP_TINY && std::abs(z) < EMP_TINY);
}

bool Vector3D::isEqual(Vector3D v)
{
	return (std::abs(x - v.x) < EMP_TINY &&  std::abs(y - v.y) < EMP_TINY && std::abs(z - v.z) < EMP_TINY);
}


double Vector3D::getLength()
{
	return sqrt(x*x+y*y+z*z);
}


double Vector3D::getSquaredLength()
{
	return (x*x + y*y + z*z);
}

void Vector3D::normalize()
{
	double l = getLength();
	if (l == 0) {
		FATAL(errorMessage,"Normalizing vector with Zero length");
		return;
	}

	x /= l; y /= l; z /= l;
}


void Vector3D::print()
{
	std::cerr << "Vector3D(";
	std::cerr << x << ",";
	std::cerr << y << ",";
	std::cerr << z << ")" << std::endl;
}
