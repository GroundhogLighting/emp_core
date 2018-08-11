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
        setType("light");
        std::string name = j->at("name").get<std::string>();
        setName(&name);
        
        json color = j->at("color").get<json>();
        r = getFromJSON("r",&color);
        g = getFromJSON("g",&color);
        b = getFromJSON("b",&color);
                
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
    
};
