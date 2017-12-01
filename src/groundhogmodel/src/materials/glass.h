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

private:
	double r = 0.86; //!< The red transmissivity
	double g = 0.86; //!< The green transmissivity
	double b = 0.86; //!< The blue transmissivity
	
public:

	//! Builds a new Glass material
	/*!
	@author German Molina
	@param j The JSON object describing the Glass
	*/
	Glass(json j);

	//! Retrieves the 'r' member
	/*!
	@author German Molina
	@return the red reflectance
	*/
	double red();

	//! Retrieves the 'g' member
	/*!
	@author German Molina
	@return the green reflectance
	*/
	double green();

	//! Retrieves the 'b' member
	/*!
	@author German Molina
	@return the red reflectance
	*/
	double blue();

	//! Parses a tokenized Groundhog primitive
	/*!
	A Groundhog primitive may be a Radiance primitive or an extension
	of them

	@author German Molina
	@param[in] tokens The primitive tokenized
	@return success
	*/
	bool parsePrimitive(std::vector <std::string> * tokens);

    //! Writes the Glass material in Radiance format
    /*!
    @author German Molina
    @param[in] file The file to write to
    @return success
    */
    bool writeInRadianceFormat(FILE * file);
};