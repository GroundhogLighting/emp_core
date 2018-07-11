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


#pragma once

#include <string>
#include "../../common/geometry/transform.h"

#include "./material.h"

//! Class for representing various types of geometrical objects

/*!
This is the base class for several objects (i.e. Face and potentially more)
that represent geometry. 

The name Otype was copied from Radiance, since the name "Object" was too generic
*/

class Otype {
private:
	std::string name; //!< The unique name of the object
	std::string type; //!< The type of object	
	Material * material = NULL; //!< The material of the Face
	

public:
	//! Constructor of a new Otype
	/*!
	@author German Molina
	*/
	Otype();

	//! Destroys an Otype
	/*!
	@author German Molina
	*/
	virtual ~Otype();

	//! Retrieves the name of the Otype
	/*!
	@author German Molina
	@return The name of the Otype
	*/
	std::string getName() const;

	//! Sets the name of the Otype
	/*!
	@author German Molina
	@param[in] newName The name of the Otype
	*/
	void setName(const std::string * const newName);


	//! Retrieves the type of the Otype
	/*!
	@author German Molina
	@return The type of the Otype
	*/
    std::string getType() const;

    //! Sets the type of the Otype
    /*!
     @author German Molina
     @return The type of the Otype
     */
    void setType(const std::string type);

	//! Compares a name with another string
	/*!
	@author German Molina
	@param[in] otherName The pointer to another string
	@return is equal
	*/
	bool compareName(const std::string * const otherName) const;
	

	//! Retrieves the Material assigned to the Otype
	/*!
	@author German Molina
	@return The pointer to the Material
	*/
	const Material * const getMaterial() const;

	//! Assigns a Material to the Otype
	/*!
	@author German Molina
	@param[in] mat The pointer to the Material
	*/
    void setMaterial(Material * const mat);

    
	
    
};
