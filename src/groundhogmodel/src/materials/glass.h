#pragma once

#include "../material.h"
#include "json/json.hpp"

using nlohmann::json;

//! Glass material
/*!
This material is extracted directly from the Radiance reference...
read that document to learn more

The Primitive of the Glass material es:

void glass %MAT_NAME% 0 0 3 Red_t Green_t Blue_t

*/

class Glass : public Material {

public:
	double r = 0.86; //!< The red transmissivity
	double g = 0.86; //!< The green transmissivity
	double b = 0.86; //!< The blue transmissivity
	


	//! Builds a new Glass material
	/*!
	@author German Molina
	@param j The JSON object describing the Glass
	*/
	Glass(json * j)
    {
        primitiveLength = 9;
        fillFromJSON(j);
        setType("glass");
    }
	
    //! Builds a new Glass material
    /*!
     @author German Molina
     @param name The name for the Material
     */
    Glass(std::string * name)
    {
        setName(name);
        setType("glass");
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

    //! Writes the Glass material in Radiance format
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
