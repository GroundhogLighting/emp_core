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

#include "./componentinstance.h"
#include "./layer.h"
#include "../../common/utilities/io.h"

Layer::Layer(std::string * layerName) 
{
	name = *layerName;
};

Layer::~Layer() 
{
    for(auto x : objects)
        delete x;
    
    for(auto x : instances)
        delete x;
};

bool Layer::compareName(std::string * layerName) 
{
	return name == *layerName;
}

void Layer::addObject(Otype * o) 
{
	objects.push_back(o);
}

std::vector <ComponentInstance * > * Layer::getComponentInstancesRef() 
{
	return &instances;
}

std::string * Layer::getName() 
{
	return &name;
}

std::vector <Otype * > * Layer::getObjectsRef() 
{
	return &objects;
}


Otype * Layer::getObjectRef(size_t i) 
{
	return objects[i];
}


ComponentInstance * Layer::getComponentInstanceRef(size_t i) 
{
	return instances[i];
}


bool Layer::isEmpty() 
{
	return (objects.size() == 0 && instances.size() == 0);
}
