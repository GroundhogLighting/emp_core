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


void GroundhogModel::addComponentDefinition(std::string * name)
{	
	ComponentDefinition * c = new ComponentDefinition(name);
	definitions.push_back( c );
}


ComponentDefinition *  GroundhogModel::getComponentDefinitionByName(std::string * definitionName)
{
	for (size_t i = 0; i < definitions.size(); i++) {
      std::string * name = definitions[i]->getName();
	  if (*definitionName == *name) {
		  return definitions[i];
	  }
	}
#ifndef AVOID_EMP_CORE_WARNINGS
	FATAL(errorMessage,"Component Definition " + *definitionName + " could not be found");
#endif
    return nullptr;
}

Layer *  GroundhogModel::getLayerByName(std::string * layerName)
{
    size_t nLayers = layers.size();
    std::string * name;
    
	for (size_t i = 0; i < nLayers; i++) {
      name = layers[i]->getName();
	  if (*layerName == *name) {
		  return layers[i];
	  }
	}
#ifndef AVOID_EMP_CORE_WARNINGS
	FATAL(errorMessage,"Layer " + *layerName + " could not be found");
#endif
	return nullptr;
}

Otype *  GroundhogModel::getOtypeByName(std::string * objectName)
{
    size_t nLayers = layers.size();
    std:: string * name;
    Otype * object;
    
    // First, check layers
    for (size_t i = 0; i < nLayers; i++) {
        size_t nObjects = layers[i]->getObjectsRef()->size();
        for(size_t j = 0; j < nObjects; j++){
            object = layers[i]->getObjectRef(j);
            name = object->getName();
            if (*objectName == *name) {
                return object;
            }
        }
        
    }
    
    // Second, check in component definitions
    size_t nDefinitions = definitions.size();
    for (size_t i = 0; i < nDefinitions; i++) {
        size_t nObjects = definitions[i]->getObjectsRef()->size();
        for(size_t j = 0; j < nObjects; j++){
            object = definitions[i]->getObjectRef(j);
            name = object->getName();
            if (*objectName == *name) {
                return object;
            }
        }
        
    }
    // If not found, 
#ifndef AVOID_EMP_CORE_WARNINGS
    FATAL(errorMessage,"Object " + *objectName + " could not be found");
#endif
    return nullptr;
}

void GroundhogModel::addView(View * view)
{
	views.push_back(view);
}

View * GroundhogModel::getViewRef(size_t i)
{
	return views[i];
}

View * GroundhogModel::getViewByName(std::string * viewName)
{
    size_t nViews = views.size();
    for (size_t i = 0; i < nViews; i++) {
            std::string * name = views[i]->getName();
            if (*viewName == *name) {
                return views[i];
            }
        }
    #ifndef AVOID_EMP_CORE_WARNINGS
        FATAL(errorMessage,"View " + *viewName + " could not be found");
    #endif
        return nullptr;
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

Workplane * GroundhogModel::getWorkplaneByName(std::string * wp)
{
	for (size_t i = 0; i < workplanes.size(); i++) {
        std::string * name = workplanes[i]->getName();
		if (*name == *wp)
			return workplanes[i];
	}
#ifndef AVOID_EMP_CORE_WARNINGS
    FATAL(errorMessage,"Workplane " + *wp + " could not be found");
#endif
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
    
    
    if (cl == "dielectric") {
        Dielectric * g = new Dielectric(j);
        materials.push_back(g);
        return g;
    } else if (cl == "glass") {
        Glass * g = new Glass(j);
        materials.push_back(g);
        return g;
    } else if (cl == "glow") {
        Glow * g = new Glow(j);
        materials.push_back(g);
        return g;
    } else if (cl == "interface") {
        Interface * g = new Interface(j);
        materials.push_back(g);
        return g;
    } else if (cl == "light") {
        Light * g = new Light(j);
        materials.push_back(g);
        return g;
    } else if (cl == "metal") {
        Metal * g = new Metal(j);
        materials.push_back(g);
        return g;
    } else if (cl == "plastic") {
		Plastic * p = new Plastic(j);
		materials.push_back(p);
		return p;
    } else if (cl == "spotlight") {
        Spotlight * g = new Spotlight(j);
        materials.push_back(g);
        return g;
    } else if (cl == "trans") {
        Trans * g = new Trans(j);
        materials.push_back(g);
        return g;
    } else {
        
        std::string e = "Unsupported material class "+ cl;
        FATAL(errorMessage,e);
		return NULL;
	}
}

void GroundhogModel::addMaterial(Material * m)
{
    materials.push_back(m);
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

Material *  GroundhogModel::getMaterialByName(std::string * materialName)
{
    for (size_t i = 0; i < materials.size(); i++) {
        std::string * name = materials[i]->getName();
        if (*materialName == *name) {
            return materials[i];
        }
    }
#ifndef AVOID_EMP_CORE_WARNINGS
    FATAL(errorMessage,"Material " + *materialName + " could not be found");
#endif
    return nullptr;
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
