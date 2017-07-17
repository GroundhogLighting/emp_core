#pragma once

#include "./componentdefinition.h"
#include "./face.h"

#include <vector>

class ComponentDefinition {

private:
	std::string name;
	std::vector <Face *> faces;
	//Workplanes // **
	//Sensors // **
	//std::vector <Material *> materials;	
	//Component Instances
	//WindowGroups
	//Observers // **	

};