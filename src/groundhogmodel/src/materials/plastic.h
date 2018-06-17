#pragma once

#include "../material.h"

#include "json/json.hpp"

using nlohmann::json;

//! Plastic material
/*!
This material is extracted directly from the Radiance reference... 
read that document to learn more

The Primitive of the Plastic material es:

void plastic %MAT_NAME% 0 0 5 Red_r Green_r Blue_r Specularity Roughness

*/


class Plastic : public Material {

public:
	double r = 0.6; //!< The red component
	double g = 0.6; //!< The green component
	double b = 0.6; //!< The blue component
	double specularity = 0; //!< The specularity
	double roughness = 0; //!< The roughness


	//! Builds a new Plastic material
	/*!
	@author German Molina
	@param j The JSON object describing the Plastic
	*/
	Plastic(json * j)
    {
        setType("plastic");
        std::string name = j->at("name").get<std::string>();
        setName(&name);
        
        json color = j->at("color").get<json>();
        r = getFromJSON("r",&color);
        g = getFromJSON("g",&color);
        b = getFromJSON("b",&color);
        
        specularity = getFromJSON("specularity",j);
        roughness = getFromJSON("roughness",j);                
    }		

    //! Builds a new Light material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Plastic(std::string * name)
    {
        setName(name);
        setType("plastic");
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
        return true;
    }

};
