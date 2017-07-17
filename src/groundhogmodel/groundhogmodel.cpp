#include "./groundhogmodel.h"
#include "../common/utilities/io.h"
#include "./src/layer.h"

GroundhogModel::GroundhogModel() {
	DEBUG_MSG("Creating GroundhogModel");
};

GroundhogModel::~GroundhogModel() {
	//destroy layers
	for (unsigned int i = 0; i < layers.size(); i++) {
		delete layers[i];
	}
	DEBUG_MSG("Destroying GroundhogModel");
}


void GroundhogModel::addLayer(std::string * layerName) {	
	layers.push_back( new Layer( layerName) );	
	DEBUG_MSG("Adding layer " + *layerName + " to model");
}

bool GroundhogModel::addFaceToLayer(std::string * layerName, Face * face) {
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {		
		if (layers[layerCount]->compareName(*layerName)) {
			DEBUG_MSG("Found layer "+*layerName);
			layers[layerCount]->addFace(face);
			return true;
		}
	}
	fatal("Layer " + *layerName + " could not be found");
	return false;
}

size_t GroundhogModel::getNumLayers() {
	return layers.size();
}


Layer * GroundhogModel::getLayerRef(unsigned int i) {
	return layers[i];
}