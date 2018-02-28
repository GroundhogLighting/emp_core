
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
#include "../../../common/geometry/vector.h"

//! Ring object
/*!
 This object is extracted directly from the Radiance reference...
 read that document to learn more
 
 
 */
class Ring : public Otype {
public:
    Point3D center = Point3D(0,0,0); //!< The center of the ring
    Vector3D direction = Vector3D(0,0,1); //!< The direction where the ring is oriented
    double r0 = 0; //!< The inner radius
    double r1 = 1; //!< The outer radius
    
    //! Creates a Ring object
    /*!
     Assigns the name of the face and sets an empty Polygon3D
     
     @author German Molina
     @param[in] faceName The name of the face
     */
    Ring(std::string * objectName)
    {
        setName( objectName );
        setType("ring");
        
    }
    
    
    //! Writes the object in Radiance format
    /*!
     @author German Molina
     @param[in] file The file to write the face to
     @param[in] material The name of the Material to assign the face
     @param[in] transform The optional Transform object
     @return success
     */
    bool writeInRadianceFormat(FILE * file, const char * material, Transform * transform)
    {
        // get the name of the face
        std::string * objectName = getName();
        std::string * type = getType();
        fprintf(file, "%s %s %s\n0\n0\n8\n", material, type->c_str(), objectName->c_str());
        
        // Print arguments
        
        if (transform == nullptr) {
            fprintf(file, "%f %f %f %f %f %f %f %f\n", center.getX(), center.getY(), center.getZ(), direction.getX(), direction.getY(), direction.getZ(), r0, r1);
        }
        else {
            Point3D centerP = center.transform(transform);
            Vector3D directionP = direction.transform(transform);
            fprintf(file, "%f %f %f %f %f %f %f %f\n", centerP.getX(), centerP.getY(), centerP.getZ(), directionP.getX(), directionP.getY(), directionP.getZ(), r0, r1);
        }
        
        
        return true;
    }
    
};



