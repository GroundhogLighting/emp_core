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

#include "../src/face.h"
#include "../src/componentinstance.h"

//! A Layer is the main group of geometry in a GroundhogModel. 

/*!
Every ComponentInstance or Face that is not in another ComponentDefinition will be 
in a Layer
*/
class Layer{

private:
	std::string name; //!< Name of the Layer
	std::vector <Face * > faces; //!< The faces in the Layer
	std::vector <ComponentInstance * > instances; //!< The ComponentInstance in the Layer

public:

	//! Creates a Layer object
	/*!
	Assigns the name

	@author German Molina
	@param[in] layerName The name of the Layer
	*/
	Layer(std::string layerName);

	//! Destroys a Layer object
	/*!
	Deletes every Face in the Layer

	@author German Molina
	*/
	~Layer();

	//! Checks if the Layer has a certain name
	/*!	
	@author German Molina
	@param[in] layerName The name to check
	@return is the Layer name equivalent to layerName ?
	*/
	bool compareName(std::string * layerName);

	//! Adds a Face to the Layer
	/*!
	@author German Molina
	@param[in] face The Face to add to the Layer
	*/
	void addFace(Face * face);

	//! Retrieves the reference to the ComponentInstance vector
	/*!
	@author German Molina
	@return The reference to the ComponentInstance vector
	*/
	std::vector <ComponentInstance * > * getComponentInstancesRef();

	//! Retrieves the name of the Layer
	/*!
	@author German Molina
	@return The name of the Layer
	*/
	std::string getName();

	//! Retrieves the reference to the Face vector
	/*!
	@author German Molina
	@return The reference to the Face vector
	*/
	std::vector <Face * > * getFacesRef();

	//! Retrieve the reference to a specific Face
	/*!
	@author German molina
	@param[in] i The index of the Face to retrieve
	*/
	Face * getFaceRef(size_t i);

	//! Retrieve the reference to a specific ComponentInstance
	/*!
	@author German molina
	@param[in] i The index of the ComponentInstance to retrieve
	*/
	ComponentInstance * getComponentInstanceRef(size_t i);

	//! Checks if the Layer is empty
	/*!
	Face is empty if there are not ComponentInstance and no Face

	@author German Molina
	@return is empty ?
	*/
	bool isEmpty();
};