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

#include "./otypes/face.h"

//! Represents groups of windows, that may contain one or more objects
/*!
Sometimes the user wants to group Windows, which is useful for some simulation
methods.

In any case, every window in the model is put in a Window Group... some of them
will be made of 1 window (i.e. ungrouped windows) and some of several.
*/

class WindowGroup {
private:
	std::string name; //!< The name of the group
	std::vector <Face *> windows; //!< Contains the faces in the window group

public:

	//! Creates a new WindowGroup
	/*!
	@author German Molina
	@param[in] name The name of the window group
	*/
	WindowGroup(std::string name);

	//! Destroys a WindowGroup
	/*!
	@author German Molina
	*/
	~WindowGroup();

	//! Adds a Face to the window group
	/*!
	@author German Molina
	@param[in] face The face to add
	*/
	void addFace(Face * face);

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

	//! Retrieves the reference to a certain Face in windows
	/*!
	@author German Molina
	@param[in] i The index of the Face to retrieve
	@return The reference
	*/
	Face * getWindowRef(size_t i);
};
