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


bool Plastic::writeInRadianceFormat(FILE * file)
{

    fprintf(file, "void %s %s\n0\n0\n", &type[0], &name[0]);
    fprintf(file, "5 %f %f %f %f %f\n", r, g, b, specularity, roughness);

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