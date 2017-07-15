#pragma once

#include <vector>

//#include "../common/geometry/point.h"

#include "./src/material.h"
//#include "./src/view.h"
#include "./src/layer.h"

class GroundhogModel {

private:
	//Objectives // **
	std::vector <Layer *> layers;
	//Workplanes // **
	//Sensors // **
	//std::vector <Material *> materials;
	//Views
	//Component Definitions
	//Component Instances
	//WindowGroups
	//Weather
	//Date
	//Options
	//Observers // **
	//Location
public:
	GroundhogModel();
	~GroundhogModel();
	void addLayer(std::string layerName);
	bool addFaceToLayer(std::string layerName, Face * face);
	size_t getNumLayers();
	Layer * getLayerRef(unsigned int i);	
};