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

#include "./radiance_core.h"
#include "calculations/radiance.h"
#include "../common.h"
#include "calculations/tasks/RTraceTask.h"
#include "calculations/tasks/OconvTask.h"

int oconv_command(lua_State * L)
{
  // Check number of arguments
  int args[2] = { 1,2 };
  int n = checkNArguments(L, args, 2);

  // Check the type of arguments
  checkArgType(L, LUA_TSTRING, 1); // octreeName
  if (n == 2) {
    checkArgType(L, LUA_TTABLE, 2); // Options
  }
  
  // All ok as inputs.
  std::string octreeName = lua_tostring(L,1);
  OconvOptions options = OconvOptions();

  // Fill if needed
  if (n == 2)
    options.fillFromLuaTable(L, 2);
  
  GroundhogModel * model = getCurrentModel(L);  
  RadExporter exporter = RadExporter(model);
  
  if (!oconv(octreeName, &options, exporter))
    executionError(L, "Error while trying to create an octree");

  return 0;
}

int rtrace_command(lua_State * L)
{
    // Check number of arguments
    int args[2] = { 2,3 };
    int n = checkNArguments(L, args, 2);

    // Check the type of arguments
    checkArgType(L, LUA_TSTRING, 1); // Workplane name
    if (n >= 2) {
        checkArgType(L, LUA_TSTRING, 2); // Octree name
    }
    if (n == 3) {
        checkArgType(L, LUA_TTABLE, 3); // optons
    }

    // All ok as inputs.
    std::string workplaneName = lua_tostring(L, 1);
    std::string octreeName = lua_tostring(L, 2);

    GroundhogModel * model = getCurrentModel(L);

    // Retrieve the workplane
    Workplane * wp = getWorkplane(L, workplaneName);

    // Get the current RTraceOptions
    RTraceOptions * options = model->getRTraceOptions();

    // Build other options
    OptionSet otherOptions = OptionSet();
    otherOptions.addOption("max_area", 0.25);
    otherOptions.addOption("max_aspect_ratio", 1.3);
    otherOptions.addOption("ambient_file", octreeName+".amb");
    if (n == 3) {
        otherOptions.fillFromLuaTable(L, 3);
    }

    OconvOptions oconvOptions = OconvOptions();
    if (n == 3) {
        oconvOptions.fillFromLuaTable(L, 3);
    }

    // Create Task
    RTraceTask * task = new RTraceTask(model, options, wp, &oconvOptions);
    
    // Need an independent TaskManager for this
    TaskManager taskManager = TaskManager();
    taskManager.addTask(task);
    json results = json();
    taskManager.solve(&results);

    // Report
    
    for (auto ray : *(task->rays)) {

      // USE RESULTS
      double red = colval(ray.rcol, RED);
      double green = colval(ray.rcol, GRN);
      double blue = colval(ray.rcol, BLU);
      std::cout << red << ", " << green << ", " << blue << ", " << " === " << blue*179.0 << std::endl;
    }


    
  return 0;
}
