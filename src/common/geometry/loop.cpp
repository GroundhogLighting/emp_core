#include "./loop.h"
#include "../utilities/io.h"

Loop::Loop() {
	DEBUG_MSG("Creating loop");
	//vertices.reserve(3);
	vertices = std::vector < Point3D * >();
}

Loop::Loop(Loop * loop) {
	vertices = std::vector < Point3D * >();
	size_t numVertices = loop->getNumVertices();
	for (int i = 0; i < numVertices; i++) {
		addVertex(new Point3D(loop->getVertex(i)));
	}
}

Loop::~Loop() {

	for (unsigned i = 0; i < vertices.size(); i++) {
		delete vertices[i];
	}

	DEBUG_MSG("Destroying loop");
}

void Loop::addVertex(Point3D * point) {
	vertices.push_back(point);
}


size_t Loop::getNumVertices() {
	return vertices.size();
}


Point3D * Loop::getVertex(size_t i) {
	return vertices[i];
}


