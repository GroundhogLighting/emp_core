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


#include "componentdefinition.h"

#include "common/utilities/io.h"

#include <string>

ComponentDefinition::ComponentDefinition(std::string * componentName) 
{
	name = *componentName;
	DEBUG_MSG("Creating component " + *componentName);
}


ComponentDefinition::~ComponentDefinition() 
{
	DEBUG_MSG("Destroying component " + name);

	for (size_t i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	
	for (size_t i = 0; i < instances.size(); i++) {
		delete instances[i];
	}
}


void ComponentDefinition::addObject(Otype * o)
{
	DEBUG_MSG("ADDING FACE TO" + name);
	objects.push_back(o);
}

size_t ComponentDefinition::getNumObjects() 
{
	return objects.size();
}


std::string ComponentDefinition::getName() 
{
	return name;
}


Otype * ComponentDefinition::getObjectRef(size_t i)
{
	return objects[i];
}


ComponentInstance * ComponentDefinition::getComponentInstanceRef(size_t i) 
{
	return instances[i];
}

std::vector <Otype * > * ComponentDefinition::getObjectsRef()
{
	return &objects;
}

std::vector <ComponentInstance * > * ComponentDefinition::getComponentInstancesRef() 
{
	return &instances;
}