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

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "./commands/options.h"
#include "./commands/tasks_manager.h"
#include "./commands/gh_model.h"
#include "./commands/api_io.h"
#include "./commands/export_import.h"
#include "./commands/radiance_core.h"


//! Registers all the available commands in the API
/*!
@author German Molina
@param[in] L The lua_State * object
*/
void registerCommands(lua_State * L)
{
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
  lua_register(L, "solve", solveTaskManager);

  /* @APIfunction

  Prints the current task flow to the standard output
  */
  lua_register(L, "print_task_flow", printTaskManager);

  /* @APIfunction

  Removes current tasks from Task Manager
  */
  //lua_register(L, "purge_tasks", solveTaskManager);

  /* @APIfunction

  Adds a task to the task manager

  @param[required] task_name The name of the task to add
  @param[required] options The options given
  */
  lua_register(L, "task", addTask);




  /* ========================= */
  /* @APIgroup EXPORT / IMPORT */
  /* ========================= */


  /* @APIfunction

  Exprts a

  */
  lua_register(L, "export_workplane", exportWorkplaneToRadiance);

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
  Creates an octree of a model. The name of the octree can be chosen, and
  several options can be set. They are:

  - include_windows: If true, the windows will be included in the octree. Defaults to true.
  - sky: If a string is given, a sky will be included in the octree. Else, no sky will be given.
  - black_geometry: If true, all the non-window geometry becomes black. Defaults to false.
 
  @param[required] octree_name The name of the octree to create
  @param[optional] options The table of options if the octree
  */
  lua_register(L, "oconv", oconv_command);

  /* @APIfunction
  Calculates the illuminance in a workplane from a certain octree.

  Options are:

  - max_area: The maximum area allowed in the workplane triangulation
  - max_aspect_ratio: The maximum aspect ratio allowed in the workplane triangulation
  - ambient_file: The ambient file to use

  @param[required] workplane_name The name of the directory
  @param[required] octree_name The name of the octree to create
  @param[optional] options The table of options if the octree
  */
  lua_register(L, "calc_illuminance", rtrace_command);





}
