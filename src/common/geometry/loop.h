#pragma once

#include <vector>

#include "./point.h"

class Loop {
private:
	std::vector < Point3D * > vertices;
public:
	Loop();
	Loop(Loop * loop); // clone implementation
	~Loop();
	void addVertex(Point3D * point);
	size_t getNumVertices();
	Point3D * getVertex(size_t i);
};