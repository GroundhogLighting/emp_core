#include "./layer.h"
#include "../../common/utilities/io.h"

Layer::Layer(std::string layerName) {
	DEBUG_MSG("Creating layer " + layerName);
	name = layerName;
};

Layer::~Layer() {
	// delete faces
	for (unsigned int i = 0; i < faces.size(); i++) {
		delete faces[i];
	}
	DEBUG_MSG("Destroying layer " + name);

};

std::string Layer::getName() {
	return name;
}

bool Layer::compareName(std::string layerName) {
	return name == layerName;
}

void Layer::addFace(Face * face) {
	faces.push_back(face);
}

std::vector <Face * > * Layer::getFacesRef() {
	return &faces;
}


Face * Layer::getFaceRef(size_t i) {
	return faces[i];
}