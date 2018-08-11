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


#pragma once

#include <string>

#include "../../common/geometry/point3d.h"
#include "../../common/geometry/vector.h"

//!  For calculating lighting metrics in a specific point
/*!
	This class represents a Photosensor; that is, a virutal 
	lux-meter (or illuminance meter) placed somewhere in the model.
*/
class Photosensor {
private:
	std::string name; //!< The name of the Photosensor
	Point3D position = Point3D(0, 0, 0); //!< The place to do the "measurement"
	Vector3D direction = Vector3D(0, 0, 1); //!< The direction on which the "measurement" is performed

public:

	//! Creates a new Photosensor with the given name
	/*!
	@author German Molina
	@param[in] n The name of the created Photosensor
	*/
	Photosensor(std::string n);

	//! Retrieves the name of the Photosensor
	/*!
	@author German Molina
	@return The name of the Photosensor
	*/
	std::string getName();

	//! Retrieves the position of the Photosensor
	/*!
	@author German Molina
	@return The position
	*/
	Point3D getPosition();

	//! Retrieves the direction of the Photosensor
	/*!
	@author German Molina
	@return The direction
	*/
	Vector3D getDirection();

	//! Sets the position of the Photosensor
	/*!
	@author German Molina
	@param[in] p The position to set
	*/
	void setPosition(Point3D p);

	//! Sets the direction of the Photosensor
	/*!
	@author German Molina
	@param[in] v The direction to set
	*/
	void setDirection(Vector3D v);

};
