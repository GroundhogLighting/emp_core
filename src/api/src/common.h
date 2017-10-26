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
#pragma once

#include "./config_constants.h"

#include "./common/taskmanager/taskmanager.h"
#include "./groundhogmodel/groundhogmodel.h"


// Include LUA headers
extern "C" {
	#include <lua.h>
	//#include <lualib.h>
	#include <lauxlib.h> 
}


//! Retrieves the current GroundhogModel exposed to the API
/*!
@author German Molina
@param L The lua_State * object
@return The pointer to the current GroundhogModel
*/
GroundhogModel * getCurrentModel(lua_State * L);


//! Retrieves the current TaskManager exposed to the API
/*!
@author German Molina
@param L The lua_State * object
@return The pointer to the current TaskManager
*/
TaskManager * getCurrentTaskManager(lua_State * L);

//! Checks that the number of arguments matches a certain value
/*!
@author German Molina
@param[in] L The lua_State * object
@param[in] nargs The number of expected arguments
@return The pointer to the current TaskManager
*/
bool checkNArguments(lua_State * L, int nargs);