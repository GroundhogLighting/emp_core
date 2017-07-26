#pragma once

#include "./componentdefinition.h"
#include "./componentinstance.h"
#include "./face.h"

#include <vector>

class ComponentInstance;

class ComponentDefinition {

private:
	std::string name;
	std::vector <Face *> faces;
	//Workplanes // **
	//Sensors // **
	//std::vector <Material *> materials;	
	std::vector <ComponentInstance *> instances;
	//WindowGroups
	//Observers // **	

public:
	ComponentDefinition(std::string * componentName);
	~ComponentDefinition();
	void addFace(Face * face);
	size_t getNumFaces();
	std::string getName();
	Face * getFaceRef(size_t i);
	ComponentInstance * getComponentInstanceRef(size_t i);
	std::vector <Face * > * getFacesRef();
	std::vector <ComponentInstance * > * getComponentInstancesRef();
};