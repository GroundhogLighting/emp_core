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

#include "./polygon.h"

#include "../../config_constants.h"

//! Simple structure containing the minimum and maximum X, Y and Z coordinates of something.

/*!
	They are always orthogonal with the origin.

	Intended to be created, used and deleted.
*/

class BBox {
public:
	double min_x = HUGE; //!< Minimum available X value
	double max_x = MINUS_HUGE; //!< Maximum available X value
	double min_y = HUGE; //!< Minimum available Y value
	double max_y = MINUS_HUGE; //!< Maximum available Y value
	double min_z = HUGE; //!< Minimum available Z value
	double max_z = MINUS_HUGE; //!< Maximum available Z value
	double c_x = 0; //!< X component of the center of the box
	double c_y = 0; //!< Y component of the center of the box
	double c_z = 0; //!< Z component of the center of the box


	//! Creates the BBox of a polygon
	/*!
	@author German Molina
	@param[in] polygon The Polygon3D to calculate the BBOX o
	*/
	BBox(Polygon3D * polygon);
};