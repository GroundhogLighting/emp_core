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
#include <vector>

#include "../../common/geometry/polygon.h"



// Represents a Polygon3D where some lighting metrics will be calculated
/*!
This class correspond to Workplanes, which are surfaces that do not represent
real geometry, but areas where we want to calculate lighting metrics.

They are transformed in Triangulation objects before being exported or used, and 
are written or used as sensor grids 

*/

class Workplane {
private:
	std::string name; //!< Name of the workplane
	std::vector <Polygon3D * > polygons; //!< The polygons in the workplane.
    double maxArea = 0.25; //!< The desired 'pixel' resolution when triangulating
    double maxAspectRatio = 1.3; //!< The desired maximum aspect ratio
    std::vector <std::string> tasks = std::vector<std::string>(0); //!< The tasks assigned to the workplane
    
public:

	//! Creates an empty Workplane object
	/*!
	@author German Molina
	@param[in] workplaneName The name of the workplane (needs to be fixed)
	*/
	Workplane(std::string workplaneName);

	//! Destroys a workplane
	/*!
	@author German Molina	
	*/
	~Workplane();

	//! Adds a new polygon to the Workplane
	/*!
	@author German Molina
	@param[in] polygon The polygon to add
	*/
	void addPolygon(Polygon3D * polygon);
	
	//! Retrieves the name
	/*!
	@author German Molina
	@return the name;
	*/
    std::string getName() const;

	//! Checks if the workplane has a certain name
	/*!
	@author German Molina
	@param[in] name The name to check
	@return Is the same name;
	*/
	bool compareName(const std::string * const name) const;

	//! Retrieves the number of Polygon3D in the Workplane
	/*!
	@author German Molina
	@return The number of Workplane
	*/
	const size_t getNumPolygons() const;

	//! Retrieves the reference to a Polygon3D in polygons
	/*!
	@author German Molina
	@param i The index of the Polygon3D to extract
	@return The reference
	*/
	Polygon3D * const getPolygonRef(const size_t i) const;

    //! Retrieves the maximum area for each pixel in the grid
    /*!
    @author German Molina
    @return the area
    */
    const double getMaxArea() const;
    
    //! Sets the maximum area for each pixel in the grid
    /*!
     @author German Molina
     @param v The value
     */
    void setMaxArea(const double v);
    
    //! Retrieves the maximum aspect ratio for each pixel in the grid
    /*!
     @author German Molina
     @return the area
     */
    const double getMaxAspectRatio() const;
    
    //! Sets the maximum aspect ratio for each pixel in the grid
    /*!
     @author German Molina
     @param v The value
     */
    void setMaxAspectRatio(const double v);
    
    //! Adds a task to the Workplane
    /*!
     @author German Molina
     @param taskName the name of the task
    */
    void addTask(const std::string taskName);
    
    //! Retrieves a certain Task Name
    /*!
     @author German Molina
     @return A pointer to the task name
     */
     const std::vector<std::string> * const getTasks() const;
};
