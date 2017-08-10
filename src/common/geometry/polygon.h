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

#include <vector>

#include "./point3d.h"
#include "./vector.h"
#include "./loop.h"

//! A Polygon representation

/*!
Polygons are geometrical abstraction containing a single exterior Loop and 
(optionally) one or more interior loops (holes). 

The area may or may not 
be assigned. It is not used by ray-traing process.
*/

class Polygon3D {

private:
	Loop * outerLoop; //!< The outer loop
	std::vector < Loop * > innerLoops; //!< A vector with all the interior loops
	double area; //!< The are of the polygon (assumed to be in m2)
	Vector3D * normal;

public:

	//! Creates a new Polygon object
	/*!
	Assigns an area of -1 (which makes no sense, so it makes clear that it
	has not been calculated), an empty exterior loop and an empty array 
	of interior loops.

	@author German Molina
	*/
	Polygon3D();


	//! Destroys a Polygon object
	/*!	
	@author German Molina
	*/
	~Polygon3D();


	//! Retrieves the area of the Polygon
	/*!
	Returns the area. If such area was never calculated, it will
	return -1 (the default value assigned when creating the polygon)

	@author German Molina
	@return the area of the polygon
	*/
	double getArea();

	//! Sets the area of the Polygon
	/*!	
	@author German Molina
	@param[in] area the area of the polygon
	*/
	void setArea(double area);

	//! Retrieves the outer loop reference
	/*!
	@author German Molina
	@return The outer loop reference
	*/
	Loop * getOuterLoopRef();

	//! Adds a new interior loop
	/*!
	@author German Molina
	@return The pointer to the added loop
	*/
	Loop * addInnerLoop();

	//! Returns TRUE or FALSE, indicating if the polygon has interior loops
	/*!
	@author German Molina
	@return has inner loops?
	*/
	bool hasInnerLoops();


	//! Retrieves the number of interior loops
	/*!
	@author German Molina
	@return The number of interior loops
	*/
	size_t countInnerLoops();

	//! Returns a pointer to a new loop that represents the same area and geometry, but connects all the interior loops to the exterior loop (no holes)
	/*!
	This method connects all interior loops to the exterior one, transforming a polygon 
	with holes into one without holes.

	@author German Molina
	@note this loop needs to be deleted after its use
	@return The reference to the closed loop
	*/
	Loop * getClosedLoop();

	//! Cleans all the loops.
	/*!
	See Loop::clean()

	@author German Molina
	@return success
	*/
	bool clean();

	//! Retrieves the referente to a certain interior loop
	/*!
	@author German Molina
	@return The pointer
	*/
	Loop * getInnerLoopRef(size_t i);

	//! Sets the normal
	/*!
	@author German Molina	
	@param normal The normal of the Polygon3D
	*/
	void setNormal(Vector3D * normal);

	//! Retrieves the normal
	/*!
	@author German Molina
	@return the normal reference
	*/
	Vector3D * getNormal();

};