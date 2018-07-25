/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

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


#include "./workplane.h"

#include "../../common/utilities/io.h"

Workplane::Workplane(std::string workplaneName) {

	name = workplaneName;


}

Workplane::~Workplane() {

    for(auto x : polygons)
        delete x;
}

void Workplane::addPolygon(Polygon3D * polygon)
{
	polygons.push_back(polygon);
}

std::string Workplane::getName() const
{
	return name;
}

bool Workplane::compareName(const std::string * const otherName) const
{
	return name == *otherName;
}


const size_t Workplane::getNumPolygons() const
{
	return polygons.size();
}

Polygon3D * const Workplane::getPolygonRef(const size_t i) const
{
	return polygons[i];
}

const double Workplane::getMaxArea() const
{
  return maxArea;
}

const double Workplane::getMaxAspectRatio() const
{
    return maxAspectRatio;
}


void Workplane::setMaxArea(const double v)
{
    maxArea = v;
}

void Workplane::setMaxAspectRatio(const double v)
{
    maxAspectRatio = v;
}

void Workplane::addTask(const std::string taskName)
{
    tasks.push_back(taskName);
}

const std::vector<std::string> * const Workplane::getTasks() const
{
    return &tasks;
}
