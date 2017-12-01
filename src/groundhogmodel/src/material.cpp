/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

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


#include "./material.h"
#include "common/utilities/io.h"
#include "common/utilities/stringutils.h"

Material::Material()
{	
	DEBUG_MSG("Creating a new material");

}

Material::~Material()
{
	DEBUG_MSG("Destroying material " + name);
}

std::string Material::getName()
{
	return name;
}

std::string Material::getType()
{
	return type;
}

bool Material::compareName(std::string * otherName)
{
	return (name == *otherName);
}

bool Material::fillFromJSON(json j)
{
	try {
		name = j.at("name").get<std::string>();
		type = j.at("class").get<std::string>();

		// Get the RAD tokens
		std::vector<std::string> tokens = std::vector<std::string>();
		std::string rad = j["rad"];
		tokenize(&rad, &tokens);

		// check if correct number of tokens
		if (tokens.size() != primitiveLength) {
			fatal("Incorrect primitive when parsing -- " + type, __LINE__, __FILE__);
			return false;
		}

		// Parse primitive tokens
		if (!parsePrimitive(&tokens)) {
			fatal("Error while parsing primitive", __LINE__, __FILE__);
			return false;
		}
	}
	catch (const std::exception & e){
		fatal("Error when converting JSON into Material", __LINE__, __FILE__);
		fatal(e.what(),__LINE__,__FILE__);
		return false;
	}

	return true;
}


bool Material::writeInRadianceFormat(FILE * file)
{
	return true;
}

bool Material::parsePrimitive(std::vector <std::string> * tokens)
{
	return true;
}