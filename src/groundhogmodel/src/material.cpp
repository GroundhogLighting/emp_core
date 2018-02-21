/*****************************************************************************
	Emp

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

}

Material::~Material()
{
}

std::string * Material::getName()
{
	return &name;
}

std::string * Material::getType()
{
	return &type;
}

bool Material::compareName(std::string * otherName)
{
	return (name == *otherName);
}

bool Material::fillFromJSON(json * j)
{
	try {
		name = j->at("name").get<std::string>();
		type = j->at("class").get<std::string>();

		// Get the RAD tokens
		std::vector<std::string> tokens = std::vector<std::string>();
		std::string rad = (*j)["rad"];
		tokenize(&rad, &tokens);

		// check if correct number of tokens
		if (tokens.size() != primitiveLength) {
          FATAL(errorMessage,"Incorrect primitive when parsing -- " + type);
		  return false;
		}

		// Parse primitive tokens
		if (!parsePrimitive(&tokens)) {
			FATAL(errorMessage,"Error while parsing primitive");
			return false;
		}
	}
	catch (const std::exception & ex){
		FATAL(errorMessage,"Error when converting JSON into Material");
        FATAL(errorMessage2,ex.what());
        return false;
	}

	return true;
}

