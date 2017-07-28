#pragma once

#include <vector>
#include "./point.h"

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
};