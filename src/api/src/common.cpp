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

#include "./common.h"
#include <iostream>

GroundhogModel * getCurrentModel(lua_State * L)
{
	lua_getglobal(L, LUA_MODEL_VARIABLE);
	GroundhogModel * model = (GroundhogModel *)lua_touserdata(L, lua_gettop(L));
	lua_pop(L, 1);
	return model;
}

TaskManager * getCurrentTaskManager(lua_State * L)
{
	lua_getglobal(L, LUA_TASKMANAGER_VARIABLE);
	TaskManager * taskManager = (TaskManager *)lua_touserdata(L, lua_gettop(L));
	lua_pop(L, 1);
	return taskManager;
}

void checkNArguments(lua_State * L, int nRArgs)
{
	int nargs = lua_gettop(L);
	if (nargs != nRArgs) 
		nArgumentError(L, nRArgs);			
}


void nArgumentError(lua_State * L, int nRequiredArgs)
{
	std::string errmsg = "Function requires " + std::to_string(nRequiredArgs) + " but received " + std::to_string(lua_gettop(L));
	sendError(L, "Number of arguments", &errmsg[0]);
}

void checkArgType(lua_State * L, int expectedType, int argPoisition)
{
  if (lua_type(L, argPoisition) != expectedType)
    argTypeError(L, expectedType, 1);    
}


void argTypeError(lua_State * L, int expectedType, int argPoisition)
{
  std::string errmsg = "Argument "+std::to_string(argPoisition)+" should be a " + std::string(lua_typename(L,expectedType)) + " but a "+std::string(lua_typename(L,lua_type(L,argPoisition)))+" was received";
  sendError(L, "Argument type", &errmsg[0]);
}


void sendError(lua_State * L, const char * kind, const char * err)
{
	lua_Debug ar;
	lua_getstack(L, 1, &ar);
	lua_getinfo(L, "nSl", &ar);

	std::string errMsg = "'"+std::string(kind) + "' error in line " + std::to_string(ar.currentline) + ": " + std::string(err);
	lua_pushstring(L, &errMsg[0]);
	lua_error(L);
}