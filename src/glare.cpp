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


#include <string>

#include "mem_leak.h"
#include "config_constants.h"

#include "./glare.h"
#include "./common/utilities/io.h"
#include "./common/utilities/stringutils.h"
#include "./common/utilities/file.h"

#include "readers/skp/SKPreader.h"
#include "api/api.h"

#include "common/taskmanager/tasks/export.h"

#include "Lua/src/lua.hpp"





bool Glare::parseInputs(int argc, char* argv[]) 
{
	if (argc == 1) {
		/* Only input is program name. i.e. no inputs... wrong */
		std::cout << USAGE << std::endl;
		return false;
	}else if (argc == 2) {
		// Input file... calculation will be performed.
		// call in the shape of 'Glare inputFile'
		inputFile = std::string(argv[1]);		
	}
	else {
		// Input file and at least one more.
		inputFile = std::string(argv[1]);
		secondArgument = std::string(argv[2]);		
	}

	// Check if inputFile makes sense.
	char * supportedInputs[] = { ".skp" };
	if (!stringIncludeAny(inputFile, supportedInputs,2)) {
		FATAL(errorMessage,"Only .SKP input files are supported for now");
		return false;
	}

	// verify that inputFile exists
	if (!fexists(inputFile)) {
		FATAL(errorMessage,"File '" + std::string(inputFile) + "' not found");
		return false;
	}


	
	return true;
} // END OF PARSE INPUTS


bool Glare::solve(int argc, char* argv[])
{
	
	
	// Load file
	loadFile(inputFile);

	// Analize second argument
	if (secondArgument.empty()) {				
      FATAL(errorMessage,"Solving a model is not yet supported!");
	  return false;		

	} else if (stringInclude(secondArgument, ".lua")) {
		// Lua script was input... process

		// check if script exists
		if (!fexists(secondArgument)) {
			FATAL(errorMessage,"Lua script '" + std::string(secondArgument) + "' not found");
			return false;
		}

        // Create task dictionary
        std::map<std::string, TaskFactory> taskDictionary;

		// Process LUA script
		int status, result;

		// Create lua state
		lua_State * L = luaL_newstate();

		// Open libraries
		luaL_openlibs(L);

		// Load API
		loadAPI(L,&model, &taskDictionary, &taskManager,argc,argv);

		// Load script
		status = luaL_loadfile(L, secondArgument.c_str());
		if (status) {
			std::cerr <<  lua_tostring(L, -1) << std::endl;
			return false;
		}

		result = lua_pcall(L, 0, LUA_MULTRET, 0);
		if (result) {
			std::cerr << lua_tostring(L, -1) << std::endl;
			return false;
		}

		// Autosolve?
		bool autoSolve = true;

		lua_getglobal(L, LUA_AUTOSOLVE_VARIABLE);
		// Check type
		if (lua_type(L, 1) == LUA_TBOOLEAN) {
			autoSolve = lua_toboolean(L, 1);
		}

		if (autoSolve) {
			taskManager.solve();
		}


	}
	else {
		// translate
		if (!stringInclude(secondArgument, ".")) {
			// Radiance format... no extension
          ExportRadianceDirWithWorkplanes * task = new ExportRadianceDirWithWorkplanes(secondArgument, &model, verbose);
          taskManager.addTask(task);
          taskManager.solve();          
		}
		else {
			FATAL(errorMessage,"Unrecognized file extension in " + secondArgument);
			return false;
		}
	}

	return true;
};



bool Glare::loadFile(std::string input) 
{
	// inputFile is a Sketchup model
	if (stringInclude(input, ".skp")) {
		SKPReader reader(&model,verbose);
		if (!reader.parseSKPModel(input)) {
			FATAL(errorMessage,"Could not read file '" + std::string(input) + "'");
			return false;
		}
	}
	return true;
}

		

