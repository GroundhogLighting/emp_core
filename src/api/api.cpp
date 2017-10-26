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

#include "./api.h"
#include "config_constants.h"

#include "./src/options.h"
#include "./src/tasks.h"

void loadAPI(lua_State * L, GroundhogModel * ghmodel, TaskManager * taskManager) {

	/* REGISTER THE GROUNDHOG MODEL */
	lua_pushlightuserdata(L, ghmodel);
	lua_setglobal(L, LUA_MODEL_VARIABLE);

	/* REGISTER THE TASK MANAGER */
	lua_pushlightuserdata(L, taskManager);
	lua_setglobal(L, LUA_TASKMANAGER_VARIABLE);

	/* REGISTER SOLVE FUNCTION */
	lua_register(L, "solve", solveTaskManager);
	
	/* REGISTER EXPORT FUNCTIONS */
	lua_register(L, "write_radiance_dir", addExportToRadianceTask);

	/* REGISTER OCONV FUNCTIONS */
	lua_register(L, "oconv",addWholeOconvTask);

	/* REGISTER OPTIONS FUNCTIONS */
	lua_register(L, "ray_trace_options", set_rtrace_options);
	lua_register(L, "print_ray_trace_options", print_rtrace_options);

	/* REGISTER RTRACE FUNCTIONS */
	lua_register(L, "test_rtrace", addRTRACETask);
}

