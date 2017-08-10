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

class Point2D {

private:
	double x; //!< X component of the point
	double y; //!< Y component of the point

public:

	//! Creates a new Point2D object.
	/*!
	Assigns x and y values to the x and y members respectively

	@author German Molina
	@param[in] x the x component of the point
	@param[in] y the y component of the point
	*/
	Point2D(double x, double y);

	//! Clones a Point2D object.
	/*!
	Assigns x and y values from the input point

	@author German Molina
	@param[in] point the point to clone
	*/
	Point2D(Point2D * point);

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

	
	//! Calculates the square of the distance to another point
	/*!
	This is faster than calculating the actual distance, and end in the
	same results when what is intended is to calculate relative distance
	(i.e. calculate the closest point to another point).

	@author German Molina
	@param[in] point the point to calculate the distance to
	@return the square of the distance
	*/
	double squaredDistanceTo(Point2D * point);

	//! Calculates the distance to another point
	/*!
	@author German Molina
	@param[in] point the point to calculate the distance to
	@return the distance
	*/
	double distanceTo(Point2D * point);
};