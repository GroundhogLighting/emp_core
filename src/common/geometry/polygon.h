#pragma once

#include <vector>

#include "./point.h"
#include "./vector.h"
#include "./loop.h"

class Polygon3D {

private:
	Loop * outerLoop;
	std::vector < Loop * > innerLoops;
	double area;

public:
	Polygon3D();
	~Polygon3D();
	double getArea();
	void setArea(double area);
	Loop * getOuterLoopReference();
	Loop * addInnerLoop();
	bool hasInnerLoops();
	Loop * getOuterLoopRef();
	size_t countInnerLoops();
	Loop * getClosedLoop();
};