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

#include "./api.h"
#include "config_constants.h"

#include "common/utilities/stringutils.h"
#include "./load_commands.h"
#include "./load_tasks.h"

#include <utility>



void registerTask(lua_State * L, const char * name, TaskFactory f)
{
  std::map<std::string, TaskFactory> * taskDictionary = getCurrentTaskDictionary(L);
  
  (*taskDictionary)[std::string(name)] = f;

  //taskDictionary->insert(std::make_pair(name, f));  
}



void loadAPI(lua_State * L, GroundhogModel * ghmodel, std::map<std::string, TaskFactory> * taskDictionary, TaskManager * taskManager, int argc, char* argv[]) {

	/* REGISTER THE GROUNDHOG MODEL */
	lua_pushlightuserdata(L, ghmodel);
	lua_setglobal(L, LUA_MODEL_VARIABLE);

	/* REGISTER THE TASK MANAGER */
	lua_pushlightuserdata(L, taskManager);
	lua_setglobal(L, LUA_TASKMANAGER_VARIABLE);

    /* REGISTER TASK DICTIONARY */
    lua_pushlightuserdata(L, taskDictionary);
    lua_setglobal(L, LUA_TASKDICTIONARY_VARIABLE);

	/* LOAD ARGUMENTS TABLE*/
	lua_newtable(L);
	int table_pos = lua_gettop(L);
	for (int i = 3; i < argc; i++) {		
		if (is_number(argv[i])) {
			lua_pushnumber(L, std::stod(argv[i]));
		}
		else {
			lua_pushstring(L, argv[i]);
		}
		
		lua_seti(L, table_pos, i-2);		
	}
	lua_setglobal(L, "argv");

    /* REGISTER COMMANDS */
    registerCommands(L);

    /* REGISTER TASKS */
    registerTasks(L);

    
}

