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
#include "../../3rdparty/json/json.hpp"

using nlohmann::json;

//! Class that represent various materials

/*!
	Since Glare is intended to obtaing numerical and physically correct output, 
	materials are very important. Wrong material definition would lead to incorrect
	output.

	Glare materials must be physically correct and not only "look good". Most (if not all) 
	of them will be derived from Radiance materials.
*/

class Material {
protected:
	std::string name; //!< The unique name of the material
	std::string type; //!< The type of material
	int color[3] = {153, 153, 153};
	double alpha = 1;


public:
	//! Constructor of a new Material
	/*!
	@author German Molina
	@param[in] name The name of the new Material
	*/
	Material();

	//! Destroys a material
	/*!
	@author German Molina
	*/
	~Material();

	//! Retrieves the name of the Material
	/*!
	@author German Molina
	@return The name of the material
	*/
	std::string getName();

	//! Retrieves the type of the Material
	/*!
	@author German Molina
	@return The type of the material
	*/
	std::string getType();

	//! Compares a name with another string
	/*!
	@author German Molina
	@param[in] otherName The pointer to another string
	@return is equal
	*/
	bool compareName(std::string * otherName);

	//! Retrieves basic data from a JSON and fills the Material info
	/*!
	@author German Molina
	@param[in] j The JSON that represents the Material
	*/
	void getBasicData(json j);

	//! Writes a Material in Radiance format
	/*!
	@author German Molina
	@param[in] dir The directory where the material should be written
	@return success
	@note This function should be overriden by a function with the same name in each Material derived class
	*/
	virtual bool writeRadianceDefinition(std::string * dir);
};