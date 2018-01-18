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

#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "common/taskmanager/task.h"
#include "api/common.h"

//! Register an RTraceTask
/*!
Allows calculating the luminance or illuminance of a scene by using a certain sky

@author German Molina
@param[in] L The lua_State * object
@return The task
*/
Task * workplaneIlluminanceFactory(lua_State * L);

//! Writes the Radiance scene file
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeRadSceneFile(lua_State * L);


//! Writes the Radiance RIF file
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeRadRifFile(lua_State * L);


//! Writes the model information
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeModelInfo(lua_State * L);

//! Writes the ComponentDefinition objects
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeComponentDefinitions(lua_State * L);


//! Writes the View objects
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeViews(lua_State * L);

//! Writes the current sky 
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeCurrentSky(lua_State * L);


//! Writes the current weather
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeCurrentWeather(lua_State * L);


//! Writes all the materials in the model
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeMaterials(lua_State * L);


//! Writes all the layers in the model
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeLayers(lua_State * L);


//! Writes all the photosensors in the model
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writePhotosensors(lua_State * L);

//! Writes a workplane
/*!
 @author German Molina
 @param[in] L The lua_State * object
 @return The task
 */
Task * writeWorkplane(lua_State * L);



