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

#include "./windowgroup.h"
#include "common/utilities/io.h"

WindowGroup::WindowGroup(std::string groupName) {
	name = groupName;
}

WindowGroup::~WindowGroup() {
    for(auto x : windows)
        delete x;
}


void WindowGroup::addFace(Face * face) {
	windows.push_back(face);
}

std::string WindowGroup::getName() {
	return name;
}

bool WindowGroup::compareName(std::string * otherName) {
	return name == *otherName;
}

size_t WindowGroup::getNumWindows() {
	return windows.size();
}

Face * WindowGroup::getWindowRef(size_t i) {
	return windows[i];
}
