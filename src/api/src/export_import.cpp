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
#include "./export_import.h"
#include "groundhogmodel/groundhogmodel.h"
#include "./tasks_manager.h"
#include "common/taskmanager/tasks/export.h"

int exportToRadiance(lua_State * L)
{
  GroundhogModel * model = getCurrentModel(L);
  
  // Need an independent TaskManager for this
  TaskManager * taskManager = new TaskManager();

  // Check argument number
  checkNArguments(L, 1);
  
  // Check type
  checkArgType(L, LUA_TSTRING, 1);

  // get target dir.
  std::string dir = lua_tostring(L, 1);
    
  taskManager->addTask(new ExportRadianceDirWithWorkplanes(dir, model, false));
  taskManager->solve();

  delete taskManager;
  
  return 0;
}
