/*****************************************************************************
	Emp

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
};

GroundhogModel::~GroundhogModel()
{
    
    for(auto x : layers)
        delete x;
    
    for(auto x : workplanes)
        delete x;
    
    for(auto x : photosensors)
        delete x;
    
    for(auto x : materials)
        delete x;
    
    for(auto x : views)
        delete x;
    
    for(auto x : definitions)
        delete x;
    
    for(auto x : windowGroups)
        delete x;
}


void GroundhogModel::addLayer(std::string * layerName)
{	
	Layer * l = new Layer(layerName);
	layers.push_back( l );	
}

bool GroundhogModel::addObjectToLayer(std::string * layerName, Otype * o)
{
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {		
		if (layers[layerCount]->compareName(layerName)) {
			layers[layerCount]->getObjectsRef()->push_back(o);
			return true;
		}
	}
	FATAL(errorMessage,"Layer " + *layerName + " could not be found");
	return false;
}


bool GroundhogModel::addComponentInstanceToLayer(std::string * layerName, ComponentInstance * instance)
{
	for (unsigned layerCount = 0; layerCount < layers.size(); layerCount++) {
		if (layers[layerCount]->compareName(layerName)) {
			layers[layerCount]->getComponentInstancesRef()->push_back(instance);
			return true;
		}
	}
	FATAL(errorMessage,"Layer " + *layerName + " could not be found");
	return false;
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
}


ComponentDefinition *  GroundhogModel::getComponentDefinitionByName(std::string * definitionName)
{
	for (size_t i = 0; i < definitions.size(); i++) {
      std::string * name = definitions[i]->getName();
	  if (*definitionName == *name) {
		  return definitions[i];
	  }
	}
	FATAL(errorMessage,"Component Definition " + *definitionName + " could not be found");
	return NULL;
}

Layer *  GroundhogModel::getLayerByName(std::string * layerName)
{
	for (size_t i = 0; i < layers.size(); i++) {
      std::string * name = layers[i]->getName();
	  if (*layerName == *name) {
		  return layers[i];
	  }
	}
	FATAL(errorMessage,"Layer " + *layerName + " could not be found");
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


void GroundhogModel::addPolygonToWorkplane(std::string * workplaneName, Polygon3D * polygon) 
{
	for (unsigned i = 0; i < workplanes.size(); i++) {
		if (workplanes[i]->compareName(workplaneName)) {
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

Workplane * GroundhogModel::getWorkplaneByName(std::string wp)
{
	for (size_t i = 0; i < workplanes.size(); i++) {
		if (*(workplanes[i]->getName()) == wp)
			return workplanes[i];
	}	
	return NULL;
}

Material * GroundhogModel::addMaterial(json * j)
{
	// Check if material already exists
	std::string name = (*j)["name"];
	for (size_t i = 0; i < materials.size(); i++) {
		if (materials[i]->compareName(&name))
			return materials[i];
	}
    std::string cl = (*j)["class"];
    
	if (cl == "plastic") {
		Plastic * p = new Plastic(j);
		materials.push_back(p);
		return p;
	}
	else if (cl == "glass") {
		Glass * g = new Glass(j);
		materials.push_back(g);
		return g;
	}
	else {
        
        std::string e = "Unsupported material class "+ cl;
        FATAL(errorMessage,e);
		return NULL;
	}
}


Material * GroundhogModel::addDefaultMaterial()
{
  json j = {
    { "name" , "Default-Material" },
    { "color" ,{ 153,153,153 } },
    { "rad" , "void plastic %MAT_NAME% 0 0 5 0.6 0.6 0.6 0 0" },
    { "alpha" , 1 },
    { "class" , "plastic" }
  };
  return addMaterial(&j);
}


Material * GroundhogModel::addDefaultGlass()
{ 
  json j = {
    { "name" , "Default-Glass" },
    { "color" ,{ 0,0,1 } },
    { "rad" , "void glass %MAT_NAME% 0 0 3 0.86 0.86 0.86" },
    { "alpha" , 0.4 },
    { "class" , "glass" }
  };
  return addMaterial(&j);
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

Location * GroundhogModel::getLocation()
{
	return &location;
}

Date * GroundhogModel::getDate()
{
	return &date;
}

RTraceOptions * GroundhogModel::getRTraceOptions()
{
	return &rtraceOptions;
}


Material * GroundhogModel::hasMaterial(std::string * matName)
{
  size_t nMaterials = materials.size();

  for (size_t i = 0; i < nMaterials; i++)
  {
    if (materials[i]->compareName(matName)) {
      return materials[i];
    }
  }
  return NULL;
}
