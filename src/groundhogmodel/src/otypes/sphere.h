
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

#include "../../../common/geometry/point3d.h"

//! Bubble object
/*!
 This object is extracted directly from the Radiance reference...
 read that document to learn more
 
 
 */
class Sphere : public Otype {
public:
    Point3D center = Point3D(0,0,0); //!< The center of the Sphere
    double radius = 1; //!< The radius of the Sphere
    
    //! Creates a Sphere object
    /*!
     Assigns the name of the face and sets an empty Polygon3D
     
     @author German Molina
     @param[in] faceName The name of the face
     */
    Sphere(std::string * objectName)
    {
        setName( objectName );
        setType("sphere");
    }
    
    
    
    
};
