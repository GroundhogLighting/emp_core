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

#include "./componentdefinition.h"
#include "../../common/geometry/transform.h"


class ComponentDefinition;

//! A reference to a ComponentDefinition, including a location, position and scale

/*!
ComponentInstance are used for including ComponentDefinition within Layer or ComponentDefinition.
*/

class ComponentInstance {


private:
	ComponentDefinition * definition; //!< The pointer to the ComponentDefinition

	double rotationX; //!< The rotation on the X axis of the instance
	double rotationY; //!< The rotation on the Y axis of the instance
	double rotationZ; //!< The rotation on the Z axis of the instance

	double scale; //!< The scale of the instance

	double x; //!< The X component of the position 
	double y; //!< The Y component of the position 
	double z; //!< The Z component of the position 

public:

	//! Creates a new ComponentInstance object
	/*!
	Defaults to a non-rotated non-translated and non-scaled instance.

	@author German Molina
	@param[in] definitionPt The pointer to the ComponentDefinition to which this ComponentInstance points
	*/
	ComponentInstance(ComponentDefinition * const definitionPt);

	//! Destroys a ComponentInstance object
	/*!
	@author German Molina
	*/
	~ComponentInstance();

	//! Sets the X location
	/*!	
	@author German Molina
	@param[in] x The X location
	*/
	void setX(double x);

	//! Sets the Y location
	/*!
	@author German Molina
	@param[in] y The Y location
	*/
	void setY(double y);

	//! Sets the Z location
	/*!
	@author German Molina
	@param[in] z The Z location
	*/
	void setZ(double z);

	//! Sets the rotation on the X axis
	/*!
	@author German Molina
	@param[in] rx The rotation on X axis
	*/
	void setRotationX(double rx);

	//! Sets the rotation on the Y axis
	/*!
	@author German Molina
	@param[in] ry The rotation on Y axis
	*/
	void setRotationY(double ry);

	//! Sets the rotation on the Z axis
	/*!
	@author German Molina
	@param[in] rz The rotation on Z axis
	*/
	void setRotationZ(double rz);

	//! Retrieves the X location
	/*!
	@author German Molina
	@return The X location
	*/
	double getX() const;

	//! Retrieves the Y location
	/*!
	@author German Molina
	@return The Y location
	*/
	double getY() const;

	//! Retrieves the Z location
	/*!
	@author German Molina
	@return The Z location
	*/
	double getZ() const;

	//! Retrieves the X rotation
	/*!
	@author German Molina
	@return The X location
	*/
	double getRotationX() const;

	//! Retrieves the Y rotation
	/*!
	@author German Molina
	@return The Y location
	*/
	double getRotationY() const;

	//! Retrieves the Z rotation
	/*!
	@author German Molina
	@return The Z location
	*/
	double getRotationZ() const;

	//! Sets the scale of the instance
	/*!
	@author German Molina
	@param[in] s The scale
	*/
	void setScale(double s);

	//! Retrieves the scale of the instance
	/*!
	@author German Molina
	@return The scale
	*/
	double getScale() const;

	//! Retrieves the ComponentDefinition reference of the instance
	/*!
	@author German Molina
	@return The ComponentDefinition reference
	*/
	const ComponentDefinition * const getDefinitionRef() const;

    //! Retrieves a Transform object corresponding to this instance
    /*!
    @author German Molina
    @return The Transform
    */
    Transform * getTransform() const;

};
