#include "./plastic.h"

#include "config_constants.h"
#include "common/utilities/io.h"
#include <fstream>

Plastic::Plastic(json j)
{
	DEBUG_MSG("Creating Plastic");
	primitiveLength = 11;
	fillFromJSON(j);
}


double Plastic::red()
{
	return r;
}

double Plastic::green()
{
	return g;
}

double Plastic::blue()
{
	return b;
}

double Plastic::spec()
{
	return specularity;
}

double Plastic::rough()
{
	return roughness;
}


bool Plastic::writeRadianceDefinition(std::string * dir)
{

	std::ofstream file;
	file.open(*dir + "/" + name + ".mat");

	file << "void" << GLARE_TAB << type << GLARE_TAB << name << std::endl;
	file << 0 << std::endl;
	file << 0 << std::endl;
	file << 5 << GLARE_TAB << r << GLARE_TAB << g << GLARE_TAB << b << GLARE_TAB << specularity << GLARE_TAB << roughness << std::endl;

	file.close();

	return true;
}

bool Plastic::parsePrimitive(std::vector <std::string> * tokens)
{
	size_t i = 6;
	r = std::stod((*tokens)[i++]);
	g = std::stod((*tokens)[i++]);
	b = std::stod((*tokens)[i++]);
	specularity = std::stod((*tokens)[i++]);
	roughness = std::stod((*tokens)[i++]);
	return true;
}