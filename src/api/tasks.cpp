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

#include "./tasks.h"

#include "common/taskmanager/tasks/raytrace.h"
#include "common/taskmanager/tasks/export.h"

Task * workplaneIlluminanceFactory(lua_State * L)
{

  OconvOptions options = OconvOptions();
  /* Options default to OconvOptions
  options.addOptions("include_windows",true);
  options.addOption("black_geometry", false);
  options.addOption("sky", "current");
  options.addOption("include_sky", true);
  options.addOption("lights_on", false);
  */
  options.addOption("workplane", "none");
  options.addOption("max_area", 0.25);
  options.addOption("max_aspect_ratio", 1.3);

  // Fill with data from the given options
  options.fillFromLuaTable(L, 2);

  // Retrieve the current model
  GroundhogModel * model = getCurrentModel(L);

  // Check that the workplane exists
  std::string wpName = options.getOption<std::string>("workplane");
  Workplane * wp = model->getWorkplaneByName(wpName);
  if (wp == NULL) {
    std::string err = "Workplane " + wpName + " does not exist in the model";
    sendError(L, "No Workplane", &err[0]);
    return 0;
  }

  return new RTraceTask(model, &options, wp);

}

Task * exportModel(lua_State * L)
{
  OptionSet options = OptionSet();
  options.addOption("directory", "RadianceModel");
  options.addOption("verbose", true);

  // Fill with data from the given options
  options.fillFromLuaTable(L, 2);

  GroundhogModel * model = getCurrentModel(L);

  return new ExportRadianceDirWithWorkplanes(options.getOption<std::string>("directory"), model, options.getOption<bool>("verbose"));
}