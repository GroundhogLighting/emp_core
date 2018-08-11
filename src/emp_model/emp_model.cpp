/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

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


#include "./emp_model.h"
#include "../common/utilities/io.h"



EmpModel::EmpModel()
{
};

EmpModel::~EmpModel()
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


void EmpModel::addLayer(std::string * layerName)
{	
	Layer * l = new Layer(layerName);
	layers.push_back( l );	
}

bool EmpModel::addObjectToLayer(std::string * layerName, Otype * o)
{
    Layer * layer = getLayerByName(layerName);
    layer->addObject(o);
        
	return false;
}


size_t EmpModel::getNumLayers()
{
	return layers.size();
}

size_t EmpModel::getNumComponentDefinitions()
{
	return definitions.size();
}

Layer * EmpModel::getLayerRef(size_t i)
{
	return layers[i];
}

ComponentDefinition * EmpModel::getComponentDefinitionRef(size_t i)
{
	return definitions[i];
}


void EmpModel::addComponentDefinition(std::string * name)
{	
	ComponentDefinition * c = new ComponentDefinition(name);
	definitions.push_back( c );
}


ComponentDefinition *  EmpModel::getComponentDefinitionByName(std::string * definitionName)
{
	for (size_t i = 0; i < definitions.size(); i++) {
      std::string name = definitions[i]->getName();
	  if (*definitionName == name) {
		  return definitions[i];
	  }
	}
#ifndef AVOID_EMP_CORE_WARNINGS
	FATAL(errorMessage,"Component Definition " + *definitionName + " could not be found");
#endif
    return nullptr;
}

Layer *  EmpModel::getLayerByName(std::string * layerName)
{
    size_t nLayers = layers.size();
    
    
	for (size_t i = 0; i < nLayers; i++) {
      std::string name = layers[i]->getName();
	  if (*layerName == name) {
		  return layers[i];
	  }
	}
#ifndef AVOID_EMP_CORE_WARNINGS
	FATAL(errorMessage,"Layer " + *layerName + " could not be found");
#endif
	return nullptr;
}

const Otype * const EmpModel::getOtypeByName(std::string * objectName) const
{
    size_t nLayers = layers.size();
    
    
    
    // First, check layers
    for (size_t i = 0; i < nLayers; i++) {
        size_t nObjects = layers[i]->getObjectsRef()->size();
        for(size_t j = 0; j < nObjects; j++){
            const Otype * object = layers[i]->getObjectRef(j);
            std:: string name = object->getName();
            if (*objectName == name) {
                return object;
            }
        }
        
    }
    
    // Second, check in component definitions
    size_t nDefinitions = definitions.size();
    for (size_t i = 0; i < nDefinitions; i++) {
        size_t nObjects = definitions[i]->getObjectsRef()->size();
        for(size_t j = 0; j < nObjects; j++){
            const Otype * object = definitions[i]->getObjectRef(j);
            std::string name = object->getName();
            if (*objectName == name) {
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

void EmpModel::addView(View * view)
{
	views.push_back(view);
}

View * EmpModel::getViewRef(size_t i)
{
	return views[i];
}

View * EmpModel::getViewByName(std::string * viewName)
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

size_t EmpModel::getNumViews()
{
	return views.size();
}

void EmpModel::setNorthCorrection(double nC)
{
	northCorrection = nC;
}
double EmpModel::getNorthCorrection()
{
	return northCorrection;
}


void EmpModel::addPolygonToWorkplane(std::string * workplaneName, Polygon3D * polygon) 
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


void EmpModel::addWindowToGroup(std::string * windowGroupName, Face * face) 
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

void EmpModel::addIllumToGroup(std::string * illumGroupName, Polygon3D * polygon)
{
    for (size_t i = 0; i < illumGroups.size(); i++) {
        if (illumGroups[i]->compareName(illumGroupName)) {
            illumGroups[i]->addPolygon(polygon);
            return;
        }
    }
    
    // Workplane not found... lets create one.
    IllumGroup * ig = new IllumGroup(*illumGroupName);
    ig->addPolygon(polygon);
    illumGroups.push_back(ig);
}


size_t EmpModel::getNumWindowGroups() 
{
	return windowGroups.size();
}

size_t EmpModel::getNumIllumGroups()
{
    return illumGroups.size();
}

size_t EmpModel::getNumWorkplanes() 
{
	return workplanes.size();
}


WindowGroup * EmpModel::getWindowGroupRef(size_t i) 
{
	return windowGroups[i];
}

IllumGroup * EmpModel::getIllumGroupRef(size_t i)
{
    return illumGroups[i];
}


Workplane * EmpModel::getWorkplaneRef(size_t i) 
{
	return workplanes[i];
}

Workplane * EmpModel::getWorkplaneByName(const std::string * const wp) const
{
	for (size_t i = 0; i < workplanes.size(); i++) {
        std::string name = workplanes[i]->getName();
		if (name == *wp)
			return workplanes[i];
	}
#ifndef AVOID_EMP_CORE_WARNINGS
    FATAL(errorMessage,"Workplane " + *wp + " could not be found");
#endif
	return NULL;
}

Material * EmpModel::addMaterial(json * j)
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

void EmpModel::addMaterial(Material * m)
{    
    materials.push_back(m);
}


Material * EmpModel::addDefaultMaterial()
{
  json j = {
      { "name" , "Default Material" },
      { "class" , "plastic" },
      { "color",{
          {"r",0.6},{"g",0.6},{"b",0.6}
      }},
      { "specularity", 0},{"roughness",0}
  };
  return addMaterial(&j);
}


Material * EmpModel::addDefaultGlass()
{ 
  json j = {
      { "name" , "Default Glass" },
      { "class" , "glass" },
      { "color",{
          {"r",0.86},{"g",0.86},{"b",0.86}
      }},
  };
  return addMaterial(&j);
}


size_t EmpModel::getNumMaterials()
{
	return materials.size();
}

Material * EmpModel::getMaterialRef(size_t i)
{
	return materials[i];
}

Material *  EmpModel::getMaterialByName(std::string * materialName)
{
    for (size_t i = 0; i < materials.size(); i++) {
        std::string name = materials[i]->getName();
        if (*materialName == name) {
            return materials[i];
        }
    }
#ifndef AVOID_EMP_CORE_WARNINGS
    FATAL(errorMessage,"Material " + *materialName + " could not be found");
#endif
    return nullptr;
}

void EmpModel::addPhotosensor(Photosensor * p)
{
	photosensors.push_back(p);
}


size_t EmpModel::countPhotosensors()
{
	return photosensors.size();
}


Photosensor * EmpModel::getPhotosensorRef(size_t i)
{
	return photosensors[i];
}

Photosensor * EmpModel::getPhotosensorRef(std::string name)
{
    for(auto p : photosensors){
        if(p->getName() == name)
            return p;        
    }
    return nullptr;
}

Location * EmpModel::getLocation()
{
	return &location;
}

Date * EmpModel::getDate()
{
	return &date;
}

RTraceOptions * EmpModel::getRTraceOptions()
{
	return &rtraceOptions;
}


Material * EmpModel::hasMaterial(std::string * matName)
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

void EmpModel::addTask(json task)
{
    tasks.push_back(task);
}

size_t EmpModel::countTasks() const
{
    return tasks.size();
}

const json * EmpModel::getTask(size_t i) const
{
    return &tasks[i];
}

const json * EmpModel::getTask(std::string name) const
{
    size_t nTasks = tasks.size();
    for(size_t i=0; i<nTasks; i++){
        
        if(tasks[i]["name"].get<std::string>() == name)
            return &tasks[i];
    }
    
    return nullptr;
}
