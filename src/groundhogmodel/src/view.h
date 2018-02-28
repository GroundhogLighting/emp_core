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

#include "../../common/geometry/point3d.h"
#include "../../common/geometry/vector.h"

#define PERSPECTIVE_VIEW 0 // -vtv
#define PARALLEL_VIEW 1 // -vtl
#define CYLINDRICAL_VIEW 2 // -vtc
#define HEMISPHERICAL_VIEW 3 // -vth
#define ANGULAR_VIEW 4 // -vta
#define STEREOGRAPHIC_VIEW 5 // -vts

//! Represents a view point, view direction, field of view and type of view.

class View {

public:
	std::string name; //!< The name of the view
	Point3D viewPoint = Point3D(0,0,0); //!< The view point position
	Vector3D viewDirection = Vector3D(0,0,1); //!< The view direction vector
	Vector3D viewUp = Vector3D(0,0,1); //!< The view up vector
	double viewHorizontal = 45; //!< The field of view in the horizontal direction
	double viewVertical = 45; //!< The field of view in the vertical direction
	int viewType = PERSPECTIVE_VIEW; //!<  The view type according to defined macros
	double foreClippingDistance = 0; //!< view fore clipping plane
	double aftClippingDistance = 0; //!< view fore clipping plane

	//! Creates a new View object
	/*!
	All members are empty.
	@author German Molina
	*/
	View();

	//! Destroys a View object
	/*!
	Deletes all the members

	@author German Molina
	*/
	~View();

	//! Assigns a name to the view
	/*!
	@author German Molina
	@param[in] name The name of the view
	*/
	void setName(std::string * name);

	//! Retrieves the name of the view
	/*!
	@author German Molina
	@return The name of the view
	*/
	std::string * getName();

	//! Assigns a View Point to the view
	/*!
	@author German Molina
	@param[in] point The View Point of the view
	*/
	void setViewPoint(Point3D point);

	//! Retrieves the View Point of the view
	/*!
	@author German Molina
	@return The View Point of the view
	*/
	Point3D getViewPoint();
	
	//! Assigns a View Up to the view
	/*!
	@author German Molina
	@param[in] vector The View Up of the view
	*/
	void setViewUp(Vector3D vector);
	
	//! Retrieves the View Up of the view
	/*!
	@author German Molina
	@return The View Up of the view
	*/
	Vector3D getViewUp();
	
	//! Assigns a View Direction to the view
	/*!
	@author German Molina
	@param[in] vector The View Direction of the view
	*/
	void setViewDirection(Vector3D vector);
	
	//! Retrieves the View Direction of the view
	/*!
	@author German Molina
	@return The View Direction of the view
	*/
	Vector3D getViewDirection();
	
	//! Assigns the View Horizontal value of the view
	/*!
	@author German Molina
	@param[in] vH The View Horizontal of the view
	*/
	void setViewHorizontal(double vH);

	//! Retrieves the View Horizontal value of the view
	/*!
	@author German Molina
	@return The View Horizontal value of the view
	*/
	double getViewHorizontal();

	//! Assigns the View Vertical value of the view
	/*!
	@author German Molina
	@param[in] vV The View Vertical of the view
	*/
	void setViewVertical(double vV);

	//! Retrieves the View Vertical value of the view
	/*!
	@author German Molina
	@return The View Vertical value of the view
	*/
	double getViewVertical();

	//! Assigns the View Type of the view
	/*!
	@author German Molina
	@param[in] vT The View Type of the view
	*/
	void setViewType(int vT);
	
	//! Retrieves the View Type of the view
	/*!
	@author German Molina
	@return The View Type of the view
	*/
	int getViewType();

};
