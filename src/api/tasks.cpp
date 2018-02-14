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

#include "./tasks.h"

#include "calculations/tasks/RTraceTask.h"
#include "calculations/tasks/OconvTask.h"
#include "writers/rad/tasks.h"

Task * workplaneIlluminanceFactory(lua_State * L)
{

  OconvOptions oconvOptions = OconvOptions();

  GroundhogModel * model = getCurrentModel(L);
  RTraceOptions * rtraceOptions = model->getRTraceOptions();  

  OptionSet otherOptions = OptionSet();
  otherOptions.addOption("workplane", "none");
  
  // RTRace options are obtained from the global options
  otherOptions.fillFromLuaTable(L, 1);
  oconvOptions.fillFromLuaTable(L, 1);

  // Check that the workplane exists
  std::string wpName = otherOptions.getOption<std::string>("workplane");
  Workplane * wp = getWorkplane(L,wpName);


  RTraceTask * res = new RTraceTask(model, rtraceOptions, wp, &oconvOptions);
  

  return res;
}


Task * writeRadSceneFile(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("filename","scene.rad"); // The name of the scene file
    options.addOption("layers_directory","Geometry"); // The directory where the Layers are located
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteRadianceSceneFile(options.getOption<std::string>("filename"), model, &options);
    
}


Task * writeRadRifFile(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("filename","scene.rif"); // The name of the RIF file
    options.addOption("layers_directory","Geometry"); // The directory where the Layers are located
    options.addOption("views_directory","Views"); // The directory where the Views are located
    options.addOption("skies_directory","Skies"); // The directory where the Skies and Weathers are located
    options.addOption("materials_directory","Materials"); // The directory where the Materials are located
    options.addOption("materials_file","materials.mat"); // The name of the file that references all materials
    options.addOption("scene_file","scene.rad"); // The name of the scene file
    options.addOption("components_directory","Components"); // The directory where the Components are located
    options.addOption("windows_directory","Windows"); // The directory where the Windows are located
    options.addOption("illums_directory","Illums"); // The directory where the Illums are located
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteRadianceRifFile(options.getOption<std::string>("filename"), model, &options);
    
}



Task * writeModelInfo(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("filename","modelinfo.txt"); // The file to write the model info.
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteModelInfo(options.getOption<std::string>("filename"),model);
    
}


Task * writeComponentDefinitions(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Components"); // The directory to write the Components
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteComponentDefinitions(options.getOption<std::string>("directory"),model);
    
}


Task * writeViews(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Views"); // The directory to write the Views
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteViews(options.getOption<std::string>("directory"),model);
    
}


Task * writeCurrentSky(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Skies"); // The directory to write the Views
    options.addOption("filename","Sky.rad"); // The name of the file
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteCurrentSky(options.getOption<std::string>("directory"), options.getOption<std::string>("filename"),model);
}



Task * writeCurrentWeather(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Skies"); // The directory to write the weather
    options.addOption("filename","Sky.rad"); // The name of the file
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteCurrentWeather(options.getOption<std::string>("directory"), options.getOption<std::string>("filename"),model);
}


Task * writeMaterials(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Materials"); // The directory to write the Materials
    options.addOption("filename","materials.mat"); // The name of the file that references all Materials
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteMaterials(options.getOption<std::string>("directory"), options.getOption<std::string>("filename"),model);
}



Task * writeLayers(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Geometry"); // The directory to write the Materials
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WriteLayers(options.getOption<std::string>("directory"), model);
}



Task * writePhotosensors(lua_State * L)
{
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Photosensors"); // The directory to write the Materials
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
    
    // Get current model
    GroundhogModel * model = getCurrentModel(L);
    
    // Build the task
    return new WritePhotosensors(options.getOption<std::string>("directory"), model);
}


Task * writeWorkplane(lua_State * L)
{    
    // Build options
    OptionSet options = OptionSet();
    options.addOption("directory","Workplanes"); // The directory to write the Materials
    options.addOption("workplane","none"); // The name of the workplane to write
    options.addOption("max_area",0.25); // The maximum area for each pixel in the workplane
    options.addOption("max_aspect_ratio",1.3); // The maximum aspect ratio for each pixel in the workplane
    options.addOption("filename","none"); // The name of the resulting file
    
    // Fill the options
    options.fillFromLuaTable(L, 1);
        
    // Get the workplane
    Workplane * wp = getWorkplane(L,options.getOption<std::string>("workplane"));
    
    double maxArea = options.getOption<double>("max_area");
    double maxAspectRatio = options.getOption<double>("max_aspect_ratio");
    std::string name = options.getOption<std::string>("filename");
    
    if(name == "none")
        name = *(wp->getName());
    
    // Build the task
    return new WriteWorkplane(wp, maxArea, maxAspectRatio, options.getOption<std::string>("directory") + "/" +name);
}

