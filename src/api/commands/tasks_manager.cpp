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

#include <iostream>
#include <map>

#include "./tasks_manager.h"
#include "../common.h"

#include "writers/rad/tasks.h"

int solveTaskManager(lua_State * L)
{
  json results = json();
  getCurrentTaskManager(L)->solve(&results);
  return 0;
}

int printTaskManager(lua_State * L)
{
  // Check if any input was given
  int nargs[2] = { 0, 1 };
  int n = checkNArguments(L, nargs, 2);

  // check type
  if (n == 1) {
    checkArgType(L, LUA_TSTRING, 1);
    std::string filename = std::string(lua_tostring(L, 1));
    getCurrentTaskManager(L)->print(&filename[0]);
  }
  else {
    getCurrentTaskManager(L)->print(NULL); // to STDOUT
  }

  return 0;
}

int addTask(lua_State * L)
{  
    // Check number of arguments
    int args[1] = { 1 };
    checkNArguments(L, args, 1);

    // Check the type of arguments
    checkArgType(L, LUA_TTABLE, 1); // Options
    
    // Check minimum arguments
    std::string factoryKey;
    int field = lua_getfield(L, 1, "class"); // "field" is now n position 2.
    if (field != LUA_TSTRING) {
        if (field == LUA_TNIL) {
          missingOption(L, "class", "string");
        }
        else {
          badOptionError(L, "class", lua_typename(L, 2),"string");
        }
    }
    else {
        factoryKey = std::string(lua_tostring(L, 2));
    };
    // remove one value from the stack (or nil, if it was not there)
    lua_pop(L, 1);

    // Get the task dictionary
    std::map<std::string, TaskFactory> * td = getCurrentTaskDictionary(L);
    
    // Check that the Task Factory exists
    size_t q = td->count(factoryKey);
    if (q == 0) {
        std::string errmsg = "Task '" + std::string(factoryKey) + "' is not registered";
        sendError(L, "Unknown Task", &errmsg[0]);
    }
    
    // Get the Task factory
    TaskFactory f = td->at(&factoryKey[0]);
    
    // build the task
    Task * task = f(L);
    
    // Check if the name is required.
    if(task->generatesResults){
        task->reportResults = true; // We do want to report the results
        
        std::string taskName;
        field = lua_getfield(L, 1, "name"); // "field" is now n position 2.
        if (field != LUA_TSTRING) {
            if (field == LUA_TNIL) {
                missingOption(L, "name", "string");
            }
            else {
                badOptionError(L, "name", lua_typename(L, 2), "string");
            }
        }
        else {
            taskName = std::string(lua_tostring(L, 2));
        };
        // remove one value from the stack (or nil, if it was not there)
        lua_pop(L, 1);
        task->setName(&taskName);
    }
    
    // Get current task Manager
    TaskManager * tm = getCurrentTaskManager(L);

    // Add the task
    tm->addTask(task);

    return 0;
}
