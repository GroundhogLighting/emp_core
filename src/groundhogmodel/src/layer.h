#pragma once

#include <string>
#include <vector>

#include "../src/face.h"

class Layer{

private:
	std::string name;
	std::vector <Face * > faces;

public:
	Layer(std::string * layerName);
	~Layer();
	bool compareName(std::string layerName);
	void addFace(Face * face);
	std::string getName();
	std::vector <Face * > * getFacesRef();
	Face * getFaceRef(size_t i);

};