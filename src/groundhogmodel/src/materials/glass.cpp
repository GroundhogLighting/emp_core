#include "common/utilities/io.h"
#include "./glass.h"
#include <fstream>
#include "config_constants.h"


Glass::Glass(json j)
{
	DEBUG_MSG("Creating Glass");
	primitiveLength = 9;
	fillFromJSON(j);
}

double Glass::red()
{
	return r;
}

double Glass::green()
{
	return g;
}

double Glass::blue()
{
	return b;
}


bool Glass::writeInRadianceFormat(FILE * file)
{

    fprintf(file, "void %s %s\n0\n0\n", &type[0], &name[0]);
	fprintf(file, "3 %f %f %f", r, g, b);
	
	return true;
}


bool Glass::parsePrimitive(std::vector <std::string> * tokens)
{	
	size_t i = 6;
	r = std::stod((*tokens)[i++]);
	g = std::stod((*tokens)[i++]);
	b = std::stod((*tokens)[i++]);
	
	return true;
}