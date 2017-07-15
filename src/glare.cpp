//#include <SketchUpAPI/initialize.h>
//#include <SketchUpAPI/model/model.h>

#include <string>

#include "./glare.h"
#include "./common/utilities/io.h"
#include "./common/utilities/stringutils.h"
#include "./common/utilities/file.h"
#include "./readers/skp/SKPreader.h"

#include "./writers/rad/radexporter.h"

Glare::Glare() {
	DEBUG_MSG("Creating Glare object");
	verbose = true;
	doExport = false;
}

Glare::~Glare() {
	DEBUG_MSG("Destroying Glare object");
}

bool Glare::solve() {

	// Check if any input file
	if (inputFile.empty()) {
		fatal("No input file was given.");
		return false;
	}
	// verify that inputFile exists
	if (!fexists(inputFile)) {
		fatal("File '" + std::string(inputFile) + "' not found");
		return false;
	}

	// PARSE FILE
	// inputFile is a Sketchup model
	if (stringInclude(inputFile,".skp")) {
		SKPReader reader;
		if (!reader.parseSKPModel(inputFile, &model, verbose)) {
			fatal("Could not read file '" + std::string(inputFile) + "'");
			return false;
		}
	}

	// EXPORT IF REQUIRED
	if (doExport) {
		if (stringInclude(outPath,".ghm")) {
			fatal("Exporting Groundhog Model is not yet supported");
			return false;
		}
		else { // no extension, thus: Radiance			
			RadExporter writer;
			writer.setExportDir(outPath);
			return writer.exportModel(&model, verbose);
		}		
	}

	// CALCULATE


	// POST PROCESS
	

	return true;
};


bool Glare::parseInputs(int argc, char* argv[]) {
	// If nothing was given... return instructions.	
	if (argc == 1) {
		inform("No input.... TODO: print usage", true);
		return false;
	}

	// Else, process inputs
	for (int i = 1; i < argc; i++) {

		//transform into string
		std::string sArg = std::string(argv[i]);

		// Found an input file (only SKP supported for now)
		char * supportedExtensions[] = { ".skp" };
		if (stringIncludeAny(argv[i], supportedExtensions, 1)) {
			inputFile = argv[i];			
			continue;
		}// end of inputfile process		

			// check if export
		if (sArg.compare("-e") == 0) {
			doExport = true;
			outPath = std::string(argv[++i]);
			continue;
		}


		// check if verbose
		if (sArg.compare("-v") == 0) {
			verbose = false;
			continue;
		}

		warn("Unkown argument '" + std::string(argv[i]) + "'");
	} // END OF ITERATE INPUTS

	return true;
} // END OF PARSE INPUTS
		

