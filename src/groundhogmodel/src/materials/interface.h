/*****************************************************************************
 Emp
 
 Copyright (C) 2018  German Molina (germolinal@gmail.com)
 
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
        setType("interface");
        std::string name = j->at("name").get<std::string>();
        setName(&name);
        
        json color1 = j->at("color1").get<json>();
        r1 = getFromJSON("r",&color1);
        g1 = getFromJSON("g",&color1);
        b1 = getFromJSON("b",&color1);
        
        json color2 = j->at("color2").get<json>();
        r1 = getFromJSON("r",&color2);
        g1 = getFromJSON("g",&color2);
        b1 = getFromJSON("b",&color2);
        
        refractionIndex1 = getFromJSON("refraction1",j);
        refractionIndex2 = getFromJSON("refraction2",j);
    }

    //! Builds a new Glow material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Interface(std::string * name)
    {
        setName(name);
        setType("interface");
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
    
};
