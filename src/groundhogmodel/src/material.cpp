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
#include "../../common/utilities/io.h"
#include "../../common/utilities/stringutils.h"

Material::Material()
{	

}

Material::~Material()
{
    
}

double Material::getFromJSON(const char * key, json * j)
{
    if(j->find(key) != j->end()){
        return j->at(key).get<double>();
    }else{
        std::string err = "JSON has not key '"+std::string(key)+"' when creating material";
        FATAL(e,err);
        return -1;
    }
    return -1;
}

std::string * Material::getName()
{
	return &name;
}

void Material::setName(std::string * aName)
{
	name = *aName;
}

std::string * Material::getType()
{
	return &type;
}

void Material::setType(std::string theType)
{
	type = theType;
}

bool Material::compareName(std::string * otherName)
{
	return (name == *otherName);
}


