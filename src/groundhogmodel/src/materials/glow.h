
#pragma once

#include "../material.h"
#include "json/json.hpp"

using nlohmann::json;

//! Glow material
/*!
 This material is extracted directly from the Radiance reference...
 read that document to learn more
 
 The Primitive of the Glow material es:
 
 void glow %MAT_NAME% 0 0 4 Red_t Green_t Blue_t maxrad
 
 */

class Glow : public Material {
    
public:
    double r = 0.86; //!< The red radiance value
    double g = 0.86; //!< The green radiance value
    double b = 0.86; //!< The blue radiance value
    double maxRadius = 0; //!< maximum radius for shadow testing
    
    
    //! Builds a new Glow material
    /*!
     @author German Molina
     @param j The JSON object describing the Glow
     */
    Glow(json * j)
    {
        primitiveLength = 10;
        fillFromJSON(j);
        setType("glow");
    }

    //! Builds a new Glow material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Glow(std::string * name)
    {
        setName(name);
        setType("glow");
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
        maxRadius = std::stod((*tokens)[i++]);
        
        return true;
    }
    
    
};
