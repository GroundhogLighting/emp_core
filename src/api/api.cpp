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
#include "./src/tasks_manager.h"
#include "./src/gh_model.h"
#include "./src/api_io.h"
#include "./src/export_import.h"
#include "./src/radiance_core.h"

#include "common/utilities/stringutils.h"

void loadAPI(lua_State * L, GroundhogModel * ghmodel, TaskManager * taskManager, int argc, char* argv[]) {

	/* REGISTER THE GROUNDHOG MODEL */
	lua_pushlightuserdata(L, ghmodel);
	lua_setglobal(L, LUA_MODEL_VARIABLE);

	/* REGISTER THE TASK MANAGER */
	lua_pushlightuserdata(L, taskManager);
	lua_setglobal(L, LUA_TASKMANAGER_VARIABLE);

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
	lua_setglobal(L, "args");


    /* ======================== */
	/* @APIgroup INPUT / OUTPUT */
	/* ======================== */

    /* @APIfunction
    
    Throws an error and exits the script

    @param[required] message The error message to show
    */
	lua_register(L, "raise", raise);
    
    /* @APIfunction

    Prints a warning to the standard error, but continues processing the 
    script

    @param[required] message The message to warn
    */
	lua_register(L, "warn", warn);

    /* ============================== */
	/* @APIgroup GROUNDHOG MODEL DATA */	
	/* ============================== */

	/* @APIfunction	

	Retrieves an array with the workplanes names in the model

	@return workplane_array An array with the workplanes names
	*/
	lua_register(L, "get_workplanes_list", get_workplane_list);


	/* @APIfunction

	Checks if a workplane does exist in the model

	@param[required] workplane_name The name of the workplane
	@return exist? True or False
	*/
	lua_register(L, "workplane_exist", workplane_exists);


    /* ====================== */
	/* @APIgroup TASK MANAGER */
	/* ====================== */

	/* @APIfunction

	Solves the task manager
	*/
	lua_register(L, "solve_tasks", solveTaskManager);

    /* @APIfunction
    
    Prints the current task flow to the standard output
    */
    //lua_register(L,"print_task_flow",printTaskManager);

    /* @APIfunction 

    Removes current tasks from Task Manager
    */
    //lua_register(L, "purge_tasks", solveTaskManager);
	
    /* @APIfunction

    Adds a task to the task manager
    */
    //lua_register(L,"add_task",addTaskToManager);




    /* ========================= */
	/* @APIgroup EXPORT / IMPORT */
	/* ========================= */

	/* @APIfunction

	Exports the current model to a Radiance directory

	@param[required] directory The name of the workplane	
	*/
	lua_register(L, "export_radiance_model", exportToRadiance);


    /* =============================== */
	/* @APIgroup SET-OPTIONS FUNCTIONS */
	/* =============================== */

    /* @APIfunction
    
    Modifies the ray-tracing options in the current GroundhogModel

    @param[required] options A Table with the ray-tracing options to set
    */
	lua_register(L, "ray_trace_options", set_rtrace_options);

    /* @APIfunction
    Prints the current ray-trace opcions. If a file is given, the options
    will be printed to such file. If not, the options will be printed
    to the Standard Output.

    @param[optional] file The name of the file to write
    */
	lua_register(L, "print_ray_trace_options", print_rtrace_options);

    /* ======================= */
	/* @APIgroup RADIANCE CORE */
	/* ======================= */

    /* @APIfunction
    Creates an octree of a model in a certain base directory (that complies the
    Groundhog file distribution). The name of the octree can be chosen, and 
    several options can be set. They are:

    - include_windows: If true, the windows will be included in the octree. Defaults to true.
    - sky: If a string is given, a sky will be included in the octree. Else, no sky will be given.
    - black_geometry: If true, all the non-window geometry becomes black. Defaults to false.

    @param[required] base_dir The directory where the Radiance Model is stored
    @param[required] octree_name The name of the octree to create
    @param[optional] options The table of options if the octree
    */
    lua_register(L, "oconv", oconv_command);

	
    
    
    
    //lua_register(L, "calc_DF", addDFTask);


}

