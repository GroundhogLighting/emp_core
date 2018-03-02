#pragma once

#include "../material.h"

#include "json/json.hpp"

using nlohmann::json;

//! Spotlight material
/*!
 This material is extracted directly from the Radiance reference...
 read that document to learn more
 
 The Primitive of the Spotlight material es:
 
 void spotlight %MAT_NAME% 0 0 7 Red Green Blue Angle xdir ydir zdir
 
 */

#include "../../../common/geometry/vector.h"

class Spotlight : public Material {
    
public:
    double r = 0.6; //!< The red radiance
    double g = 0.6; //!< The green radiance
    double b = 0.6; //!< The blue radiance
    double angle = 0; //!< The full cone angle (in degrees)
    Vector3D direction = Vector3D(0,0,1); //!< The direction where the light points
    
    
    //! Builds a new Spotlight material
    /*!
     @author German Molina
     @param j The JSON object describing the Spotlight
     */
    Spotlight(json * j)
    {
        primitiveLength = 13;
        fillFromJSON(j);
        setType("spotlight");
    }

    //! Builds a new Light material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Spotlight(std::string * name)
    {
        setName(name);
        setType("spotlight");
    }
    
    
    //! Parses a tokenized Groundhog primitive
    /*!
     A Groundhog primitive may be a Radiance primitive or an extension
     of them
     
     @author German Molina
     @param[in] tokens The primitive tokenized
     @return success
     */
    bool parsePrimitive(std::vector <std::string> * tokens)
    {
        size_t i = 6;
        r = std::stod((*tokens)[i++]);
        g = std::stod((*tokens)[i++]);
        b = std::stod((*tokens)[i++]);
        angle = std::stod((*tokens)[i++]);
        double x = std::stod((*tokens)[i++]);
        double y = std::stod((*tokens)[i++]);
        double z = std::stod((*tokens)[i++]);
        direction = Vector3D(x,y,z);
        return true;
    }
    
};

