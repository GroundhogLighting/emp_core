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

#include "../utilities/io.h"
#include "./bbox.h"

BBox::BBox(Polygon3D * polygon)
{
	min_x = HUGE;
	max_x = MINUS_HUGE;
	min_y = HUGE;
	max_y = MINUS_HUGE;
	min_z = HUGE;
	max_z = MINUS_HUGE;	
		

	Loop * loop = polygon->getOuterLoopRef();
	
	if (loop->size() == 0) {
		min_x = 0;
		max_x = 0;
		min_y = 0;
		max_y = 0;
		min_z = 0;
		max_z = 0;
		c_x = 0;
		c_y = 0;
		c_z = 0;

		warn("Creating a BBOX of a Polygon with an empty outer loop");
		return;
	}

	for (size_t i = 0; i < loop->size(); i++) {
		Point3D * p = loop->getVertexRef(i);
		double x = p->getX(); 
		double y = p->getY(); 
		double z = p->getZ();
		
		if (x < min_x)
			min_x = x;

		if (x > max_x)
			max_x = x;

		if (y < min_y)
			min_y = y;

		if (y > max_y)
			max_y = y;

		if (z < min_z)
			min_z = z;

		if (z > max_z)
			max_z = z;

		c_x = (min_x + max_x) / 2;
		c_y = (min_y + max_y) / 2;
		c_z = (min_z + max_z) / 2;
		
	}
}