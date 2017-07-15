#pragma once

#include <SketchUpAPI/model/model.h>
#include "./groundhogmodel/groundhogmodel.h"

class Glare {

private:
	std::string inputFile;
	bool verbose;
	bool doExport;
	std::string outPath;
	GroundhogModel model;

public:
	Glare();
	~Glare();
	
	bool solve();
	bool parseInputs(int argc, char* argv[]);
	
}; // END OF GLARE CLASS