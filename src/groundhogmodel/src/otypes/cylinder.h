
/*****************************************************************************
 Emp
 
 Copyright (C) 2018  German Molina (germolinal@gmail.com)
 
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

//! Cylinder object
/*!
 This object is extracted directly from the Radiance reference...
 read that document to learn more
 
 
 */
class Cylinder : public Otype {
public:
    Point3D p0 = Point3D(0,0,0); //!< The center of one end of the cylinder
    Point3D p1 = Point3D(0,0,1); //!< The center of the other end of the cylinder
    double radius = 1; //!< The radius in one end of the cylinder
    
    //! Creates a Cylinder object
    /*!
     Assigns the name of the face and sets an empty Polygon3D
     
     @author German Molina
     @param[in] faceName The name of the face
     */
    Cylinder(std::string * objectName)
    {
        setName( objectName );
        setType("cylinder");        
    }
    
    
    
    
};


