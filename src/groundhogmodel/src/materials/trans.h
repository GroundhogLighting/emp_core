
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
        primitiveLength = 13;
        fillFromJSON(j);
    }
    
    //! Prints the Material in Radiance format
    /*!
     @author German Molina
     @param[in] file The file to write to
     @return success
     */
    bool writeInRadianceFormat(FILE * file)
    {
        fprintf(file, "void %s %s\n0\n0\n", &type[0], &name[0]);
        fprintf(file, "7 %f %f %f %f %f %f %f\n", r, g, b, specularity, roughness, transmissivity, tspec);
        
        return true;
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

