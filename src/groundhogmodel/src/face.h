#pragma once

#include <string>
#include "./face.h"
#include "../../common/geometry/polygon.h"

//! The integration of a Polygon3D and a Material

/*!
A Face represents physical objects of the model.
*/

class Face {
private:
	std::string name; //!< The name of the face
	//Material
	Polygon3D * polygon; //!< The polygon that represents the geometry of the face

public:

	//! Creates a Face object
	/*!
	Assigns the name of the face and sets an empty Polygon3D

	@author German Molina
	@param[in] faceName The name of the face
	*/
	Face(std::string faceName);

	//! Destroys the Face
	/*!
	Also destroys the Polygon

	@author German Molina
	*/
	~Face();

	//! Retrieves the name of the Face
	/*!
	@author German Molina
	@return The name of the face
	*/
	std::string getName();

	//! Sets the Polygon3D of the Face
	/*!
	Deletes the current polygon, and adds the new one.

	@author German Molina
	@param[in] newPolygon
	*/
	void setPolygon(Polygon3D * newPolygon);

	//! Checks if the Face has inner Loop (or holes)
	/*!
	@author German Molina
	@return has inner Loop ?
	*/
	bool hasInnerLoops();

	//! Checks if the Face has too many inner Loop (or holes)
	/*!
	@author German Molina
	@return has too many inner Loop ?
	*/
	bool hasTooManyInnerLoops();

	//! Retrieves the reference to the Outer Loop of the Face
	/*!
	@author German Molina
	@return The reference to the Outer Loop
	*/
	Loop * getOuterLoopRef();

	//! Retrieves the reference to a closed Loop of the Face Polygon3D
	/*!
	@author German Molina
	@return The reference to the new closed Loop
	@see Polygon3D::getClosedLoop()
	@note Should delete the closed Loop after its use
	*/
	Loop * getClosedLoop();
};