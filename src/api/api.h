
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
#include "common/taskmanager/taskmanager.h"
#include "groundhogmodel/groundhogmodel.h"


// Include LUA headers
extern "C" {
#include <lua.h>
	//#include <lualib.h>
#include <lauxlib.h> 
}

//! Loads the Lua API and all the functions available in the scripts
/*
@author German Molina
@param[in] L the lua_State
@param[in] ghmodel The GroundhogModel to set as global variable
@param[in] taskManager The TaskManager to set as global variable
@param[in] argc Number of arguments
@param[in] argv The arguments given to the command line
*/
void loadAPI(lua_State * L, GroundhogModel * ghmodel, TaskManager * taskManager, int argc, char* argv[]);
