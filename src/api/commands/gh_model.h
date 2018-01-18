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

#pragma once


// Include LUA headers
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//! Retrieves a list of the Workplane objects in the GroundhogModel
/*!
@author German Molina
@param[in] L The lua_State of the api
@return the number of return values within the script
*/
int get_workplane_list(lua_State * L);

//! Checks if a workplane exists in a GroundhogModel
/*
@author German Molina
@param[in] L The lua state
@return the number of return values within the script
*/
int workplane_exists(lua_State * L);
