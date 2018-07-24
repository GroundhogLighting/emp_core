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

#include <vector>

#include "./point3d.h"
#include "./vector.h"
#include "./loop.h"

//! A Polygon representation

/*!
Polygons are geometrical abstraction containing a single exterior Loop and 
(optionally) one or more interior loops (holes). 

It should be noted that Emp will define the external Loop counter clock wise
(i.e. right hand rule), and the holes in the oposite direction.

The area may or may not 
be assigned. It is not used by ray-traing process.
*/

class Polygon3D {

private:
	Loop * outerLoop; //!< The outer loop
	std::vector < Loop * > innerLoops; //!< A vector with all the interior loops
	double area = -1; //!< The are of the polygon (assumed to be in m2)
	Vector3D normal = Vector3D(0,0,0); //!< The normal of the Polygon3D... has to be set manually

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
	size_t countInnerLoops() const;

	//! Returns a pointer to a new loop that represents the same area and geometry, but connects all the interior loops to the exterior loop (no holes)
	/*!
	This method connects all interior loops to the exterior one, transforming a polygon 
	with holes into one without holes.

	@author German Molina
	@note this loop needs to be deleted after its use.
	@return The reference to the closed loop
	*/
	Loop * getClosedLoop();

	//! Cleans all the Loop in a Polygon3D.
	/*!
	See Loop::clean()

	@author German Molina
	*/
	void clean();

	//! Retrieves the referente to a certain interior loop
	/*!
	@author German Molina
	@return The pointer
	*/
	Loop * getInnerLoopRef(size_t i) const;

	//! Sets the normal
	/*!
	@author German Molina	
	@param normal The normal of the Polygon3D
	*/
	void setNormal(Vector3D normal);

    //! Sets the normal by calculating the cross product between two segments
    /*!
     @author German Molina
     @todo This may not work when working with non convex polygons
     */
    void setNormal();
    
	//! Sets the normal by calculating the cross product between two segments
	/*!
	@author German Molina
     @param i The pivot
	@todo This may not work when working with non convex polygons
	*/
	void setNormal(size_t i);

	//! Retrieves the normal
	/*!
	@author German Molina
	@return the normal reference
	*/
	Vector3D getNormal();

	//! Tests if a point is inside or outside a polygon
	/*!
	@author German Molina
	@param[in] p The point to test
	@return true or false
	*/
	bool testPoint(Point3D p);

	//! Returns a 2D version of the Polygon3D
	/*!
		Most meshing algorithms and libraries are
		designed for 2D polygons. Accordingly, we
		need to transform our Polygon3D into other
		Polygon3D on which all the Z components of 
		its Point3D are equal (i.e. it now lies on 
		on the XY plane)

	@author German Molina
	@return the transformed polygon
	@note remember to delete the returned Polygon3D after using it.
	*/
	Polygon3D * get2DPolygon();

	//! Reverses what was done on the Polygon3D::get2DPolygon() function
	/*!

	@author German Molina
	@return the transformed polygon
	@param[in] normal The normal of the original Polygon3D
	@note remember to delete the returned Polygon3D after using it.
	*/
	Polygon3D * restore3DPolygon(Vector3D normal);

	//! Returns auxiliar axes for Polygon3D::get2DPolygon()
	/*!
	@author German Molina
	@param[in] normal The normal of the plane
	@param[out] i The auxiliar i axis
	@param[out] j The auxiliar j axis
	@param[out] k The auxiliar k axis
	@return success
	*/
	bool getAuxiliarAxes(Vector3D normal, Vector3D * i, Vector3D * j, Vector3D * k);

	//! Returns inverse auxiliar axes for Polygon3D::get2DPolygon()
	/*!
	@author German Molina
	@param[in] normal The normal of the plane
	@param[out] auxi The auxiliar i axis
	@param[out] auxj The auxiliar j axis
	@param[out] auxk The auxiliar k axis
	@return success
	*/
	bool getInverseAuxiliarAxes(Vector3D normal, Vector3D * auxi, Vector3D * auxj, Vector3D * auxk);
    
    //! Returns the total number of points that are not NULL in the polygon
    /*!
     @author German Molina
     @return the number of points
     */
    size_t countRealPoints() const;

};

extern Polygon3D polygon3D;

