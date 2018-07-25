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
        setType("dielectric");
        std::string name = j->at("name").get<std::string>();
        setName(&name);
        
        json color = j->at("color").get<json>();
        r = getFromJSON("r",&color);
        g = getFromJSON("g",&color);
        b = getFromJSON("b",&color);
        
        refractionIndex = getFromJSON("refraction_index",j);
        hartmannConstant = getFromJSON("hartmann_constant",j);
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
