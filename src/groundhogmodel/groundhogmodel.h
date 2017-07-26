#pragma once

#include <vector>


#include "./src/material.h"
#include "./src/layer.h"
#include "./src/componentdefinition.h"
#include "./src/view.h"
#include "../common/utilities/date.h"
#include "./src/location.h"

class GroundhogModel {

private:
	//Objectives // **
	std::vector <Layer *> layers;
	//Workplanes // **
	//Sensors // **
	//std::vector <Material *> materials;
	std::vector <View *> views;
	std::vector <ComponentDefinition *> definitions;
	//WindowGroups
	//Weather

	Location * location;

	Date * date;
	//Options
	//Observers // **	
	double northCorrection;

public:
	GroundhogModel();
	~GroundhogModel();
	void addLayer(std::string * layerName);

	bool addFaceToLayer(std::string * layerName, Face * face);
	bool addComponentInstanceToLayer(std::string * layerName, ComponentInstance * instance);

	void addFace(std::vector <Face *> * faces, Face * face);
	void addComponentInstance(std::vector <ComponentInstance *> * instances, ComponentInstance * instance);


	size_t getNumLayers();
	size_t getNumComponentDefinitions();
	Layer * getLayerRef(size_t i);
	ComponentDefinition * getComponentDefinitionRef(size_t i);
	void addComponentDefinition(ComponentDefinition * componentDefinition);
	ComponentDefinition *  getComponentDefinitionByName(std::string * definitionName);
	Layer *  getLayerByName(std::string * layerName);
	void addView(View * view);
	View * getViewRef(size_t i);
	size_t getNumViews();
	void setNorthCorrection(double nC);
	double getNorthCorrection();

	void setLatitude(double l);
	void setLongitude(double l);
	void setTimeZone(double l);
	void setCity(std::string c);
	std::string getCity();
	void setCountry(std::string c);
	std::string getCountry();

	void setMonth(int m);
	void setDay(int d);
	void setHour(int h);
	void setMinute(int min);

	double getLatitude();
	double getLongitude();
	double getTimeZone();
	int getMonth();
	int getDay();
	int getHour();
	int getMinute();
};