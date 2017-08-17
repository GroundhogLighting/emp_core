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
#include "./readers/skp/SKPreader.h"

#include "./writers/rad/radexporter.h"

#define PRINT_USAGE std::cerr << usage << std::endl;

Glare::Glare() 
{
	DEBUG_MSG("Creating Glare object");
	verbose = true;
	//model = NULL;
	model = new GroundhogModel();
}

Glare::~Glare() 
{
	DEBUG_MSG("Destroying Glare object");
	delete model;
}



bool Glare::parseInputs(int argc, char* argv[]) 
{
	if (argc == 2) {
		// Input file... calculation will be performed.
		// call in the shape of 'Glare inputFile'
		inputFile = std::string(argv[1]);
	}
	else if (argc == 3) {
		// export... call in the shape of 
		// 'Glare inputFile outFile
		inputFile = std::string(argv[1]);
		outputFile = std::string(argv[2]);
	}
	else { // input error
		PRINT_USAGE
		return false;
	}

	// Check if input file makes sense.

	char * supportedInputs[] = { ".skp",".lua" };
	if (!stringIncludeAny(inputFile, supportedInputs,2)) {
		fatal("Only .SKP and .LUA input files are supported for now", __LINE__, __FILE__);
		return false;
	}

	
	return true;
} // END OF PARSE INPUTS




bool Glare::solve() 
{
	
	// verify that inputFile exists
	if (!fexists(inputFile)) {
		fatal("File '" + std::string(inputFile) + "' not found", __LINE__, __FILE__);
		return false;
	}

	// Check what kind of process is wanted
	if (!stringInclude(inputFile, ".lua")) {
		/* STANDARD CALCULATION */
		// load model
		loadFile(model, inputFile, verbose);

		/* CHECK IF JUST EXPORT */
		if (!outputFile.empty()) {
			if (stringInclude(outputFile, ".ghm")) {
				fatal("Exporting .GHM file is not yet supported", __LINE__, __FILE__);
				return false;
			}
			else { // no extension, thus: Radiance			
				RadExporter * writer = new RadExporter(model,outputFile,verbose);
				bool success = writer -> exportModel();
				delete writer;
				return success;
			}
		}

		fatal("calculations are not yet supported", __LINE__, __FILE__);
		return false;
	}
	else {
		// In other case, process the lua script
		fatal("The Lua API is not yet supported.", __LINE__, __FILE__);
		return false;
	}

	return true;
};



bool Glare::loadFile(GroundhogModel * model, std::string input, bool verbose) 
{
	// inputFile is a Sketchup model
	if (stringInclude(input, ".skp")) {
		SKPReader reader;
		if (!reader.parseSKPModel(input, model, verbose)) {
			fatal("Could not read file '" + std::string(input) + "'", __LINE__, __FILE__);
			return false;
		}
	}
	return true;
}

		

