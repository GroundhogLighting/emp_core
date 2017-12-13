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

#include "./radiance_core.h"
#include "calculations/radiance.h"
#include "../common.h"

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
  RadExporter exporter = RadExporter(model, "WILL NOT BE USED", false);
  
  if (!oconv(octreeName, &options, exporter))
    executionError(L, "Error while trying to create an octree");

  return 0;
}