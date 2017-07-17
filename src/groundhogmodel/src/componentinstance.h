#pragma once

#include "./componentinstance.h"
#include "componentdefinition.h"
class ComponentInstance {
private:
	ComponentDefinition * definition;
	ComponentDefinition * parent; // if NULL, the parent is the model itself
	
};