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

#pragma once
#include "./vector.h"

//! A 3D point representation

/*!
This geometrical abstraction represent a point in space. Values are assumed to be in meters.
*/

class Point3D {

private:
	double x; //!< X component of the point
	double y; //!< Y component of the point
	double z; //!< Z component of the point

public:

	//! Creates a new Point3D object.
	/*!
	Assigns x, y and z values to the x, y and z members respectively

	@author German Molina
	@param[in] x the x component of the point
	@param[in] y the y component of the point
	@param[in] z the z component of the point
	*/
	Point3D(double x, double y, double z);

	//! Clones a Point3D object.
	/*!
	Assigns x, y and z values from the input point

	@author German Molina
	@param[in] point the point to clone
	*/
	Point3D(Point3D * point); 

	//~Point3D();

	//! Retrieves the X component of the point
	/*!	
	@author German Molina	
	@return the X component of the point
	*/
	double getX();

	//! Retrieves the Y component of the point
	/*!
	@author German Molina
	@return the Y component of the point
	*/
	double getY();

	//! Retrieves the Z component of the point
	/*!
	@author German Molina
	@return the Z component of the point
	*/
	double getZ();

	//! Calculates the square of the distance to another point
	/*!
	This is faster than calculating the actual distance, and end in the
	same results when what is intended is to calculate relative distance
	(i.e. calculate the closest point to another point).

	@author German Molina
	@param[in] point the point to calculate the distance to
	@return the square of the distance
	*/
	double squaredDistanceTo(Point3D * point);

	//! Calculates the distance to another point
	/*!
	@author German Molina
	@param[in] point the point to calculate the distance to
	@return the distance 
	*/
	double distanceTo(Point3D * point);

	//! Point sustraction, returns a Vector3D
	/*!
	@author German Molina
	@param[in] p The other poing
	@return The vector
	@note The resulting vector should be deleted when no longer needed
	*/
	Vector3D operator-(Point3D * p);
};