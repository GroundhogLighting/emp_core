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

#ifndef AVOID_SKP

#include "./SKPreader.h"
#include "config_constants.h"
#include "common/utilities/io.h"
#include "common/utilities/stringutils.h"
#include "groundhogmodel/groundhogmodel.h"
#include "groundhogmodel/src/otype.h"
#include "common/geometry/polygon.h"
#include "groundhogmodel/src/photosensor.h"

#include <SketchUpAPI/initialize.h>
#include <SketchUpAPI/model/model.h>
#include <SketchUpAPI/model/entities.h>
#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/layer.h>
#include <SketchUpAPI/model/drawing_element.h>
#include <SketchUpAPI/model/entity.h>
#include <SketchUpAPI/model/attribute_dictionary.h>
#include <SketchUpAPI/model/loop.h>
#include <SketchUpAPI/model/component_definition.h>
#include <SketchUpAPI/model/component_instance.h>
#include <SketchUpAPI/transformation.h>
#include <SketchUpAPI/model/scene.h>
#include <SketchUpAPI/model/camera.h>
#include <SketchUpAPI/model/shadow_info.h>
#include <SketchUpAPI/model/material.h>
#include <SketchUpAPI/model/group.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define CHECK_SU(x) checkSUResult(x,"x",__LINE__)

SKPReader::SKPReader(GroundhogModel * ghmodel, bool newVerbose) 
{
	
	//initialize API
	SUInitialize();

	suModel = SU_INVALID;

	groundhogDictionaryName = SU_INVALID;

    CHECK_SU(SUStringCreateFromUTF8(&groundhogDictionaryName, SKP_GROUNDHOG_DICTIONARY));
    /*
	checkSUResult(
		SUStringCreateFromUTF8(&groundhogDictionaryName, SKP_GROUNDHOG_DICTIONARY),
		"SUStringCreateFromUTF8",
		__LINE__
	);
    */

	model = ghmodel;
	verbose = newVerbose;
};

SKPReader::~SKPReader() 
{
	
	
	//release dictionary
  CHECK_SU(SUStringRelease(&groundhogDictionaryName));
  /*
	checkSUResult(
		SUStringRelease(&groundhogDictionaryName),
		"SUStringRelease",
		__LINE__
	);
  */
	
	// Must release the model or there will be memory leaks
	SUModelRelease(&suModel);
	
	// Always terminate the API when done using it
	SUTerminate();
};


bool SKPReader::checkSUResult(SUResult res, std::string functionName, int ln) 
{
	if (res == SU_ERROR_NONE) {
		return true;
	}
	std::string error;
	if (res == SU_ERROR_NULL_POINTER_INPUT) {
		error = "SU_ERROR_NULL_POINTER_INPUT";
	}
	else if (res == SU_ERROR_INVALID_INPUT) {
		error = "SU_ERROR_INVALID_INPUT";
	}
	else if (res == SU_ERROR_NULL_POINTER_OUTPUT) {
		error = "SU_ERROR_NULL_POINTER_OUTPUT";
	}
	else if (res == SU_ERROR_INVALID_OUTPUT) {
		error = "SU_ERROR_INVALID_OUTPUT";
	}
	else if (res == SU_ERROR_OVERWRITE_VALID) {
		error = "SU_ERROR_OVERWRITE_VALID";
	}
	else if (res == SU_ERROR_GENERIC) {
		error = "SU_ERROR_GENERIC";
	}
	else if (res == SU_ERROR_SERIALIZATION) {
		error = "SU_ERROR_SERIALIZATION";
	}
	else if (res == SU_ERROR_OUT_OF_RANGE) {
		error = "SU_ERROR_OUT_OF_RANGE";
	}
	else if (res == SU_ERROR_NO_DATA) {
		error = "SU_ERROR_NO_DATA";
	}
	else if (res == SU_ERROR_INSUFFICIENT_SIZE) {
		error = "SU_ERROR_INSUFFICIENT_SIZE";
	}
	else if (res == SU_ERROR_UNKNOWN_EXCEPTION) {
		error = "SU_ERROR_UNKNOWN_EXCEPTION";
	}
	else if (res == SU_ERROR_MODEL_INVALID) {
		error = "SU_ERROR_MODEL_INVALID";
	}
	else if (res == SU_ERROR_MODEL_VERSION) {
		error = "SU_ERROR_MODEL_VERSION";
	}
	else if (res == SU_ERROR_LAYER_LOCKED) {
		error = "SU_ERROR_LAYER_LOCKED";
	}
	else if (res == SU_ERROR_DUPLICATE) {
		error = "SU_ERROR_DUPLICATE";
	}
	else if (res == SU_ERROR_PARTIAL_SUCCESS) {
		error = "SU_ERROR_PARTIAL_SUCCESS";
	}
	else if (res == SU_ERROR_UNSUPPORTED) {
		error = "SU_ERROR_UNSUPPORTED";
	}
	else if (res == SU_ERROR_INVALID_ARGUMENT) {
		error = "SU_ERROR_INVALID_ARGUMENT";
	}
	else {
		error = "Unrecognized error....";
	}

    FATAL(errorMessage,"function '" + functionName + "' returned '" + error);
	return false;
}


bool SKPReader::parseSKPModel(std::string inputFile)
{		
	//Load model
	if (!checkSUResult(
		SUModelCreateFromFile(&suModel, inputFile.c_str()),
		"SUModelCreateFromFile",
		__LINE__
	)) return false;

	// Load layers	
	if(!loadLayers())
		return false;

	// load components and groups
	if (!loadComponentDefinitions())
		return false;

    if (!loadGroupDefinitions())
      return false;

    if (!fillComponentDefinitions())
      return false;

    if (!fillGroupDefinitions())
      return false;

	// Fill layers and load related surfaces, discrimitating between
	// workplanes, windows, etc.
	if (!loadLayersContent())
		return false;

	// Load views
	if (!loadViews())
		return false;

	// Load model info (location, date, etc).
	if (!loadModelInfo())
		return false;
	
	// Load weather
	if (!loadWeather())
		return false;
	
	return true;
};


bool SKPReader::getStringFromShadowInfo(SUShadowInfoRef shadowInfo, const char * key, std::string * value) 
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!checkSUResult(
		SUTypedValueCreate(&suValue),
		"SUTypedValueCreate",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUShadowInfoGetValue(shadowInfo, key, &suValue),
		"SUShadowInfoGetValue",
		__LINE__
	)) return false;

	SUStringRef suString = SU_INVALID;

	if (!checkSUResult(
		SUStringCreate(&suString),
		"SUStringCreate",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUTypedValueGetString(suValue, &suString),
		"SUTypedValueGetString", 
		__LINE__
	)) return false;

	
	if (!SUStringtoString(suString, value,true))
		return false;
	
	return true;
}

bool SKPReader::getDoubleFromShadowInfo(SUShadowInfoRef shadowInfo, const char * key, double * value) 
{
	SUTypedValueRef suValue= SU_INVALID;
	if (!checkSUResult(
		SUTypedValueCreate(&suValue),
		"SUTypedValueCreate",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUShadowInfoGetValue(shadowInfo, key, &suValue),
		"SUShadowInfoGetValue",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUTypedValueGetDouble(suValue, value),
		"SUTypedValueGetDouble",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUTypedValueRelease(&suValue),
		"SUTypedValueGetDouble", 
		__LINE__
	)) return false;

	return true;
}


bool SKPReader::getTimeFromShadowInfo(SUShadowInfoRef shadowInfo, int64_t * value) 
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!checkSUResult(
		SUTypedValueCreate(&suValue),
		"SUTypedValueCreate",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUShadowInfoGetValue(shadowInfo, "ShadowTime", &suValue),
		"SUShadowInfoGetValue",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUTypedValueGetTime(suValue, value),
		"SUTypedValueGetTime",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUTypedValueRelease(&suValue),
		"SUTypedValueGetDouble",
		__LINE__
	)) return false;

	return true;
}

bool SKPReader::loadModelInfo() 
{
	// load north correction
	double northC;
	if (!checkSUResult(
		SUModelGetNorthCorrection(suModel, &northC),
		"SUModelGetNorthCorrection",
		__LINE__
	)) return false;
	model->setNorthCorrection(northC);

	// load date, longitude and latitude.
	SUShadowInfoRef shadowInfo;
	if (!checkSUResult(
		SUModelGetShadowInfo(suModel, &shadowInfo),
		"SUModelGetShadowInfo",
		__LINE__
	)) return false;

	// Get the location
	Location * loc = model->getLocation();

	// Set latitude
	double latitude;
	getDoubleFromShadowInfo(shadowInfo, "Latitude", &latitude);
	loc->setLatitude(latitude);

	// Set longitude
	double longitude;
	getDoubleFromShadowInfo(shadowInfo, "Longitude", &longitude);
	loc->setLongitude(longitude);

	// Set time zone
	double timeZone;
	getDoubleFromShadowInfo(shadowInfo, "TZOffset", &timeZone);
	loc->setTimeZone(timeZone);
	
	//set city
	std::string city;
	getStringFromShadowInfo(shadowInfo, "City", &city);
	loc->setCity(city);

	//set country
	std::string country;
	getStringFromShadowInfo(shadowInfo, "Country", &country);
	loc->setCountry(country);

	// set time
	int64_t epoch;
	getTimeFromShadowInfo(shadowInfo, &epoch);
	Date date = Date(epoch); //  + timeZone*60*60
	Date * d = model->getDate();
	d->setMonth(date.getMonth());
	d->setDay(date.getDay());
	d->setHour(date.getHour());
	d->setMinute(date.getMinute());

	return true;
}

bool SKPReader::SUCameraToView(std::string * viewName, SUCameraRef camera, View * view) 
{

	// set the name
	view->setName(viewName);

	// get view point and up.
	SUPoint3D position;
	SUPoint3D target; //this is useless
	SUVector3D up;
	if (!checkSUResult(
		SUCameraGetOrientation(camera, &position, &target, &up),
		"SUCameraGetOrientation",
		__LINE__
	)) return false;
	view->setViewPoint(new Point3D(TO_M(position.x), TO_M(position.y), TO_M(position.z)));
	view->setViewUp(Vector3D(up.x, up.y, up.z));

	// get and set the view direction
	SUVector3D direction;
	if (!checkSUResult(
		SUCameraGetDirection(camera, &direction),
		"SUCameraGetDirection",
		__LINE__
	)) return false;
	view->setViewDirection(Vector3D(direction.x, direction.y, direction.z));

	// get and set type
	bool perspective;
	if (!checkSUResult(
		SUCameraGetPerspective(camera, &perspective),
		"SUCameraGetPerspective",
		__LINE__
	)) return false;

	int type = perspective ? PERSPECTIVE_VIEW : PARALLEL_VIEW;
	view->setViewType(type);

	double viewHeight;
	if (type == PERSPECTIVE_VIEW) {
		if (!checkSUResult(
			SUCameraGetPerspectiveFrustumFOV(camera, &viewHeight),
			"SUCameraGetPerspectiveFrustumFOV",
			__LINE__
		)) return false;
	}
	else { // PARALLEL
		if (!checkSUResult(
			SUCameraGetOrthographicFrustumHeight(camera, &viewHeight),
			"SUCameraGetOrthographicFrustumHeight",
			__LINE__
		)) return false;
	}
	view->setViewVertical(viewHeight);

	// get aspect ratio
	double aspectRatio;
	SUResult aspectRatioResult = SUCameraGetAspectRatio(camera, &aspectRatio);
	
	// this needs to be checked manually	
	switch (aspectRatioResult) {
        case SU_ERROR_NONE:
            break; // all OK.
        case SU_ERROR_INVALID_INPUT:
            fatal("SU_ERROR_INVALID_INPUT when trying to get aspect ratio of view",__LINE__,__FILE__);
            break;
        case SU_ERROR_NO_DATA:
            // the camera uses the screen aspect ratio... will assume the following
            aspectRatio = 16.0 / 9.0;
            break;
        case SU_ERROR_NULL_POINTER_OUTPUT:
            fatal("SU_ERROR_NULL_POINTER_OUTPUT when trying to get aspect ratio of view",__LINE__,__FILE__);
            break;
        case SU_ERROR_NULL_POINTER_INPUT:
            break;
        default:
            break;
    }
	

	view->setViewHorizontal(aspectRatio * viewHeight);

	//return
	return true;
}

bool SKPReader::SUViewToView(SUSceneRef suView, View * view) 
{
	// get the name of the view
	
	SUStringRef suViewName = SU_INVALID;
	if (!checkSUResult(
		SUStringCreate(&suViewName),
		"SUStringCreate",
		__LINE__
	)) return false;
	
	if (!checkSUResult(
		SUSceneGetName(suView, &suViewName),
		"SUSceneGetName", 
		__LINE__
	)) return false;
	
	std::string viewName;

	if (!SUStringtoString(suViewName,&viewName,true))
		return false;


	// Get the camera
	SUCameraRef camera = SU_INVALID;
	if (!checkSUResult(
		SUSceneGetCamera(suView,&camera),
		"SUSceneGetCamera",
		__LINE__
	)) return false;

	if (!SUCameraToView(&viewName, camera, view)) {
		SUCameraRelease(&camera);
		return false;
	}
		
	return true;
}

bool SKPReader::loadViews() 
{
	
	// get the current view
	SUCameraRef activeCamera;

	if (!checkSUResult(
		SUModelGetCamera(suModel, &activeCamera),
		"SUModelGetActiveScene",
		__LINE__
	)) return false;

	View * activeView = new View();
    std::string viewName = "view";
	SUCameraToView(&viewName, activeCamera, activeView);
	model->addView(activeView);

	// load stored views
	size_t countScenes;

	if (!checkSUResult(
		SUModelGetNumScenes(suModel, &countScenes),
		"SUModelGetNumScenes",
		__LINE__
	)) return false;
	
	if (countScenes == 0) {
		return true;
	}
	std::vector<SUSceneRef> views(countScenes);

	if (!checkSUResult(
		SUModelGetScenes(suModel, countScenes, &views[0],&countScenes),
		"SUModelGetScenes",
		__LINE__
	)) return false;

	for (size_t i = 0; i < countScenes; i++) {
		View * storedView = new View();
		SUViewToView(views[i], storedView);
		model->addView(storedView);
	}

	return true;
}

bool SKPReader::loadLayers() 
{

	// count layers
	size_t countLayers = 0;
	if(!checkSUResult(
		SUModelGetNumLayers(suModel, &countLayers),
		"SUModelGetNumLayers",
		__LINE__
	)) return false;

	//get those layers
	std::vector<SULayerRef> layers(countLayers);
	if(!checkSUResult(
		SUModelGetLayers(suModel, countLayers, &layers[0], &countLayers),
		"SUModelGetLayers",
		__LINE__
	)) return false;

	// inform layer status
	INFORM(i,"Counted layers: " + size_tToString(countLayers), verbose);

	// create and load the layers
	for (unsigned int i = 0; i < layers.size(); i++) {		
		SUStringRef suLayerName = SU_INVALID;		
		if(!checkSUResult(
			SUStringCreate(&suLayerName),
			"SUStringCreate",
			__LINE__
		)) return false;


		if (!checkSUResult(
			SULayerGetName(layers[i],&suLayerName),
			"SUStringRelease",
			__LINE__
		)) return false;

		std::string layerName;

		if (!SUStringtoString(suLayerName,&layerName,true))
			return false;
		
		model->addLayer(&layerName);
		INFORM(informMessage,"Layer " + layerName + " added",verbose);
	};	

	return true;
} // end of Load Layers

bool SKPReader::getSUComponentDefinitionName(SUComponentDefinitionRef definition, std::string * name) 
{
	//define a SUString
	SUStringRef suComponentName = SU_INVALID;

	if (!checkSUResult(
		SUStringCreate(&suComponentName),
		"SUStringCreate",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUComponentDefinitionGetName(definition, &suComponentName),
		"SUComponentInstanceGetName",
		__LINE__
	)) return false;

	std::string componentName;
	if (!SUStringtoString(suComponentName,&componentName,true))
		return false;

	*name = componentName;
	return true;
}


bool SKPReader::addComponentInstanceToVector(std::vector <ComponentInstance * > * dest, SUComponentInstanceRef suComponentInstance) 
{

	// get definition
	SUComponentDefinitionRef definition;
	if (!checkSUResult(
		SUComponentInstanceGetDefinition(suComponentInstance, &definition),
		"SUComponentInstanceGetDefinition",
		__LINE__
	)) return false;

	std::string definitionName;
	getSUComponentDefinitionName(definition, &definitionName);

	// get the definition
	ComponentDefinition * definitionRef = model->getComponentDefinitionByName(&definitionName);
	if (definitionRef == NULL) {
		FATAL(errorMessage,"Impossible to find " + definitionName + " when adding an instance... ignoring instance");
		return false;
	}

	//create the instance
	ComponentInstance * instance = new ComponentInstance(definitionRef);

	//Fill the position
	if(!fillComponentInstanceLocation(instance, suComponentInstance))
		return false;

	//add and return
	dest->push_back(instance);
	return true;
}


bool SKPReader::bulkFacesIntoVector(std::vector <Otype * > * dest, SUEntitiesRef entities) 
{

	// count faces in these entities
	size_t numFaces = 0;
	if (!checkSUResult(
		SUEntitiesGetNumFaces(entities, &numFaces),
		"SUEntitiesGetNumFaces",
		__LINE__
	)) return false;

	if (numFaces == 0)
		return true;

	// get the faces
	std::vector<SUFaceRef> faces(numFaces);
	if (!checkSUResult(
		SUEntitiesGetFaces(entities, numFaces, &faces[0], &numFaces),
		"SUEntitiesGetFaces",
		__LINE__
	)) return false;

	// import faces
	for (size_t i = 0; i < numFaces; i++) {	
		Face * f = SUFaceToFace(faces[i]);

		if (f == NULL)
			return false;

		dest->push_back(f);
	}
	return true;
}


bool SKPReader::loadComponentDefinition(SUComponentDefinitionRef definition) 
{
  // Check if it has label
  std::string label;
  if (getSUEntityLabel(SUComponentDefinitionToEntity(definition), &label)) {

    if (label == SKP_SOLVED_WORKPLANE)
      return true;

    if (label == SKP_PHOTOSENSOR) {
      if (!addPhotosensorsToModel(definition)) {
        FATAL(errorMessage, "Error when trying to add Photosensos to the model");
        return false;
      }
    }
  }

  //get the name
  std::string definitionName;
  if (!getSUComponentDefinitionName(definition, &definitionName)) {
	  warn("Impossible to get name of component");
	  return false;
  };

  // Create the definition
  ComponentDefinition * componentDefinition = new ComponentDefinition(&definitionName);

  // add it to the model
  model->addComponentDefinition(componentDefinition);  

  return true;
}

bool SKPReader::loadComponentDefinitions()
{
  // count the component definitions
  size_t countDefinitions = 0;
  if (!checkSUResult(
    SUModelGetNumComponentDefinitions(suModel, &countDefinitions),
    "SUModelGetNumComponentDefinitions",
    __LINE__
  )) return false;

  // return if none
  if (countDefinitions == 0) {
    inform("No component definitions in model", verbose);
    return true; // success
  }
  INFORM(i, std::to_string(countDefinitions) + " component definitions in model", verbose);

  // get the component definitions
  std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
  if (!checkSUResult(
    SUModelGetComponentDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions),
    "SUModelGetComponentDefinitions",
    __LINE__
  )) return false;

  // Now, load One by One
  for (size_t i = 0; i < countDefinitions; i++) {
    loadComponentDefinition(definitions[i]);
  }
  return true;
}



bool SKPReader::fillComponentDefinitions() 
{
	// count the component definitions
	size_t countDefinitions = 0;
	if (!checkSUResult(
		SUModelGetNumComponentDefinitions(suModel, &countDefinitions),
		"SUModelGetNumComponentDefinitions",
		__LINE__
	)) return false;

	// return if none
	if (countDefinitions == 0) {
		inform("No component definitions in model",verbose);
		return true; // success
	}

	// get the component definitions
	std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
	if (!checkSUResult(
		SUModelGetComponentDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions),
		"SUModelGetComponentDefinitions", 
		__LINE__
	)) return false;

	// Now, load One by One
	for (size_t i = 0; i < countDefinitions; i++) {
      
      //get the name
      std::string definitionName;
      if (!getSUComponentDefinitionName(definitions[i], &definitionName)) {
        warn("Impossible to get name of component");
        return false;
      };

      ComponentDefinition * ghDefinition = model->getComponentDefinitionByName(&definitionName);

      // get entities
      SUEntitiesRef entities;
      if (!checkSUResult(
      SUComponentDefinitionGetEntities(definitions[i], &entities),
      "SUComponentDefinitionGetEntities",
      __LINE__
      )) return false;


      // Load faces
      bulkFacesIntoVector(ghDefinition->getObjectsRef(), entities);

      // load instances
      bulkComponentInstancesIntoVector(ghDefinition->getComponentInstancesRef(), entities);
      
	}
	return true;
  
}



bool SKPReader::fillGroupDefinitions()
{
  // count the component definitions
  size_t countDefinitions = 0;
  if (!checkSUResult(
    SUModelGetNumGroupDefinitions(suModel, &countDefinitions),
    "SUModelGetNumComponentDefinitions",
    __LINE__
  )) return false;

  // return if none
  if (countDefinitions == 0) {
    inform("No component definitions in model", verbose);
    return true; // success
  }

  // get the component definitions
  std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
  if (!checkSUResult(
    SUModelGetGroupDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions),
    "SUModelGetComponentDefinitions",
    __LINE__
  )) return false;

  // Now, load One by One
  for (size_t i = 0; i < countDefinitions; i++) {

    std::string definitionName;
    if (!getSUComponentDefinitionName(definitions[i], &definitionName)) {
      warn("Impossible to get name of component");
      return false;
    };

    ComponentDefinition * ghDefinition = model->getComponentDefinitionByName(&definitionName);

    // get entities
    SUEntitiesRef entities;
    if (!checkSUResult(
      SUComponentDefinitionGetEntities(definitions[i], &entities),
      "SUComponentDefinitionGetEntities",
      __LINE__
    )) return false;


    // Load faces
    bulkFacesIntoVector(ghDefinition->getObjectsRef(), entities);

    // load instances
    bulkComponentInstancesIntoVector(ghDefinition->getComponentInstancesRef(), entities);

  }
  return true;

}

bool SKPReader::loadInstance(SUComponentInstanceRef instance)
{
  
  std::cout << "about to load............" << std::endl;
  // ignore instances with certain labels
  std::string label;
  if (getSUEntityLabel(SUComponentInstanceToEntity(instance), &label)) {
    if (label == SKP_PHOTOSENSOR)
      return true;

    if (label == SKP_SOLVED_WORKPLANE)
      return true;
  }

  // get name of layer
  SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instance);
  std::string layerName;
  if (!getSUDrawingElementLayerName(drawingElement, &layerName))
    return false;

  Layer * layerRef = model->getLayerByName(&layerName);
  if (layerRef == NULL) {
    return false;
  }

  std::cout << " Addding  instance to " << *(layerRef->getName()) << std::endl;

  addComponentInstanceToVector(layerRef->getComponentInstancesRef(), instance);
  
  return true;
}


bool SKPReader::loadGroupDefinitions()
{
  
  // count the component definitions
  size_t countDefinitions = 0;
  if (!checkSUResult(
    SUModelGetNumGroupDefinitions(suModel, &countDefinitions),
    "SUModelGetNumComponentDefinitions",
    __LINE__
  )) return false;

  // return if none
  if (countDefinitions == 0) {
    inform("No Group definitions in model", verbose);
    return true; // success
  }
  INFORM(i, std::to_string(countDefinitions) + " group definitions in model", verbose);

  // get the component definitions
  std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
  if (!checkSUResult(
    SUModelGetGroupDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions),
    "SUModelGetComponentDefinitions",
    __LINE__
  )) return false;

  // Now, load One by One
  for (size_t i = 0; i < countDefinitions; i++) {
    loadComponentDefinition(definitions[i]);
  }
  
  return true;
}

bool SKPReader::loadLayersContent() 
{
	// Get the entity container of the model.
	SUEntitiesRef entities = SU_INVALID;
	if (!checkSUResult(
		SUModelGetEntities(suModel, &entities),
		"SUModelGetEntities",
		__LINE__
	)) return false;


	// count and load faces
	size_t faceCount = 0;
	if (!checkSUResult(
		SUEntitiesGetNumFaces(entities, &faceCount),
		"SUModelGetEntities",
		__LINE__
	)) return false;

	if (faceCount > 0) {
		INFORM(i,"Counted Faces: " + size_tToString(faceCount), verbose);

		std::vector<SUFaceRef> faces(faceCount);
		if (!checkSUResult(
			SUEntitiesGetFaces(entities, faceCount, &faces[0], &faceCount),
			"SUEntitiesGetFaces",
			__LINE__
		)) return false;
	
		for (size_t i = 0; i < faceCount; i++) {

			// CHECK LABEL OF FACE
			std::string faceLabel;
			bool hasLabel = getSUFaceLabel(faces[i], &faceLabel);
		
			if ( hasLabel ) {

				if (faceLabel == SKP_WORKPLANE) {
				// if it is workplane
					addWorkplaneToModel(faces[i]);
				}
				else if (faceLabel == SKP_ILLUM) {
				// if it is illum

				}
				else if (faceLabel == SKP_WINDOW) {
				// if it is window
					addWindowToModel(faces[i]);
				}
				continue;
			}
		
			// if has no label (i.e. is geometry face)		
			std::string layerName;		
			if (!getSUFaceLayerName(faces[i],&layerName))
				return false;

			Layer * layerRef = model->getLayerByName(&layerName);
			if (layerRef == NULL) {
				return false;
			}
		
			Face * face = SUFaceToFace(faces[i]);
			if (face == NULL)
				return false;		

			// add the face
			layerRef->getObjectsRef()->push_back(face);

		} // end of iterating faces
	            
	}
	else {
		// Do not return... there may be groups or component instances
		warn("No faces in model");		
	}


	// load component instances
	size_t instanceCount = 0;
	if (!checkSUResult(
		SUEntitiesGetNumInstances(entities, &instanceCount),
		"SUEntitiesGetNumInstances",
		__LINE__
	)) return false;

    if (instanceCount > 0) {
	  std::vector<SUComponentInstanceRef> instances(instanceCount);
	  if (!checkSUResult(
		  SUEntitiesGetInstances(entities, instanceCount, &instances[0], &instanceCount),
		  "SUEntitiesGetInstances",
		  __LINE__
	  )) return false;

	  // fill layers with the instances
	  for (size_t i = 0; i < instanceCount; i++) {		
        loadInstance(instances[i]);
	  }      

    } // end loading component instances.

    // Load group instances   
    size_t groupCount = 0;
    if (!checkSUResult(
      SUEntitiesGetNumGroups(entities, &groupCount),
      "SUModelGetEntities",
      __LINE__
    )) return false;

    if (groupCount > 0) {
      // Declare the vector
      std::vector<SUGroupRef> groups(groupCount);
      
      // Fill vector
      if (!checkSUResult(
        SUEntitiesGetGroups(entities, groupCount, &groups[0], &groupCount),
        "SUEntitiesGetInstances",
        __LINE__
      )) return false;

      // fill layers with the instances
      for (size_t i = 0; i < groupCount; i++) {
        SUComponentInstanceRef instance = SUGroupToComponentInstance(groups[i]);
        loadInstance(instance);        
      }

    } // end loading groups

	return true;
} // end of Load Faces

bool SKPReader::SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon) 
{

	// get area
	double area;
	if (!checkSUResult(
		SUFaceGetArea(face,&area),
		"SUFaceGetArea",
		__LINE__
	)) return false;
	polygon->setArea(TO_M2(area));
	
	// Get the normal
	SUVector3D normal;
	if (!checkSUResult(
		SUFaceGetNormal(face, &normal),
		"SUFaceGetArea",
		__LINE__
	)) return false;
	polygon->setNormal(Vector3D(normal.x, normal.y, normal.z));

	// get the outer loop
	SULoopRef suOuterLoop = SU_INVALID;
	if (!checkSUResult(
		SUFaceGetOuterLoop(face,&suOuterLoop),
		"SUFaceGetOuterLoop",
		__LINE__
	)) return false;

	// translate outer loop
	if (!SULoopToLoop(suOuterLoop, polygon->getOuterLoopRef()))
		return false;

	// get number of inner loops
	size_t countInnerLoops;
	if (!checkSUResult(
		SUFaceGetNumInnerLoops(face, &countInnerLoops),
		"SUFaceGetNumInnerLoops",
		__LINE__
	)) return false;

	// Get and translate those loops, if at least one
	if (countInnerLoops > 0) {
		// get them
		std::vector<SULoopRef> innerLoops(countInnerLoops);

		if (!checkSUResult(
			SUFaceGetInnerLoops(face, countInnerLoops, &innerLoops[0],&countInnerLoops),
			"SUFaceGetInnerLoops",
			__LINE__
		)) return false;

		// iterate them
		for (int j = 0; j < countInnerLoops; j++) {			
			if (!SULoopToLoop(innerLoops[j], polygon->addInnerLoop()))
				return false;
		}//end of iterating inner loops

	} // end of if there is an inner loop

	// clean the polygon
	polygon->clean();
	
	return true;
}

bool SKPReader::SULoopToLoop(SULoopRef suLoop, Loop * loop) 
{
	// First, count vertices
	size_t vertexCount;
	if (!checkSUResult(
		SULoopGetNumVertices(suLoop,&vertexCount),
		"SULoopGetNumVertices", 
		__LINE__
	)) return false;

	
	// Second, retrieve vertices
	std::vector < SUVertexRef > vertices(vertexCount);	
	
	if (!checkSUResult(
		SULoopGetVertices(suLoop, vertexCount, &vertices[0], &vertexCount),
		"SULoopGetVertices",
		__LINE__
	)) return false;


	// Third, translate each vertex
	for (int i = 0; i < vertexCount; i++) {
		SUPoint3D position;
		if (!checkSUResult(
			SUVertexGetPosition(vertices[i], &position),
			"SUVertexGetPosition",
			__LINE__
		)) return false;

		loop->addVertex(new Point3D(TO_M(position.x),TO_M(position.y),TO_M(position.z)));		
	}

	return true;
}

bool SKPReader::getSUFaceName(SUFaceRef face, std::string * name) 
{
	return getSUEntityName(SUFaceToEntity(face), name);
}

bool SKPReader::getSUFaceLayerName(SUFaceRef face, std::string * name) 
{	
	return getSUDrawingElementLayerName( SUFaceToDrawingElement(face), name);
}

bool SKPReader::getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name) 
{
	SULayerRef layer = SU_INVALID;
	if (!checkSUResult(
		SUDrawingElementGetLayer(element,&layer),
		"SUDrawingElementGetLayer",
		__LINE__
	)) return false;

	// Create string
	SUStringRef layerName = SU_INVALID;
	if (!checkSUResult(
		SUStringCreate(&layerName),
		"SUStringCreate",
		__LINE__
	)) return false;

	// retrieve the value
	if (!checkSUResult(
		SULayerGetName(layer,&layerName),
		"SULayerGetName",
		__LINE__
	)) return false;

	// get final length
	
	return SUStringtoString(layerName,name,true);
};

bool SKPReader::getSUEntityName(SUEntityRef entity, std::string * name) 
{
	SUTypedValueRef suValue = SU_INVALID;
	if (getValueFromEntityGHDictionary(entity, SKP_NAME, &suValue)) {
		// There was, indeed, a Grounghog name
		if (!getFromSUTypedValue(suValue, name,true))
			return false;

		return true;
	}

	// else, retrieve ID.
	int32_t id = getEntityID(entity);
	if (id < 0)
		return false;
	*name = std::to_string(id);
	return true;

} // end of Get Entity Name


bool SKPReader::bulkComponentInstancesIntoVector(std::vector <ComponentInstance * > * dest, SUEntitiesRef  entities) {

  /* LOAD THE COMPONENT INSTANCES FIRST */	
	size_t instanceCount = 0;
	if (!checkSUResult(
		SUEntitiesGetNumInstances(entities, &instanceCount),
		"SUEntitiesGetNumInstances",
		__LINE__
	)) return false;

	if (instanceCount != 0) {		
	  std::vector<SUComponentInstanceRef> instances(instanceCount);
	  if (!checkSUResult(
		  SUEntitiesGetInstances(entities, instanceCount, &instances[0], &instanceCount),
		  "SUEntitiesGetInstances",
		  __LINE__
	  )) return false;

	  // fill layers with the instances
	  for (size_t i = 0; i < instanceCount; i++) {
		  // get name of layer
		  SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instances[i]);
		  std::string layerName;
		  if (!getSUDrawingElementLayerName(drawingElement, &layerName))
			  return false;
		
		  addComponentInstanceToVector(dest, instances[i]);

	  }
	}

    /* THEN LOAD THE GROUPS */
    size_t groupInstanceCount = 0;
    if (!checkSUResult(
      SUEntitiesGetNumGroups(entities, &groupInstanceCount),
      "SUEntitiesGetNumInstances",
      __LINE__
    )) return false;


    if (groupInstanceCount != 0) {
      
      std::vector<SUGroupRef> groups(groupInstanceCount);
      if (!checkSUResult(
        SUEntitiesGetGroups(entities, groupInstanceCount, &groups[0], &groupInstanceCount),
        "SUEntitiesGetInstances",
        __LINE__
      )) return false;

      // fill layers with the instances
      for (size_t i = 0; i < groupInstanceCount; i++) {
        // Get drawing element
        SUGroupRef group = groups[i];
        SUComponentInstanceRef instance = SUGroupToComponentInstance(group);
        SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instance);
      
        // get name of layer
        std::string layerName;
        if (!getSUDrawingElementLayerName(drawingElement, &layerName))
          return false;

        addComponentInstanceToVector(dest, instance);

      }
    }

	return true;
}

bool SKPReader::fillComponentInstanceLocation(ComponentInstance * instance, SUComponentInstanceRef suInstance) {
	
	SUTransformation transform;

	if (!checkSUResult(
		SUComponentInstanceGetTransform(suInstance, &transform),
		"SUComponentInstanceGetTransform",
		__LINE__
	)) return false;

	instance->setX(TO_M(transform.values[12]));
	instance->setY(TO_M(transform.values[13]));
	instance->setZ(TO_M(transform.values[14]));
	
	double rx = atan2(-transform.values[9], transform.values[10]);
	double c2 = sqrt(transform.values[0] * transform.values[0] + transform.values[4] * transform.values[4]);
	double ry = atan2(transform.values[8], c2);
	double rz = atan2(-transform.values[4], transform.values[0]);

	instance->setRotationX(TO_DEGREE(rx));
	instance->setRotationY(TO_DEGREE(ry));
	instance->setRotationZ(TO_DEGREE(rz));

	// We rotate then move... how about scaling first, then 
	//  bringing the origin back to the original origin and then 
	//  rotating and moving?
	instance->setScale(1);

	return true;
}


bool SKPReader::getSUFaceLabel(SUFaceRef face, std::string * name)
{
	return getSUEntityLabel(SUFaceToEntity(face), name);
}


bool SKPReader::getSUEntityLabel(SUEntityRef entity, std::string * name)
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!getValueFromEntityGHDictionary(entity, SKP_LABEL, &suValue))
		return false;

	if (!getFromSUTypedValue(suValue, name,true))
		return false;

	return true;
}

bool SKPReader::addWorkplaneToModel(SUFaceRef suFace) {

	// get the name of the face
	std::string name;
	bool hasName = getSUFaceName(suFace, &name);
	if (!hasName) {
		FATAL(errorMessage,"Invalid workplane: has no name");
		return false;
	}

	// Build the polygon
	Polygon3D * polygon = new Polygon3D();
	if (!SUFaceToPolygon3D(suFace, polygon))
		return false;

	model->addPolygonToWorkplane(&name, polygon);
	return true;
}

bool SKPReader::addWindowToModel(SUFaceRef suFace)
{
	// get the name of the face
	std::string name;
	if (!getSUFaceName(suFace, &name)) { // this will allways put something
		FATAL(errorMessage,"Impossible to get name from Window");
		return false;
	}
	
	// Create the window group string
	std::string winGroup;

	// Check if it has a Window Group
	SUTypedValueRef suWinGroup = SU_INVALID;
	if (getValueFromEntityGHDictionary(SUFaceToEntity(suFace), SKP_WINGROUP, &suWinGroup)) {
		// If it has, set the windowgroup name to that...
      if (!getFromSUTypedValue(suWinGroup, &winGroup, true)) {
		  FATAL(errorMessage,"Error when trying to retrieve Window Group name");
        }
        
	}
	else {
		// if not, set the name of the window.
		winGroup = name;
	}

	// Create the face
	Face * face = SUFaceToFace(suFace);
	if (face == NULL)
		return false;
		
	// Add the window
	model->addWindowToGroup(&winGroup, face);
	return true;
}


int32_t SKPReader::getEntityID(SUEntityRef entity)
{
	// if not, check for a SketchUp Assigned name		
	// else, set ID
	int32_t id;
	if (!checkSUResult(
		SUEntityGetID(entity, &id),
		"SUEntityGetID",
		__LINE__
	)) {
		FATAL(errorMessage,"Error when retrieving entity ID");
		return -1;
	}

	return id;
}


bool SKPReader::getValueFromEntityGHDictionary(SUEntityRef entity, const char * key, SUTypedValueRef * value)
{
	// check how many dictionaries
	size_t dictionaryCount;
	if (!checkSUResult(
		SUEntityGetNumAttributeDictionaries(entity, &dictionaryCount),
		"SUEntityGetNumAttributeDictionaries",
		__LINE__
	)) return false;

	// if there are no dictionaries, then return.
	if (dictionaryCount == 0)
		return false;

	//retrieve dictionaries
	std::vector <SUAttributeDictionaryRef> dictionaries(dictionaryCount);
	if (!checkSUResult(
		SUEntityGetAttributeDictionaries(entity, dictionaryCount, &dictionaries[0], &dictionaryCount),
		"SUEntityGetAttributeDictionaries",
		__LINE__
	)) return false;

	// Check if it has a Groundhog dictionary
	for (int i = 0; i < dictionaryCount; i++) {
		SUStringRef dictionaryName = SU_INVALID;
		if (!checkSUResult(
			SUStringCreate(&dictionaryName),
			"SUStringCreate",
			__LINE__
		)) return false;

		if (!checkSUResult(
			SUAttributeDictionaryGetName(dictionaries[i], &dictionaryName),
			"SUAttributeDictionaryGetName",
			__LINE__
		)) return false;


		int result;
		if (!checkSUResult(
			SUStringCompare(dictionaryName, groundhogDictionaryName, &result),
			"SUStringCompare", 
			__LINE__
		)) return false;

		if (!checkSUResult(
			SUStringRelease(&dictionaryName),
			"SUStringRelease",
			__LINE__
		)) return false;

		if (result == 0) { // then, it is a Groundhog dictionary

			//retrieve the value			
			if (!checkSUResult(
				SUTypedValueCreate(value),
				"SUTypedValueCreate",
				__LINE__
			)) return false;


			SUResult res = SUAttributeDictionaryGetValue(dictionaries[i], key, value);
			if (res == SU_ERROR_NONE || res == SU_ERROR_NO_DATA) {
				return res == SU_ERROR_NONE;
			}
			else {
				checkSUResult(
					res,
					"SUAttributeDictionaryGetValue",
					__LINE__
				);
				return false;
			}			
		}
	}
	return false; //should not reach here.
}

bool SKPReader::SUStringtoString(SUStringRef suString, std::string * string, bool fix)
{

	size_t stringLength;
	if (!checkSUResult(
		SUStringGetUTF8Length(suString, &stringLength),
		"SUStringGetUTF8Length",
		__LINE__
	)) return false;

	std::string utf8String;
	utf8String.reserve(stringLength);

	if (!checkSUResult(
		SUStringGetUTF8(suString, stringLength, &utf8String[0], &stringLength),
		"SUStringGetUTF8",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUStringRelease(&suString),
		"SUStringRelease",
		__LINE__
	)) return false;

	string->reserve(stringLength);
	string->resize(stringLength);

	utf8toASCII(&utf8String[0], stringLength, &(*string)[0], &stringLength);
	
	if(fix)
		fixString(&(*string)[0], stringLength);

	return true;

}

bool SKPReader::getFromSUTypedValue(SUTypedValueRef suValue, std::string * value, bool fix)
{
	// Create a SU String
	SUStringRef suString = SU_INVALID;
	if (!checkSUResult(
		SUStringCreate(&suString),
		"SUStringCreate",
		__LINE__
	)) return false;

	// Retrieve the String
	if (!checkSUResult(
		SUTypedValueGetString(suValue, &suString),
		"SUTypedValueGetString",
		__LINE__
	)) {
		SUStringRelease(&suString);
		return false;
	}
	SUStringtoString(suString, value,fix);
	
	return true;
}


Material * SKPReader::addMaterialToModel(SUMaterialRef material)
{	
	
	// create the json
	json j;

	SUEntityRef entityMat = SUMaterialToEntity(material);

    // Check if the material exists
    std::string name;
    if (!getSUMaterialName(material, &name))
      return NULL;

    Material * m = model->hasMaterial(&name);
    if (m != NULL)
      return m;

	// Check if it is a Radiance Material
	std::string label;
    
	if (getSUEntityLabel(entityMat, &label)) {

		// it is a Radiance material
		if(label != SKP_MATERIAL){
			FATAL(errorMessage,"Material with weird label " + label);
			return NULL;
		}		
		
		// get the value
		std::string value;
		if (!getGHValueFromEntity(entityMat, &value,false)) {
			return NULL;
		}
		
		j = json::parse(value);
		j["name"] = name;

	}
	else {
		guessMaterial(material, &j);
	}

	// Add the material and return
	return model->addMaterial(&j);
}

bool SKPReader::getGHValueFromEntity(SUEntityRef entity, std::string * value, bool fix)
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!getValueFromEntityGHDictionary(entity, SKP_VALUE, &suValue))
		return false;

	if (!getFromSUTypedValue(suValue, value,fix))
		return false;

	return true;
}

bool SKPReader::guessMaterial(SUMaterialRef material, json * j)
{
	std::string name;

	if (!getSUMaterialName(material,&name))
		return false;

	WARN(message,"Guessing material " + name);
	
	// set the name
	(*j)["name"] = name;

	// set alpha
	double alpha;
	if (!checkSUResult(
		SUMaterialGetOpacity(material,&alpha),
		"SUMaterialGetOpacity", __LINE__
	)) return false;

	(*j)["alpha"] = alpha;

	// set type
	(*j)["class"] = (alpha < 1 ? "glass" : "plastic");

	// set color
	SUColor color;
	if (!checkSUResult(
		SUMaterialGetColor(material, &color),
		"SUMaterialGetColor", __LINE__
	)) return false;

	(*j)["color"] = {color.red, color.green, color.blue};

	if (alpha < 1) {
		(*j)["rad"] = "void glass %MATERIAL_NAME% 0 0 3 " + std::to_string(alpha*color.red / 255.0) + " " + std::to_string(alpha*color.green / 255.0) + " " + std::to_string(alpha*color.blue / 255.0) ;
	}
	else {
		(*j)["rad"] = "void plastic %MATERIAL_NAME% 0 0 5 " + std::to_string(color.red / 255.0) + " " + std::to_string(color.green / 255.0) + " " + std::to_string(color.blue / 255.0) + " 0 0";
	}
	return true;
}

bool SKPReader::getFaceMaterial(SUFaceRef face, SUMaterialRef * mat)
{
	// Check the front material
	SUMaterialRef frontMat;
	SUResult frontRes = SUFaceGetFrontMaterial(face, &frontMat);
	if (frontRes == SU_ERROR_NONE) { // it has front material
		// Check if it has physical information 
		std::string label;
		if (getSUEntityLabel(SUMaterialToEntity(frontMat), &label)) {
			// if it has, do not bother and return.
			if (label != SKP_MATERIAL) {
				FATAL(errorMessage,"Weird material label "+label);
				return false;
			}

			*mat = frontMat;
			return true; 
		}
	}

	// Check the back material
	SUMaterialRef backMat;
	SUResult backRes = SUFaceGetBackMaterial(face, &backMat);
	if (backRes == SU_ERROR_NONE) { // it has back material
		// Check if it has physical information 
		std::string label;
		if (getSUEntityLabel(SUMaterialToEntity(backMat), &label)) {
			if (label != SKP_MATERIAL) {
				FATAL(errorMessage,"Weird material label " + label);
				return false;
			}
			*mat = backMat;
			return true;
		}
	}

	// There was no Radiance material... prioritize the front one.
	if (frontRes == SU_ERROR_NONE) {
		*mat = frontMat;	
		return true;
	}
	else if (backRes == SU_ERROR_NONE) {
		*mat = backMat;	
		return true;
	}
	
	return false;
}


Face * SKPReader::SUFaceToFace(SUFaceRef suFace)
{

	// build the polygon
	Polygon3D * polygon = new Polygon3D();
	if (!SUFaceToPolygon3D(suFace, polygon))
		return NULL;

	// get the name of the face
	std::string name;

	if (!getSUFaceName(suFace, &name)) // this will allways put something
		return NULL;

	//build the face
	Face * face = new Face(&name);
	face->setPolygon(polygon);	

	// retrieve and set the material
	SUMaterialRef suMat;
	Material * mat;
	if (getFaceMaterial(suFace, &suMat)) {
		// add this material.
		mat = addMaterialToModel(suMat);
	}
	else {
		// add default material
		mat = model->addDefaultMaterial();
	}

	face->setMaterial(mat);

	return face;
}

bool SKPReader::getSUMaterialName(SUMaterialRef material, std::string * name)
{
	SUStringRef suName = SU_INVALID;

	if (!checkSUResult(
		SUStringCreate(&suName),
		"SUStringCreate",
		__LINE__
	)) return false;

	if (!checkSUResult(
		SUMaterialGetName(material, &suName),
		"SUMaterialGetName", __LINE__
	)) return false;
	
	// this fixes the name as well, and releases the SUString
	if (!SUStringtoString(suName, name,true))
		return false;

	return true;
}

bool SKPReader::addPhotosensorsToModel(SUComponentDefinitionRef definition)
{
	// count instances
	size_t numInstances;
	if (!checkSUResult(
		SUComponentDefinitionGetNumInstances(definition, &numInstances),
		"SUComponentDefinitionGetNumInstances",
		__LINE__
	)) return false;

	if (numInstances == 0)
		return true;

	std::vector <SUComponentInstanceRef> instances(numInstances);

	// Get the actual instances
	if (!checkSUResult(
		SUComponentDefinitionGetInstances(definition,numInstances,&instances[0],&numInstances),
		"SUComponentDefinitionGetNumInstances",
		__LINE__
	)) return false;

	for (size_t i = 0; i < numInstances; i++) {

		//get the name
		std::string name;
		if (!getSUEntityName(SUComponentInstanceToEntity(instances[i]), &name))
			return false;

		// create the photosensor object
		Photosensor * ph = new Photosensor(name);

		// Get the transformation
		SUTransformation transform;

		if (!checkSUResult(
			SUComponentInstanceGetTransform(instances[i], &transform),
			"SUComponentInstanceGetTransform",
			__LINE__
		)) return false;

		// Set the position
		double x = TO_M(transform.values[12]);
		double y = TO_M(transform.values[13]);
		double z = TO_M(transform.values[14]);
		ph->setPosition(Point3D(x, y, z));

		// set the direction
		ph->setDirection(Vector3D(transform.values[8], transform.values[9], transform.values[10]));

		// add the photosensor
		model->addPhotosensor(ph);
	}

	return true;
}


bool SKPReader::getValueFromModelGHDictionary(const char * key, SUTypedValueRef * value)
{
	// check how many dictionaries
	size_t dictionaryCount;
	if (!checkSUResult(
		SUModelGetNumAttributeDictionaries(suModel, &dictionaryCount),
		"SUModelGetNumAttributeDictionaries",
		__LINE__
	)) return false;

	// if there are no dictionaries, then return.
	if (dictionaryCount == 0)
		return false;

	//retrieve dictionaries
	std::vector <SUAttributeDictionaryRef> dictionaries(dictionaryCount);
	if (!checkSUResult(
		SUModelGetAttributeDictionaries(suModel, dictionaryCount, &dictionaries[0], &dictionaryCount),
		"SUModelGetAttributeDictionaries",
		__LINE__
	)) return false;

	// Check if it has a Groundhog dictionary
	for (int i = 0; i < dictionaryCount; i++) {
		SUStringRef dictionaryName = SU_INVALID;
		if (!checkSUResult(
			SUStringCreate(&dictionaryName),
			"SUStringCreate",
			__LINE__
		)) return false;

		if (!checkSUResult(
			SUAttributeDictionaryGetName(dictionaries[i], &dictionaryName),
			"SUAttributeDictionaryGetName",
			__LINE__
		)) return false;


		int result;
		if (!checkSUResult(
			SUStringCompare(dictionaryName, groundhogDictionaryName, &result),
			"SUStringCompare",
			__LINE__
		)) return false;

		if (!checkSUResult(
			SUStringRelease(&dictionaryName),
			"SUStringRelease",
			__LINE__
		)) return false;

		if (result == 0) { // then, it is a Groundhog dictionary

			//retrieve the value			
			if (!checkSUResult(
				SUTypedValueCreate(value),
				"SUTypedValueCreate",
				__LINE__
			)) return false;


			SUResult res = SUAttributeDictionaryGetValue(dictionaries[i], key, value);
			if (res == SU_ERROR_NONE || res == SU_ERROR_NO_DATA) {
				return res == SU_ERROR_NONE;
			}
			else {
				checkSUResult(
					res,
					"SUAttributeDictionaryGetValue",
					__LINE__
				);
				return false;
			}
		}
	}
	return false; //should not reach here.
}


bool SKPReader::loadWeather()
{

	SUTypedValueRef suWeather = SU_INVALID;
	if (!getValueFromModelGHDictionary(SKP_WEATHER, &suWeather))
		return true; // return if no weather or error.

	std::string value;

	if (!getFromSUTypedValue(suWeather, &value,false))
		return false;
	
	json j = json::parse(value);

	Location * loc = model->getLocation();

	return loc->fillWeatherFromJSON(&j);
}

#endif
