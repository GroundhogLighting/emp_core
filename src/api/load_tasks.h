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

#include "Lua/src/lua.hpp"

#include "./tasks.h"

//! Registers all the available tasks in the API
/*!
@author German Molina
@param[in] L The lua_State * object
*/
void registerTasks(lua_State * L)
{
  registerTask(L, "workplane_illuminance", workplaneIlluminanceFactory);


  registerTask(L, "export", exportModel);

}