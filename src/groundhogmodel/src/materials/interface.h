

#pragma once

#include "../material.h"
#include "json/json.hpp"

using nlohmann::json;

//! Interface material
/*!
 This material is extracted directly from the Radiance reference...
 read that document to learn more
 
 The Primitive of the Interface material es:
 
 void interface %MAT_NAME% 0 0 8 rtn1 gtn1 btn1 n1 rtn2 gtn2 btn2 n2
 
 */

class Interface : public Material {
    
public:
    double r1 = 0.86; //!< The red transmittance of the first dielectric
    double g1 = 0.86; //!< The green transmittance of the first dielectric
    double b1 = 0.86; //!< The blue transmittance of the first dielectric
    double refractionIndex1 = 1.52; //!< The refraction index of the first dielectric
    double r2 = 0.86; //!< The red transmittance of the second dielectric
    double g2 = 0.86; //!< The green transmittance of the second dielectric
    double b2 = 0.86; //!< The blue transmittance of the second dielectric
    double refractionIndex2 = 1.52; //!< The refraction index of the second dielectric
    
    
    //! Builds a new Interface material
    /*!
     @author German Molina
     @param j The JSON object describing the Interface
     */
    Interface(json * j)
    {
        primitiveLength = 14;
        fillFromJSON(j);
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
        r1 = std::stod((*tokens)[i++]);
        g1 = std::stod((*tokens)[i++]);
        b1 = std::stod((*tokens)[i++]);
        refractionIndex1 = std::stod((*tokens)[i++]);
        r2 = std::stod((*tokens)[i++]);
        g2 = std::stod((*tokens)[i++]);
        b2 = std::stod((*tokens)[i++]);
        refractionIndex2 = std::stod((*tokens)[i++]);
        
        
        return true;
    }
    
    //! Writes the Interface material in Radiance format
    /*!
     @author German Molina
     @param[in] file The file to write to
     @return success
     */
    bool writeInRadianceFormat(FILE * file)
    {
        fprintf(file, "void %s %s\n0\n0\n", &type[0], &name[0]);
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", r1, g1, b1, refractionIndex1, r2, g2, b2, refractionIndex2);
        
        return true;
    }
};
