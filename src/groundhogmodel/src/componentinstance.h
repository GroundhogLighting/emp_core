#pragma once

#include "./componentinstance.h"
#include "./componentdefinition.h"


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
	@param[in] definitionRef The ComponentDefinition to which this ComponentInstance points
	*/
	ComponentInstance(ComponentDefinition * definitionRef);

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
	double getX();

	//! Retrieves the Y location
	/*!
	@author German Molina
	@return The Y location
	*/
	double getY();

	//! Retrieves the Z location
	/*!
	@author German Molina
	@return The Z location
	*/
	double getZ();

	//! Retrieves the X rotation
	/*!
	@author German Molina
	@return The X location
	*/
	double getRotationX();

	//! Retrieves the Y rotation
	/*!
	@author German Molina
	@return The Y location
	*/
	double getRotationY();

	//! Retrieves the Z rotation
	/*!
	@author German Molina
	@return The Z location
	*/
	double getRotationZ();

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
	double getScale();

	//! Retrieves the ComponentDefinition reference of the instance
	/*!
	@author German Molina
	@return The ComponentDefinition reference
	*/
	ComponentDefinition * getDefinitionRef();

};