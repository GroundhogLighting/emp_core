
#pragma once

#include "../material.h"
#include "json/json.hpp"

using nlohmann::json;

//! Dielectric material
/*!
 This material is extracted directly from the Radiance reference...
 read that document to learn more
 
 The Primitive of the Dielectric material es:
 
 void dielectric %MAT_NAME% 0 0 5 Red Green Blue refrIndex hConstant
 
 */


class Dielectric : public Material {
    
public:
    double r = 0.6; //!< The red component
    double g = 0.6; //!< The green component
    double b = 0.6; //!< The blue component
    double refractionIndex = 1.52; //!< The refraction index
    double hartmannConstant = 0; //!< The hartman constant. Usuallt zero
    
    
    //! Builds a new Dielectric material
    /*!
     @author German Molina
     @param j The JSON object describing the Dielectric
     */
    Dielectric(json * j)
    {
        primitiveLength = 11;
        fillFromJSON(j);
        setType("dielectric");
    }

    //! Builds a new Dielectric material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Dielectric(std::string * name)
    {
        setName(name);
        setType("dielectric");
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
        fprintf(file, "5 %f %f %f %f %f\n", r, g, b, refractionIndex, hartmannConstant);
        
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
        refractionIndex = std::stod((*tokens)[i++]);
        hartmannConstant = std::stod((*tokens)[i++]);
        return true;
    }
    
};
