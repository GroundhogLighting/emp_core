
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

//! Tube object
/*!
 This object is extracted directly from the Radiance reference...
 read that document to learn more
 
 
 */
class Tube : public Otype {
public:
    Point3D p0 = Point3D(0,0,0); //!< The center of one end of the tube
    Point3D p1 = Point3D(0,0,1); //!< The center of the other end of the tube
    double radius = 1; //!< The radius in one end of the tube
    
    //! Creates a Tube object
    /*!
     Assigns the name of the face and sets an empty Polygon3D
     
     @author German Molina
     @param[in] faceName The name of the face
     */
    Tube(std::string * objectName)
    {
        setName( objectName );
        setType("tube");        
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
        fprintf(file, "%s %s %s\n0\n0\n7\n", material, type->c_str(), objectName->c_str());
        
        // Print arguments
        
        if (transform == nullptr) {
            fprintf(file, "%f %f %f %f %f %f %f\n", p0.getX(), p0.getY(), p0.getZ(), p1.getX(), p1.getY(), p1.getZ(), radius);
        }
        else {
            Point3D tp0 = p0.transform(transform);
            Point3D tp1 = p1.transform(transform);
            fprintf(file, "%f %f %f %f %f %f %f\n", tp0.getX(), tp0.getY(), tp0.getZ(), tp1.getX(), tp1.getY(), tp1.getZ(), radius);
        }
        
        return true;
    }
    
};



