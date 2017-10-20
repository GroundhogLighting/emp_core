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

#include "./glare.h"
#include "./common/utilities/io.h"
#include "./common/utilities/stringutils.h"
#include "./common/utilities/file.h"

#include "readers/skp/SKPreader.h"
#include "writers/rad/radexporter.h"
#include "common/tasks/task.h"
#include "api/api.h"

// Include LUA headers
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h> 
}


#define PRINT_USAGE std::cerr << usage << std::endl;

Glare::Glare() 
{
	DEBUG_MSG("Creating Glare object");	
}

Glare::~Glare() 
{
	DEBUG_MSG("Destroying Glare object");	
}



bool Glare::parseInputs(int argc, char* argv[]) 
{
	if (argc == 2) {
		// Input file... calculation will be performed.
		// call in the shape of 'Glare inputFile'
		inputFile = std::string(argv[1]);
	}
	else if (argc == 3) {
		inputFile = std::string(argv[1]);
		secondArgument = std::string(argv[2]);		
	}
	else { // input error
		PRINT_USAGE
		return false;
	}

	// Check if input file makes sense.
	char * supportedInputs[] = { ".skp" };
	if (!stringIncludeAny(inputFile, supportedInputs,2)) {
		fatal("Only .SKP input files are supported for now", __LINE__, __FILE__);
		return false;
	}


	
	return true;
} // END OF PARSE INPUTS



class ExportRadianceDir : public Task {

public:

	std::string target;
	GroundhogModel * model;
	bool verbose;

	ExportRadianceDir(std::string dir, GroundhogModel * ghmodel, bool verb)
	{
		setName("Export model in Radiance format in directory '"+dir+"'");	

		target = dir;
		model = ghmodel;
		verbose = verb;
	}

	bool isEqual(Task * t)
	{
		return target == static_cast<ExportRadianceDir *>(t)->target;		
	}

	bool solve()
	{
		RadExporter r = RadExporter(model, target, verbose);
		return r.exportModel();
	}
};

bool Glare::solve() 
{
	
	// verify that inputFile exists
	if (!fexists(inputFile)) {
		fatal("File '" + std::string(inputFile) + "' not found", __LINE__, __FILE__);
		return false;
	}

	// Load file
	loadFile(inputFile);

	// Analize second argument
	if (secondArgument.empty()) {
		// Process and return... get tasks from model

	} 
	else if (stringInclude(secondArgument, ".lua")) {
		// process lua file... get tasks from script

		// check if script exists
		if (!fexists(secondArgument)) {
			fatal("Lua script '" + std::string(secondArgument) + "' not found", __LINE__, __FILE__);
			return false;
		}

		// Process LUA script
		int status, result;

		// Create lua state
		lua_State * L = luaL_newstate();

		// Open libraries
		luaL_openlibs(L);

		// Load API
		loadAPI(L,&model);

		// Load script
		status = luaL_loadfile(L, secondArgument.c_str());
		if (status) {
			fatal("Error when reading script file '"+secondArgument+"'", __LINE__, __FILE__);
			return false;
		}

		result = lua_pcall(L, 0, LUA_MULTRET, 0);
		if (result) {
			fatal("Error when executing script file '" + secondArgument + "'", __LINE__, __FILE__);
			return false;
		}


	}
	else {
		// translate
		if (!stringInclude(secondArgument, ".")) {
			// Radiance format... no extension
			taskManager.addTask(new ExportRadianceDir(secondArgument,&model,verbose));
			taskManager.solve();
		}
		else {
			fatal("Unrecognized file extension in " + secondArgument, __LINE__, __FILE__);
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
			fatal("Could not read file '" + std::string(input) + "'", __LINE__, __FILE__);
			return false;
		}
	}
	return true;
}

		

