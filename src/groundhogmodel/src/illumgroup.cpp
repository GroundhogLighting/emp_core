
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


#include <string>

#include "./illumgroup.h"
#include "../../common/utilities/io.h"

IllumGroup::IllumGroup(std::string groupName) {
    name = groupName;
}

IllumGroup::~IllumGroup() {
    for(auto x : polygons)
        delete x;
}


void IllumGroup::addPolygon(Polygon3D * polygon) {
    polygons.push_back(polygon);
}

std::string IllumGroup::getName() {
    return name;
}

bool IllumGroup::compareName(std::string * otherName) {
    return name == *otherName;
}

size_t IllumGroup::size() {
    return polygons.size();
}

Polygon3D * IllumGroup::getPolygonRef(size_t i) {
    return polygons[i];
}
