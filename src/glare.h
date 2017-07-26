#pragma once

#include <SketchUpAPI/model/model.h>
#include "./groundhogmodel/groundhogmodel.h"

class Glare {

private:
	std::string usage = "usage v1: Glare inputFile outFile --> exports file\nusage v2: Glare inputFile --> performs standard calculation \nusage v3: Glare inputFile.lua --> performs custom calculations\n";
	
	std::string inputFile;
	std::string outputFile;
	
	bool verbose;

	GroundhogModel * model;

public:
	Glare();
	~Glare();
	
	bool solve();
	bool parseInputs(int argc, char* argv[]);
	bool loadFile(GroundhogModel * model, std::string inputFile, bool verbose);

}; // END OF GLARE CLASS