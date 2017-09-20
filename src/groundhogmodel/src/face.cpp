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


#include "./face.h"

#include "common/utilities/io.h"
#include "config_constants.h"

Face::Face(std::string faceName) 
{
	DEBUG_MSG("Creating face " + faceName);

	name = faceName;
	polygon = new Polygon3D();


}

Face::~Face() 
{

	//destroy polygon
	delete polygon;
	
	DEBUG_MSG("Destroying face "+name);
}


std::string Face::getName() 
{
	return name;
}


void Face::setPolygon(Polygon3D * newPolygon) 
{
	delete polygon;
	polygon = newPolygon;
}

bool Face::hasInnerLoops() 
{
	return polygon->hasInnerLoops();
}

bool Face::hasTooManyInnerLoops() 
{
	return polygon->countInnerLoops() > GLARE_TOO_MANY_LOOPS;
}

Loop * Face::getOuterLoopRef() 
{
	return polygon->getOuterLoopRef();
}

Loop * Face::getClosedLoop() 
{
	return polygon->getClosedLoop();
}

Material * Face::getMaterial()
{
	return material;
}

void Face::setMaterial(Material * mat)
{
	material = mat;
}