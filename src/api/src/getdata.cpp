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

#include "./getdata.h"
#include "./common.h"

#include <iostream>

int get_workplane_list(lua_State * L)
{
	GroundhogModel * model = getCurrentModel(L);
	size_t nwps = model->getNumWorkplanes();

	// Create the table
	lua_newtable(L); // index = 1
	
	// push workplane names
	for (size_t i = 0; i < nwps; i++) {
		lua_pushstring(L, &(model->getWorkplaneRef(i)->getName())[0]);
		lua_seti(L, 1, i+1); 
	}

	return 1;
}