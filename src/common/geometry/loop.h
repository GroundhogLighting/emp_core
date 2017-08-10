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

//! Sequence of Point3D

/*!
This geometrical abstraction represent a sequence of vertices (Point3D)
*/

class Loop {

private:	
	std::vector < Point3D * > vertices; //!< The vector containing the vertices
public:

	//! Creates a new Loop object.
	/*!
	Assigns an empty vector to vertices member
	*/
	Loop();
	

	//! Clones Loop object.
	/*!
	Assigns an empty vector to vertices member, and then clones all vertices
	of input loop into it.
	
	@author German Molina
	@param[in] loop the loop to be cloned
	*/
	Loop(Loop * loop); // clone implementation

	//! Destroys a Loop object.
	/*!
	Also destroys every vertex

	@author German Molina
	*/
	~Loop();

	//! Adds a new vertex to the loop.
	/*!
	@author German Molina
	@param[in] point The point to be added
	*/
	void addVertex(Point3D * point);

	//! Returns the number of vertices in the loop.
	/*!
	@author German Molina
	@return the number of vertices in the loop
	*/
	size_t size();

	//! Returns the reference to a certain vertex in the loop.
	/*!
	@author German Molina
	@param[in] i The index of the vertex to retrieve
	*/
	Point3D * getVertexRef(size_t i);

	//! Removes colinear points in a loop
	/*!
	Intends to reduce the number of vertices without changing the shape of the 
	polygon

	@author German Molina
	@todo Write it it!
	*/
	bool clean();
};