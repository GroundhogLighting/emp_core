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

#include <iostream>

#include "./tasks_manager.h"
#include "./common.h"

#include "common/taskmanager/tasks/export.h"
#include "common/taskmanager/tasks/oconv.h"
#include "common/taskmanager/tasks/daylightfactor.h"

int solveTaskManager(lua_State * L)
{
	getCurrentTaskManager(L)->solve();
	return 0;
}



int addDFTask(lua_State * L)
{
	GroundhogModel * model = getCurrentModel(L);
	TaskManager * tm = getCurrentTaskManager(L);
	RTraceOptions * options = model->getRTraceOptions();

	// Check nuber of arguments
    checkNArguments(L, 1);

	// Check type
    checkArgType(L, LUA_TTABLE, 1);

	if (lua_getfield(L, 1, "workplane") != LUA_TNIL) {
		std::string value = luaL_checkstring(L, 2);
		tm->addTask(new CalcDF(GLARE_TMP_DIR,model,value,options));
	}
	else {

	}
	lua_pop(L, 1);

	return 0;
}