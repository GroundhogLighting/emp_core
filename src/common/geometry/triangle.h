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

#include "./segment.h"
#include "fast-poly2tri/MPE_fastpoly2tri.h"

// Represents a Triangle.
/*!
This class is used mainly in the triangulation routines
*/

class Triangle {
private:
	Point3D * vertices[3]; //!< The vertices
	Segment * segments[3]; //!< The segments
	double circumradius = -1; //!< The radius of the circle that coes thorugh the three vertices
	double aspectRatio = -1; //!< The ratio of the Circumradius and the smallest edge
	Triangle * neighbors[3] = {nullptr,nullptr,nullptr}; //!< Neighboring triangles
	bool constraints[3] = { false,false,false }; //!< Which edges are constraints
	size_t index = 0; //!< The index on the Triangulation

public:
	//! Initializes a Triangle, adding segments and vertices
	/*!
	@author German Molina
	@param[in] a First vertex
	@param[in] b First vertex
	@param[in] c First vertex
	*/
	Triangle(Point3D * a, Point3D * b, Point3D * c);

	//! Destroys a Triangle object
	/*!
	@author German Molina
	*/
	~Triangle();

	//! Calculate the circumradius of the Triangle
	/*!
	Only calculates it if the value has not been 
	calculated already

	@author German Molina
	@return The circumradius
	*/
	double getCircumradius();

	//! Calculate the aspect ratio of the Triangle
	/*!
	Only calculates it if the value has not been
	calculated already

	@author German Molina
	@return The aspect ratio
	*/
	double getAspectRatio();

	//! Retrieves one vertex
	/*!
	@author German Molina
	@return The vertex pointer
	*/
	Point3D * getVertex(int i);

	//! Assigns a Triangle as neighbor
	/*!
	@author German Molina
	@param[in] t The triangle to set as neighbor
	@param[in] i The index of the neighbor
	@param[in] reciprocity Boolean that sais if we need to update the neighboring Triangle
	@return success
	*/
	bool setNeighbor(Triangle * t, int i, bool reciprocity); 

	//! Assigns an edge as constraint
	/*!
	@author German Molina
	@param[in] i The index of the constraint
	@return success
	*/
	bool setConstraint(int i);

	//! Checks if an edge is a constraint
	/*!
	@author German Molina
	@param[in] i The index of the constraint
	@return constraint
	*/
	bool isContraint(int i);


	//! Retrieves one neighbor Triangle
	/*!
	@author German Molina
	@param[in] i The index of the neighbor
	@return The neighbor
	*/
	Triangle * getNeighbor(int i);

	//! Retrieves a segment of a triangle
	/*!
	@author German Molina
	@param[in] i The index of the segment
	@return The segment pointer
	*/
	Segment * getSegment(int i);

	//! Retrieves the index of an edge based on points
	/*!
	Will return -1 if the the point is not there

	@author German Molina
	@param[in] a One point
	@param[in] b Another point
	@return The index of the point. 
	*/
	int getEdgeIndexByPoints(Point3D * a, Point3D * b);

	//! Sets the index
	/*!
	@author German Molina
	@param[in] i The index
	@return the index
	*/
	size_t setIndex(size_t i);

	//! Retrieves the index
	/*!
	@author German Molina
	@return the index
	*/
	size_t getIndex();

	//! Returns the center of a triangle (Point3D)
	/*!
	@author German Molina
	@return the center
	*/
	Point3D getCenter();

	//! Returns the circumcenter of a triangle (Point3D)
	/*!
	Obtained from: 
	https://gamedev.stackexchange.com/questions/60630/how-do-i-find-the-circumcenter-of-a-triangle-in-3d

	@author German Molina
	@return the circumcenter
	*/
	Point3D getCircumCenter();

	//! Returns the area of a triangle (Point3D)
	/*!
	Calculated using Solon's equation

	@author German Molina
	@return the area
	*/
	double getArea();

	//! Prints the triangle vertices on the screen
	/*!
	@author German Molina	
	*/
	void print();

	
	//! Tests if a Point3D is inside the Triangle
	/*!
	This function tests if a Triangle contains a certain Point3D, 
	considering the edges as "inside" the Triangle.

	The "Code" generated will vary according to the position of the
	Point3D in the Triangle. A code of -1 is returned when the Point3D
	is not in the Triangle; 0,1,2 when the Point3D is on vertices 0,1 
	or 2, respectively; 3, 4 and 5 mean that the Point3D is on edges
	0,1 and 2, respectively; and a code of 6 means that the Point3D
	is inside the Triangle.

	@author German Molina
	@param[in] p The point to test
	@param[out] code The position of the point in the triangle
	@return is in triangle
	*/
	bool testPoint(Point3D * p, int * code);

	
    //! Checks if a Emp triangle is equal to another
    /*!
    @author German Molina
    @param t The other triangle
    @return is equal?
    */
    bool isEqual(Triangle * t);
};

extern Triangle triangle;