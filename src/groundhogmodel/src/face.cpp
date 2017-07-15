#include "./face.h"

#include "../../common/utilities/io.h"

#define TOO_MANY_LOOPS 40

Face::Face(std::string faceName) {
	DEBUG_MSG("Creating face " + faceName);

	name = faceName;


}

Face::~Face() {

	//destroy polygon
	delete polygon;
	
	DEBUG_MSG("Destroying face "+name);
}


std::string Face::getName() {
	return name;
}


void Face::setPolygon(Polygon3D * newPolygon) {
	polygon = newPolygon;
}

bool Face::hasInnerLoops() {
	return polygon->hasInnerLoops();
}

bool Face::hasTooManyInnerLoops() {
	return polygon->countInnerLoops() > TOO_MANY_LOOPS;
}

Loop * Face::getOuterLoopRef() {
	return polygon->getOuterLoopRef();
}

Loop * Face::getClosedLoop() {
	return polygon->getClosedLoop();
}