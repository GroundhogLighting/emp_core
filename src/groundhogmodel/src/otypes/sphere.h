
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
        fprintf(file, "%s %s %s\n0\n0\n4\n", material, type->c_str(), objectName->c_str());
        
        // Print arguments
        
        if (transform == NULL) {
            fprintf(file, "%f %f %f %f\n", center.getX(), center.getY(), center.getZ(),radius);
        }
        else {
            Point3D p = center.transform(transform);
            fprintf(file, "%f %f %f %f\n", p.getX(), p.getY(), p.getZ(), radius);
        }
        
        return true;
    }
    
};
