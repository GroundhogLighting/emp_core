#include "componentdefinition.h"
#include "../../common/utilities/io.h"

#include <string>

ComponentDefinition::ComponentDefinition(std::string * componentName) 
{
	name = *componentName;
	DEBUG_MSG("Creating component " + *componentName);
}


ComponentDefinition::~ComponentDefinition() 
{
	DEBUG_MSG("Destroying component " + name);

	for (size_t i = 0; i < faces.size(); i++) {
		delete faces[i];
	}
	
	for (size_t i = 0; i < instances.size(); i++) {
		delete instances[i];
	}
}


void ComponentDefinition::addFace(Face * face) 
{
	DEBUG_MSG("ADDING FACE TO" + name);
	faces.push_back(face);
}

size_t ComponentDefinition::getNumFaces() 
{
	return faces.size();
}


std::string ComponentDefinition::getName() 
{
	return name;
}


Face * ComponentDefinition::getFaceRef(size_t i) 
{
	return faces[i];
}


ComponentInstance * ComponentDefinition::getComponentInstanceRef(size_t i) 
{
	return instances[i];
}

std::vector <Face * > * ComponentDefinition::getFacesRef() 
{
	return &faces;
}

std::vector <ComponentInstance * > * ComponentDefinition::getComponentInstancesRef() 
{
	return &instances;
}