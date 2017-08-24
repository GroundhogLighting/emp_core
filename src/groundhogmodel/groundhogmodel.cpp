/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "./groundhogmodel.h"
#include "../common/utilities/io.h"



GroundhogModel::GroundhogModel()
{
	DEBUG_MSG("Creating GroundhogModel");	
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

	DEBUG_MSG("Destroying GroundhogModel");
}


void GroundhogModel::addLayer(std::string * layerName)
{	
	Layer * l = new Layer(layerName);
	layers.push_back( l );	
	DEBUG_MSG("Adding layer " + *layerName + " to model");
}

bool GroundhogModel::addFaceToLayer(std::string * layerName, Face * face)
{
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {		
		if (layers[layerCount]->compareName(layerName)) {
			DEBUG_MSG("Found layer "+*layerName);
			addFaceToVector(layers[layerCount]->getFacesRef(), face);
			return true;
		}
	}
	fatal("Layer " + *layerName + " could not be found", __LINE__, __FILE__);
	return false;
}


bool GroundhogModel::addComponentInstanceToLayer(std::string * layerName, ComponentInstance * instance)
{
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {
		if (layers[layerCount]->compareName(layerName)) {
			DEBUG_MSG("Found layer " + *layerName);
			addComponentInstanceToVector(layers[layerCount]->getComponentInstancesRef(), instance);
			return true;
		}
	}
	fatal("Layer " + *layerName + " could not be found", __LINE__, __FILE__);
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
	fatal("Component Definition " + *definitionName + " could not be found", __LINE__, __FILE__);
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
	fatal("Layer " + *layerName + " could not be found", __LINE__, __FILE__);
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
	location.setLatitude(l);
}

void GroundhogModel::setLongitude(double l)
{
	location.setLongitude(l);
}

void GroundhogModel::setTimeZone(double t)
{
	location.setTimezone(t);
}

void GroundhogModel::setMonth(int m)
{
	date.setMonth(m);
}

void GroundhogModel::setDay(int d)
{
	date.setDay(d);
}

void GroundhogModel::setHour(int h)
{
	date.setHour(h);
}

void GroundhogModel::setMinute(int min)
{
	date.setMinute(min);
}

double GroundhogModel::getLatitude()
{
	return location.getLatitude();
}

double GroundhogModel::getLongitude()
{
	return location.getLongitude();
}

double GroundhogModel::getTimeZone()
{
	return location.getTimezone();
}

void GroundhogModel::setAlbedo(double a)
{
	location.setAlbedo(a);
}

double GroundhogModel::getAlbedo()
{
	return location.getAlbedo();
}

int GroundhogModel::getMonth()
{
	return date.getMonth();
}

int GroundhogModel::getDay()
{
	return date.getDay();
}

int GroundhogModel::getHour()
{
	return date.getHour();
}

int GroundhogModel::getMinute()
{
	return date.getMinute();
}


void GroundhogModel::setCity(std::string c)
{
	location.setCity(c);
}

std::string GroundhogModel::getCity()
{
	return location.getCity();
}

void GroundhogModel::setCountry(std::string c)
{
	location.setCountry(c);
}

std::string GroundhogModel::getCountry()
{
	return location.getCountry();
}


void GroundhogModel::addPolygonToWorkplane(std::string * workplaneName, Polygon3D * polygon) 
{
	for (unsigned i = 0; i < workplanes.size(); i++) {
		if (workplanes[i]->compareName(workplaneName)) {
			DEBUG_MSG("Found workplane " + *workplaneName);
			workplanes[i]->addPolygon(polygon);
			return;
		}
	}

	// Workplane not found... lets create one.
	Workplane * wp = new Workplane(*workplaneName);
	wp->addPolygon(polygon);
	workplanes.push_back(wp);	
}


void GroundhogModel::addWindowToGroup(std::string * windowGroupName, Face * face) 
{
	for (size_t i = 0; i < windowGroups.size(); i++) {
		if (windowGroups[i]->compareName(windowGroupName)) {
			DEBUG_MSG("Found window group " + *windowGroupName);
			windowGroups[i]->addFace(face);
			return;
		}
	}

	// Workplane not found... lets create one.
	WindowGroup * wg = new WindowGroup(*windowGroupName);
	wg->addFace(face);
	windowGroups.push_back(wg);
}

size_t GroundhogModel::getNumWindowGroups() 
{
	return windowGroups.size();
}

size_t GroundhogModel::getNumWorkplanes() 
{
	return workplanes.size();
}


WindowGroup * GroundhogModel::getWindowGroupRef(size_t i) 
{
	return windowGroups[i];
}

Workplane * GroundhogModel::getWorkplaneRef(size_t i) 
{
	return workplanes[i];
}

Material * GroundhogModel::addMaterial(json j)
{
	std::string name = j["name"];
	for (size_t i = 0; i < materials.size(); i++) {
		if (materials[i]->compareName(&name))
			return materials[i];
	}

	if (j["class"] == "plastic") {
		Plastic * p = new Plastic(j);
		materials.push_back(p);
		return p;
	}
	else if (j["class"] == "glass") {
		Glass * g = new Glass(j);
		materials.push_back(g);
		return g;
	}
	else {
		fatal("Unsupported material class "+j["class"] , __LINE__, __FILE__);
		return NULL;
	}
}


Material * GroundhogModel::addDefaultMaterial()
{
	return addMaterial({
		{ "name","Default Material" },
		{ "color",{ 153,153,153 } },
		{ "rad","void plastic %MAT_NAME% 0 0 5 0.6 0.6 0.6 0 0" },
		{"alpha",1},
		{ "class","plastic" }
	});
}


Material * GroundhogModel::addDefaultGlass()
{
	return addMaterial({
		{ "name","Default Glass" },
		{ "color",{ 0,0,1} },
		{ "rad","void glass %MAT_NAME% 0 0 3 0.86 0.86 0.86" },
		{"alpha",0.4},
		{ "class","glass" }
	});
}


size_t GroundhogModel::getNumMaterials()
{
	return materials.size();
}

Material * GroundhogModel::getMaterialRef(size_t i)
{
	return materials[i];
}

void GroundhogModel::addPhotosensor(Photosensor * p)
{
	photosensors.push_back(p);
}


size_t GroundhogModel::countPhotosensors()
{
	return photosensors.size();
}


Photosensor * GroundhogModel::getPhotosensorRef(size_t i)
{
	return photosensors[i];
}