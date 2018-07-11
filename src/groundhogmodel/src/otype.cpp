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

#include "./otype.h"


Otype::Otype()
{

}

Otype::~Otype()
{
    
}


std::string Otype::getName() const
{
	return name;
}

void Otype::setName(const std::string * newName)
{
	name = *newName;
}


std::string Otype::getType() const
{
	return type;
}

bool Otype::compareName(const std::string * otherName) const
{
	return (name == *otherName);
}

void Otype::setType(const std::string theType)
{
    type = theType;
}


const Material * const Otype::getMaterial() const
{
	return material;
}

void Otype::setMaterial(Material * const mat)
{
	material = mat;
}


