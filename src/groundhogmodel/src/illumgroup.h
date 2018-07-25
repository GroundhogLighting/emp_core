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

#include <vector>

#include "../../common/geometry/polygon.h"

//! Represents groups of illums, that may contain one or more objects
/*!
 Illums work as proxies for light sources, helping some rendering processes
*/

class IllumGroup {
private:
	std::string name; //!< The name of the group
	std::vector <Polygon3D *> polygons; //!< Contains the faces in the window group

public:

	//! Creates a new IllumGroup
	/*!
	@author German Molina
	@param[in] name The name of the window group
	*/
	IllumGroup(std::string name);

	//! Destroys a IllumGroup
	/*!
	@author German Molina
	*/
	~IllumGroup();

	//! Adds a Polygon to the illum group
	/*!
	@author German Molina
	@param[in] polygon The Polygon to add
	*/
	void addPolygon(Polygon3D * polygon);

	//! Retrieves the name
	/*!
	@author German Molina
	*/
	std::string getName();

	//! Checks if the group has a certain name
	/*!
	@author German Molina
	@param[in] name The name to check
	@return Is the same name;
	*/
	bool compareName(std::string * name);

	//! Retrieves the number of winows in the group
	/*!
	@author German Molina
	@return The number of windows
	*/
	size_t size();

	//! Retrieves the reference to a certain polygon in the group
	/*!
	@author German Molina
	@param[in] i The index of the Polygon to retrieve
	@return The reference
	*/
	Polygon3D * getPolygonRef(size_t i);
};
