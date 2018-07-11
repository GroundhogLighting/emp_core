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


#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "json/json.hpp"

using nlohmann::json;

//! Class that represent various materials

/*!
	Since Emp is intended to obtaing numerical and physically correct output, 
	materials are very important. Wrong material definition would lead to incorrect
	output.

	Emp materials must be physically correct and not only "look good". Most (if not all) 
	of them will be derived from Radiance materials.
*/

class Material {


public:
    std::string name; //!< The unique name of the material
    std::string type; //!< The type of material        
    
	//! Constructor of a new Material
	/*!
	@author German Molina
	*/
	Material();

	//! Destroys a material
	/*!
	@author German Molina
	*/
	virtual ~Material();

	//! Retrieves the name of the Material
	/*!
	@author German Molina
	@return The name of the material
	*/
	std::string getName() const;
    
    //! Retrieves a value from a JSON, if it exists
    /*!
     @author German Molina
     @return the value... throws otherwise
    */
    double getFromJSON(const char * key, json * j);

	//! Sets the name of the Material
	/*!
	@author German Molina
	@param The name
	@return The name of the material
	*/
	void setName(const std::string * name);

	//! Retrieves the type of the Material
	/*!
	@author German Molina
	@return The type of the material
	*/
    std::string getType() const;

	//! Sets the type of the Material
	/*!
	@author German Molina
	@return The type of the material
	*/
	void setType(const std::string type);

	//! Compares a name with another string
	/*!
	@author German Molina
	@param[in] otherName The pointer to another string
	@return is equal
	*/
	bool compareName(const std::string * const otherName) const;
	
	//! Parses a tokenized Groundhog primitive
	/*!
	A Groundhog primitive may be a Radiance primitive or an extension
	of them

	@author German Molina
	@param[in] tokens The primitive tokenized
	@return success
	@note This function should be overriden by a function with the same name in each Material derived class	
	*/
	virtual bool parsePrimitive(std::vector <std::string> * tokens) = 0;
};

#endif
