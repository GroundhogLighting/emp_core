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


#include <string>

#include "config_constants.h"

#include "./emp.h"
#include "./common/utilities/io.h"
#include "./common/utilities/stringutils.h"
#include "./common/utilities/file.h"

#ifndef AVOID_SKP
    #include "readers/skp/SKPreader.h"
#endif

#include "api/api.h"
#include "writers/rad/tasks.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}



bool Emp::parseInputs(int argc, char* argv[]) 
{	
	// Input file and lua script.
	inputFile = std::string(argv[1]);
	script = std::string(argv[2]);			

	// Check if inputFile makes sense.
    const char * supportedInputs[] = {
#ifndef AVOID_SKP
        ".skp"
#endif        
    };
	if (!stringIncludeAny(inputFile, supportedInputs,2)) {
		FATAL(errorMessage,"Only .SKP input files are supported for now");
		return false;
	}

	// verify that inputFile exists
	if (!fexists(inputFile)) {
		FATAL(errorMessage,"File '" + std::string(inputFile) + "' not found");
		return false;
	}

    /* SEARCH FOR SCRIPT */
    // add the .lua if needed
    if (!stringInclude(script, ".lua")) {
      script = script + ".lua";
    }

    // check if script exists
    if (!fexists(script)) {
      // if it does not exist, we look into the EMPATH
      if (const char * glarepath = std::getenv(EMPATH)) {
        if (fexists(std::string(glarepath) + "/" + script)) {
          script = std::string(glarepath) + "/" + script;
          return true;
        }
        else {
          FATAL(errorMessage, "Lua script '" + std::string(script) + "' not found (not even in "+ EMPATH +")");
          return false;
        }
      } else { // if there is no GLAREPATH variable, just error.
        FATAL(errorMessage, "Lua script '" + std::string(script) + "' not found");
        return false;
      }
    }
	
	return true;
} // END OF PARSE INPUTS


bool Emp::solve(int argc, char* argv[])
{
		
	/* LOAD FILE */
	loadFile(inputFile);
		
    /* LOAD API */
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
	status = luaL_loadfile(L, &script[0]);
	if (status) {
		std::cerr <<  lua_tostring(L, -1) << std::endl;
		return false;
	}

    // Solve script
	result = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (result) {
		std::cerr << lua_tostring(L, -1) << std::endl;
		return false;
	}

	// Autosolve?
	bool autoSolve = true; // defaults to true
	lua_getglobal(L, LUA_AUTOSOLVE_VARIABLE);
	// Check type
	if (lua_type(L, 1) == LUA_TBOOLEAN) {
		autoSolve = lua_toboolean(L, 1);
	}

    // solve if required
	if (autoSolve) {
      json results = json();
	  taskManager.solve(&results);
      std::cout << results;
	}
	
	return true;
};



bool Emp::loadFile(std::string input) 
{
#ifndef AVOID_SKP
	// inputFile is a Sketchup model
	if (stringInclude(input, ".skp")) {
		SKPReader reader(&model,verbose);
		if (!reader.parseSKPModel(input)) {
			FATAL(errorMessage,"Could not read file '" + std::string(input) + "'");
			return false;
		}
	}
#endif
	return true;
}

		

