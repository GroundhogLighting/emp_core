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

The number of arguments is obtained from the size of the Lua stack

@author German Molina
@param[in] L The lua_State * object
@param[in] nargs The number of expected arguments
*/
void checkNArguments(lua_State * L, int nargs);

//! Throws a Lua error informing that the number of arguments is incorrect
/*!

The number of given arguments is obtained from the size of the Lua stack

@author German Molina
@param[in] L The lua_State * object
@param[in] nRequiredArgs The number of arguments expected in the Lua state
*/
void nArgumentError(lua_State * L, int nRequiredArgs);

//! Checks if a certain argument (in the stack) is of a certain type
/*!
@author German Molina
@param[in] L The lua_State * object
@param[in] expectedType The expected lua_type 
@param[in] argPosition The position in the stack of the argument to check 
*/
void checkArgType(lua_State * L, int expectedType, int argPoisition);

//! Throws a Lua error informing that a certain argument is not of the expected type
/*!
@author German Molina
@param[in] L The lua_State * object
@param[in] expectedType The expected lua_type
@param[in] argPosition The position in the stack of the argument to check
*/
void argTypeError(lua_State * L, int expectedType, int argPoisition);

//! Throws a Lua error
/*!
@aughor German Molina
@param[in] L The lua_State * object
@param[in] kind The kind of error (i.e. usage, number of arguments, etc.)
@param[in] err The error message
*/
void sendError(lua_State * L, const char * kind, const char * err);