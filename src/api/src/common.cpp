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

bool checkNArguments(lua_State * L, int nargs)
{
	int n = lua_gettop(L);
	if (n < nargs) {
		std::cerr << "too few arguments" << std::endl;
		return false;
	}
	else if (n > nargs) {
		std::cerr << "too many arguments" << std::endl;
		return false;
	}
	return true;
}