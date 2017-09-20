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


bool Glass::writeRadianceDefinition(std::string * dir)
{

	std::ofstream file;
	file.open(*dir + "/" + name + ".mat");

	file << "void" << GLARE_TAB << type << GLARE_TAB << name << std::endl;
	file << 0 << std::endl;
	file << 0 << std::endl;
	file << 3 << GLARE_TAB << r << GLARE_TAB << g << GLARE_TAB << b  << std::endl;

	file.close();

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