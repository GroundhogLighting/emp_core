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

#include <string>
#include <vector>

#include "common/geometry/polygon.h"



// Represents a Polygon3D where some lighting metrics will be calculated
/*!
This class correspond to Workplanes, which are surfaces that do not represent
real geometry, but areas where we want to calculate lighting metrics.

They are transformed in Triangulation objects before being exported or used, and 
are written or used as sensor grids 

*/

class Workplane {
private:
	std::string name; //!< Name of the workplane
	std::vector <Polygon3D * > polygons; //!< The polygons in the workplane.

public:

	//! Creates an empty Workplane object
	/*!
	@author German Molina
	@param[in] workplaneName The name of the workplane (needs to be fixed)
	*/
	Workplane(std::string workplaneName);

	//! Destroys a workplane
	/*!
	@author German Molina	
	*/
	~Workplane();

	//! Adds a new polygon to the Workplane
	/*!
	@author German Molina
	@param[in] polygon The polygon to add
	*/
	void addPolygon(Polygon3D * polygon);
	
	//! Retrieves the name
	/*!
	@author German Molina
	@return the name;
	*/
	std::string getName();

	//! Checks if the workplane has a certain name
	/*!
	@author German Molina
	@param[in] name The name to check
	@return Is the same name;
	*/
	bool compareName(std::string * name);

	//! Retrieves the number of Polygon3D in the Workplane
	/*!
	@author German Molina
	@return The number of Workplane
	*/
	size_t getNumPolygons();

	//! Retrieves the reference to a Polygon3D in polygons
	/*!
	@author German Molina
	@param i The index of the Polygon3D to extract
	@return The reference
	*/
	Polygon3D * getPolygonRef(size_t i);
};