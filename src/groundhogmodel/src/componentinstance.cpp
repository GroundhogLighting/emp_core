#include "./componentinstance.h"
#include "../../common/utilities/io.h"


ComponentInstance::ComponentInstance(ComponentDefinition * definitionRef) {
	DEBUG_MSG("Creating component instance");
	definition = definitionRef;	
	rotationX = 0; 
	rotationY = 0; 
	rotationZ = 0; 

	scale = 1; 

	x = 0; 
	y = 0; 
	z = 0; 
}


ComponentInstance::~ComponentInstance() {
	DEBUG_MSG("Destroying component instance");	
}

void ComponentInstance::setX(double newX) {
	x = newX;
}

void ComponentInstance::setY(double newY) {
	y = newY;
}

void ComponentInstance::setZ(double newZ) {
	z = newZ;
}

double ComponentInstance::getX() {
	return x;
}

double ComponentInstance::getY() {
	return y;
}

double ComponentInstance::getZ() {
	return z;
}


void ComponentInstance::setRotationX(double newRotationX) {
	rotationX = newRotationX;
}

void ComponentInstance::setRotationY(double newRotationY) {
	rotationY = newRotationY;
}

void ComponentInstance::setRotationZ(double newRotationZ) {
	rotationZ = newRotationZ;
}

double ComponentInstance::getRotationX() {
	return rotationX;
}

double ComponentInstance::getRotationY() {
	return rotationY;
}

double ComponentInstance::getRotationZ() {
	return rotationZ;
}

void ComponentInstance::setScale(double s) {
	scale = s;
}

double ComponentInstance::getScale() {
	return scale;
}

ComponentDefinition * ComponentInstance::getDefinitionRef() {
	return definition;
}