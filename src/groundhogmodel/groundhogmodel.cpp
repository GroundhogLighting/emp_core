#include "./groundhogmodel.h"
#include "../common/utilities/io.h"



GroundhogModel::GroundhogModel()
{
	DEBUG_MSG("Creating GroundhogModel");
	layers = std::vector<Layer *>();
	definitions = std::vector<ComponentDefinition *>();
	views = std::vector<View *>();
	date = new Date(1, 1, 12, 0);
	location = new Location();
};

GroundhogModel::~GroundhogModel()
{
	//destroy layers
	for (size_t i = 0; i < layers.size(); i++) {
		delete layers[i];
	}

	for (size_t i = 0; i < definitions.size(); i++) {
		delete definitions[i];
	}

	for (size_t i = 0; i < views.size(); i++) {
		delete views[i];
	}

	delete date;
	delete location;

	DEBUG_MSG("Destroying GroundhogModel");
}


void GroundhogModel::addLayer(std::string * layerName)
{	
	layers.push_back( new Layer( layerName) );	
	DEBUG_MSG("Adding layer " + *layerName + " to model");
}

bool GroundhogModel::addFaceToLayer(std::string * layerName, Face * face)
{
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {		
		if (layers[layerCount]->compareName(*layerName)) {
			DEBUG_MSG("Found layer "+*layerName);
			addFaceToVector(layers[layerCount]->getFacesRef(), face);
			return true;
		}
	}
	fatal("Layer " + *layerName + " could not be found");
	return false;
}


bool GroundhogModel::addComponentInstanceToLayer(std::string * layerName, ComponentInstance * instance)
{
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {
		if (layers[layerCount]->compareName(*layerName)) {
			DEBUG_MSG("Found layer " + *layerName);
			addComponentInstanceToVector(layers[layerCount]->getComponentInstancesRef(), instance);
			return true;
		}
	}
	fatal("Layer " + *layerName + " could not be found");
	return false;
}

void GroundhogModel::addFaceToVector(std::vector <Face *> * faces, Face * face)
{
	faces->push_back(face);
}

void GroundhogModel::addComponentInstanceToVector(std::vector <ComponentInstance *> * instances, ComponentInstance * instance)
{
	instances->push_back(instance);
}


size_t GroundhogModel::getNumLayers()
{
	return layers.size();
}

size_t GroundhogModel::getNumComponentDefinitions()
{
	return definitions.size();
}

Layer * GroundhogModel::getLayerRef(size_t i)
{
	return layers[i];
}

ComponentDefinition * GroundhogModel::getComponentDefinitionRef(size_t i)
{
	return definitions[i];
}

void GroundhogModel::addComponentDefinition(ComponentDefinition * componentDefinition)
{
	definitions.push_back(componentDefinition);
	DEBUG_MSG("Adding component definition to model");
}


ComponentDefinition *  GroundhogModel::getComponentDefinitionByName(std::string * definitionName)
{
	for (size_t i = 0; i < definitions.size(); i++) {
		if (*definitionName == definitions[i]->getName()) {
			DEBUG_MSG("Found definition" + *definitionName);
			return definitions[i];
		}
	}
	fatal("Component Definition " + *definitionName + " could not be found");
	return NULL;
}

Layer *  GroundhogModel::getLayerByName(std::string * layerName)
{
	for (size_t i = 0; i < layers.size(); i++) {
		if (*layerName == layers[i]->getName()) {
			DEBUG_MSG("Found layer" + *layerName);
			return layers[i];
		}
	}
	fatal("Layer " + *layerName + " could not be found");
	return NULL;
}

void GroundhogModel::addView(View * view)
{
	views.push_back(view);
}

View * GroundhogModel::getViewRef(size_t i)
{
	return views[i];
}

size_t GroundhogModel::getNumViews()
{
	return views.size();
}

void GroundhogModel::setNorthCorrection(double nC)
{
	northCorrection = nC;
}
double GroundhogModel::getNorthCorrection()
{
	return northCorrection;
}

void GroundhogModel::setLatitude(double l)
{
	location->setLatitude(l);
}

void GroundhogModel::setLongitude(double l)
{
	location->setLongitude(l);
}

void GroundhogModel::setTimeZone(double t)
{
	location->setTimezone(t);
}

void GroundhogModel::setMonth(int m)
{
	date->setMonth(m);
}

void GroundhogModel::setDay(int d)
{
	date->setDay(d);
}

void GroundhogModel::setHour(int h)
{
	date->setHour(h);
}

void GroundhogModel::setMinute(int min)
{
	date->setMinute(min);
}

double GroundhogModel::getLatitude()
{
	return location->getLatitude();
}

double GroundhogModel::getLongitude()
{
	return location->getLongitude();
}

double GroundhogModel::getTimeZone()
{
	return location->getTimezone();
}

int GroundhogModel::getMonth()
{
	return date->getMonth();
}

int GroundhogModel::getDay()
{
	return date->getDay();
}

int GroundhogModel::getHour()
{
	return date->getHour();
}

int GroundhogModel::getMinute()
{
	return date->getMinute();
}


void GroundhogModel::setCity(std::string c)
{
	location->setCity(c);
}

std::string GroundhogModel::getCity()
{
	return location->getCity();
}

void GroundhogModel::setCountry(std::string c)
{
	location->setCountry(c);
}

std::string GroundhogModel::getCountry()
{
	return location->getCountry();
}