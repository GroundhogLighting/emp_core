#pragma once

#include <string>
#include <vector>

#include "../src/face.h"
#include "../src/componentinstance.h"

class Layer{

private:
	std::string name;
	std::vector <Face * > faces;
	std::vector <ComponentInstance * > instances;

public:
	Layer(std::string * layerName);
	~Layer();
	bool compareName(std::string layerName);
	void addFace(Face * face);
	std::vector <ComponentInstance * > * getComponentInstancesRef();
	std::string getName();
	std::vector <Face * > * getFacesRef();
	Face * getFaceRef(size_t i);
	ComponentInstance * getComponentInstanceRef(size_t i);
	bool isEmpty();
};