#pragma once

#include <string>
#include "./face.h"
#include "../../common/geometry/polygon.h"

class Face {
private:
	std::string name;
	//Material
	Polygon3D * polygon;

public:
	Face(std::string faceName);
	~Face();
	std::string getName();
	void setPolygon(Polygon3D * newPolygon);
	bool hasInnerLoops();
	bool hasTooManyInnerLoops();
	Loop * Face::getOuterLoopRef();
	Loop * getClosedLoop();
};