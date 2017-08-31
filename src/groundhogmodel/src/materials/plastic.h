#pragma once

#include "../material.h"
#include "../../../3rdparty/json/json.hpp"

//! Plastic material
/*!
This material is extracted directly from the Radiance reference... 
read that document to learn more

The Primitive of the Plastic material es:

void glass %MAT_NAME% 0 0 5 Red_r Green_r Blue_r Specularity Roughness

*/

using nlohmann::json;

class Plastic : public Material {

private:
	double r = 0.6; //!< The red component
	double g = 0.6; //!< The green component
	double b = 0.6; //!< The blue component
	double specularity = 0; //!< The specularity
	double roughness = 0; //!< The roughness

public:

	//! Builds a new Plastic material
	/*!
	@author German Molina
	@param j The JSON object describing the Plastic
	*/
	Plastic::Plastic(json j);

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

	//! Retrieves the specularity
	/*!
	@author German Molina
	@return specularity
	*/
	double spec();

	//! Retrieves the roughness
	/*!
	@author German Molina
	@return roughness
	*/
	double rough();

	//! Prints the Material in Radiance format
	/*!
	@author German Molina
	@param[in] dir The directory to write it at
	@return success
	*/
	bool writeRadianceDefinition(std::string * dir);

	//! Parses a tokenized Groundhog primitive
	/*!
	A Groundhog primitive may be a Radiance primitive or an extension
	of them

	@author German Molina
	@param[in] tokens The primitive tokenized
	@return success
	*/
	bool parsePrimitive(std::vector <std::string> * tokens);

};