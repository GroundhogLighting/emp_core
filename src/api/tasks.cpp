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

  OconvOptions oconvOptions = OconvOptions();

  GroundhogModel * model = getCurrentModel(L);
  RTraceOptions * rtraceOptions = model->getRTraceOptions();  

  OptionSet otherOptions = OptionSet();
  otherOptions.addOption("workplane", "none");
  otherOptions.addOption("max_area", 0.25);
  otherOptions.addOption("max_aspect_ratio", 1.3);

  // RTRace options are obtained from the global options
  otherOptions.fillFromLuaTable(L, 2);
  oconvOptions.fillFromLuaTable(L, 2);

  // Check that the workplane exists
  std::string wpName = otherOptions.getOption<std::string>("workplane");
  Workplane * wp = getWorkplane(L,wpName);


  RTraceTask * res = new RTraceTask(model, rtraceOptions, &otherOptions, wp, &oconvOptions);
  

  return res;
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

