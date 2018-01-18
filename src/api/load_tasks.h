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

#include "./tasks.h"

//! Registers all the available tasks in the API
/*!
@author German Molina
@param[in] L The lua_State * object
*/
void registerTasks(lua_State * L)
{
    registerTask(L, "workplane_illuminance", workplaneIlluminanceFactory);

    registerTask(L, "write_scene_file",writeRadSceneFile);
    
    registerTask(L,"write_model_info",writeModelInfo);
    
    registerTask(L,"write_rif_file",writeRadRifFile);
    
    registerTask(L,"write_components",writeComponentDefinitions);
    
    registerTask(L,"write_views",writeViews);
    
    registerTask(L,"write_current_sky",writeCurrentSky);
    
    registerTask(L,"write_current_weather",writeCurrentWeather);

    registerTask(L,"write_materials",writeMaterials);
    
    registerTask(L,"write_layers",writeLayers);

    registerTask(L,"write_photosensors",writePhotosensors);

    registerTask(L,"write_workplane",writeWorkplane);
}
