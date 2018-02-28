
#pragma once

#include "../material.h"
#include "json/json.hpp"

using nlohmann::json;

//! Light material
/*!
 This material is extracted directly from the Radiance reference...
 read that document to learn more
 
 The Primitive of the Light material es:
 
 void light %MAT_NAME% 0 0 3 red green blue
 
 */

class Light : public Material {
    
public:
    double r = 100; //!< The red radiance value
    double g = 100; //!< The green radiance value
    double b = 100; //!< The blue radiance value
    
    
    
    //! Builds a new Light material
    /*!
     @author German Molina
     @param j The JSON object describing the Light
     */
    Light(json * j)
    {
        primitiveLength = 9;
        fillFromJSON(j);
        setType("light");
    }

    //! Builds a new Light material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Light(std::string * name)
    {
        setName(name);
        setType("light");
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
        
        return true;
    }
    
    //! Writes the Light material in Radiance format
    /*!
     @author German Molina
     @param[in] file The file to write to
     @return success
     */
    bool writeInRadianceFormat(FILE * file)
    {
        fprintf(file, "void %s %s\n0\n0\n", &type[0], &name[0]);
        fprintf(file, "3 %f %f %f\n", r, g, b);
        
        return true;
    }
};
