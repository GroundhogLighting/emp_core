
#pragma once

#include "../material.h"

#include "json/json.hpp"

using nlohmann::json;

//! Trans material
/*!
 This material is extracted directly from the Radiance reference...
 read that document to learn more
 
 The Primitive of the Trans material es:
 
 void spotlight %MAT_NAME% 0 0 7 Red Green Blue Angle xdir ydir zdir
 
 */


class Trans : public Material {
    
public:
    double r = 0.6; //!< The red radiance
    double g = 0.6; //!< The green radiance
    double b = 0.6; //!< The blue radiance
    double specularity = 0; //!< The full cone angle (in degrees)
    double roughness = 0; //!< The X component of the orientation
    double transmissivity = 0; //!< The fraction of penetrating light that travels all the way through the material
    double tspec = 0; //!< The fraction of transmitted light that is not diffusely scattered
    
    
    //! Builds a new Trans material
    /*!
     @author German Molina
     @param j The JSON object describing the Trans
     */
    Trans(json * j)
    {
        setType("trans");
        std::string name = j->at("name").get<std::string>();
        setName(&name);
        
        json color = j->at("color").get<json>();
        r = getFromJSON("r",&color);
        g = getFromJSON("g",&color);
        b = getFromJSON("b",&color);
        
        specularity = getFromJSON("specularity",j);
        roughness = getFromJSON("roughness",j);
        transmissivity = getFromJSON("transmissivity",j);
        tspec = getFromJSON("tspec",j);
    }

    //! Builds a new Light material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Trans(std::string * name)
    {
        setName(name);
        setType("trans");
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
        specularity = std::stod((*tokens)[i++]);
        roughness = std::stod((*tokens)[i++]);
        transmissivity = std::stod((*tokens)[i++]);
        tspec = std::stod((*tokens)[i++]);
        return true;
    }
    
};

