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


#include "./loop.h"
#include "../utilities/io.h"

Loop::Loop() 
{
	DEBUG_MSG("Creating loop");
	//vertices.reserve(3);
	vertices = std::vector < Point3D * >();
}

Loop::Loop(Loop * loop) 
{
	vertices = std::vector < Point3D * >();
	size_t numVertices = loop->size();
	for (int i = 0; i < numVertices; i++) {
		addVertex(new Point3D(loop->getVertexRef(i)));
	}
}

Loop::~Loop() 
{

	for (unsigned i = 0; i < vertices.size(); i++) {
		delete vertices[i];
	}

	DEBUG_MSG("Destroying loop");
}

void Loop::addVertex(Point3D * point) 
{
	vertices.push_back(point);
}


size_t Loop::size() 
{
	return vertices.size();
}


Point3D * Loop::getVertexRef(size_t i) 
{
	return vertices[i];
}

bool Loop::clean() {	
	return true;
}


