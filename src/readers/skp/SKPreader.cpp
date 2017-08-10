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


#include "./SKPreader.h"
#include "../../config_constants.h"
#include "../../common/utilities/io.h"
#include "../../common/utilities/stringutils.h"
#include "../../groundhogmodel/groundhogmodel.h"
#include "../../groundhogmodel/src/face.h"
#include "../../common/geometry/polygon.h"

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

#include <vector>
#include <string>
#include <iostream>
#include <fstream>



SKPReader::SKPReader() 
{
	DEBUG_MSG("Creating SKPReader");

	//initialize API
	SUInitialize();

	suModel = SU_INVALID;

	groundhogDictionaryName = SU_INVALID;
	checkSUResult(
		SUStringCreateFromUTF8(&groundhogDictionaryName, SKP_GROUNDHOG_DICTIONARY),
		"SUStringCreateFromUTF8",
		"initializing SKPReader"
	);
};

SKPReader::~SKPReader() 
{
	DEBUG_MSG("Destroying SKPReader");
	
	//release dictionary
	checkSUResult(
		SUStringRelease(&groundhogDictionaryName),
		"SUStringRelease",
		"destroying SKP reader"
	);
	
	// Must release the model or there will be memory leaks
	SUModelRelease(&suModel);
	
	// Always terminate the API when done using it
	SUTerminate();
};


bool SKPReader::checkSUResult(SUResult res, std::string functionName, std::string location) 
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

	fatal("function '" + functionName + "' returned '" + error + "' when " + location);
	return false;
}


bool SKPReader::parseSKPModel(std::string inputFile, GroundhogModel * modelRef, bool verbose)
{		
	//Load model
	if (!checkSUResult(
		SUModelCreateFromFile(&suModel, inputFile.c_str()),
		"SUModelCreateFromFile",
		"Loading model"
	)) return false;

	// Load layers	
	if(!loadLayers(modelRef, verbose))
		return false;

	// load components
	if (!loadComponentDefinitions(modelRef, verbose))
		return false;

	// Fill layers and load related surfaces, discrimitating between
	// workplanes, windows, etc.
	if (!loadLayersContent(modelRef, verbose))
		return false;

	// Load views
	if (!loadViews(modelRef, verbose))
		return false;


	// Load model info (location, date, etc).
	if (!loadModelInfo(modelRef, verbose))
		return false;
	
	// Load ...


	
	return true;
};


bool SKPReader::getStringFromShadowInfo(SUShadowInfoRef shadowInfo, char * key, char * value) 
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!checkSUResult(
		SUTypedValueCreate(&suValue),
		"SUTypedValueCreate",
		"Getting string from shadow info"
	)) return false;

	if (!checkSUResult(
		SUShadowInfoGetValue(shadowInfo, key, &suValue),
		"SUShadowInfoGetValue",
		"Getting string from shadow info"
	)) return false;

	SUStringRef suString = SU_INVALID;

	if (!checkSUResult(
		SUStringCreate(&suString),
		"SUStringCreate",
		"Getting string from shadow info"
	)) return false;

	if (!checkSUResult(
		SUTypedValueGetString(suValue, &suString),
		"SUTypedValueGetString",
		"Getting string from shadow info"
	)) return false;

	char cValue[MAX_STRING_LENGTH];
	size_t cValueLength;

	if (!checkSUResult(
		SUStringGetUTF8Length(suString, &cValueLength),
		"SUStringGetUTF8Length",
		"Getting string from shadow info"
	)) return false;

	if (!checkSUResult(
		SUStringGetUTF8(suString,cValueLength,cValue,&cValueLength),
		"SUStringSetUTF8",
		"Getting string from shadow info"
	)) return false;

	if (!checkSUResult(
		SUTypedValueRelease(&suValue),
		"SUTypedValueGetDouble",
		"Getting string from shadow info"
	)) return false;

	if (!checkSUResult(
		SUStringRelease(&suString),
		"SUStringRelease",
		"Getting latitude from shadow info"
	)) return false;

	utf8toASCII(cValue, cValueLength, value, &cValueLength);

	return true;
}

bool SKPReader::getDoubleFromShadowInfo(SUShadowInfoRef shadowInfo,char * key, double * value) 
{
	SUTypedValueRef suValue= SU_INVALID;
	if (!checkSUResult(
		SUTypedValueCreate(&suValue),
		"SUTypedValueCreate",
		"Getting double from shadow info"
	)) return false;

	if (!checkSUResult(
		SUShadowInfoGetValue(shadowInfo, key, &suValue),
		"SUShadowInfoGetValue",
		"Getting double from shadow info"
	)) return false;

	if (!checkSUResult(
		SUTypedValueGetDouble(suValue, value),
		"SUTypedValueGetDouble",
		"Getting double from shadow info"
	)) return false;

	if (!checkSUResult(
		SUTypedValueRelease(&suValue),
		"SUTypedValueGetDouble",
		"Getting double from shadow info"
	)) return false;

	return true;
}


bool SKPReader::getTimeFromShadowInfo(SUShadowInfoRef shadowInfo, int64_t * value) 
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!checkSUResult(
		SUTypedValueCreate(&suValue),
		"SUTypedValueCreate",
		"Getting time from shadow info"
	)) return false;

	if (!checkSUResult(
		SUShadowInfoGetValue(shadowInfo, "ShadowTime", &suValue),
		"SUShadowInfoGetValue",
		"Getting time from shadow info"
	)) return false;

	if (!checkSUResult(
		SUTypedValueGetTime(suValue, value),
		"SUTypedValueGetTime",
		"Getting time from shadow info"
	)) return false;

	if (!checkSUResult(
		SUTypedValueRelease(&suValue),
		"SUTypedValueGetDouble",
		"Getting time from shadow info"
	)) return false;

	return true;
}

bool SKPReader::loadModelInfo(GroundhogModel * model, bool verbose) 
{
	// load north correction
	double northC;
	if (!checkSUResult(
		SUModelGetNorthCorrection(suModel, &northC),
		"SUModelGetNorthCorrection",
		"Getting Model's north correction"
	)) return false;
	model->setNorthCorrection(northC);

	// load date, longitude and latitude.
	SUShadowInfoRef shadowInfo;
	if (!checkSUResult(
		SUModelGetShadowInfo(suModel, &shadowInfo),
		"SUModelGetShadowInfo",
		"Getting Model's shadow info"
	)) return false;

	// Set latitude
	double latitude;
	getDoubleFromShadowInfo(shadowInfo, "Latitude", &latitude);
	model->setLatitude(latitude);

	// Set longitude
	double longitude;
	getDoubleFromShadowInfo(shadowInfo, "Longitude", &longitude);
	model->setLongitude(longitude);

	// Set time zone
	double timeZone;
	getDoubleFromShadowInfo(shadowInfo, "TZOffset", &timeZone);
	model->setTimeZone(timeZone);
	
	//set city
	char city[MAX_STRING_LENGTH];
	getStringFromShadowInfo(shadowInfo, "City", city);
	model->setCity(std::string(city));

	//set country
	char country[MAX_STRING_LENGTH];
	getStringFromShadowInfo(shadowInfo, "Country", country);
	model->setCountry(std::string(country));

	// set time
	int64_t epoch;
	getTimeFromShadowInfo(shadowInfo, &epoch);
	Date * date = new Date(epoch); //  + timeZone*60*60
	model->setMonth(date->getMonth());
	model->setHour(date->getDay());
	model->setHour(date->getHour());
	model->setMinute(date->getMinute());
	delete date;

	return true;
}

bool SKPReader::SUCameraToView(std::string viewName, SUCameraRef camera, View * view) 
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
		"Getting camera orientation"
	)) return false;
	view->setViewPoint(new Point3D(TO_M(position.x), TO_M(position.y), TO_M(position.z)));
	view->setViewUp(new Vector3D(up.x, up.y, up.z));

	// get and set the view direction
	SUVector3D direction;
	if (!checkSUResult(
		SUCameraGetDirection(camera, &direction),
		"SUCameraGetDirection",
		"Getting camera view direction"
	)) return false;
	view->setViewDirection(new Vector3D(direction.x, direction.y, direction.z));

	// get and set type
	bool perspective;
	if (!checkSUResult(
		SUCameraGetPerspective(camera, &perspective),
		"SUCameraGetPerspective",
		"Gettin camera perspective"
	)) return false;

	int type = perspective ? PERSPECTIVE_VIEW : PARALLEL_VIEW;
	view->setViewType(type);

	double viewHeight;
	if (type == PERSPECTIVE_VIEW) {
		if (!checkSUResult(
			SUCameraGetPerspectiveFrustumFOV(camera, &viewHeight),
			"SUCameraGetPerspectiveFrustumFOV",
			"Gettin perspective camera view height"
		)) return false;
	}
	else { // PARALLEL
		if (!checkSUResult(
			SUCameraGetOrthographicFrustumHeight(camera, &viewHeight),
			"SUCameraGetOrthographicFrustumHeight",
			"Gettin parallel camera view height"
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
		fatal("SU_ERROR_INVALID_INPUT when trying to get aspect ratio of view");
		break;
	case SU_ERROR_NO_DATA:
		// the camera uses the screen aspect ratio... will assume the following
		aspectRatio = 16.0 / 9.0;
		break;
	case SU_ERROR_NULL_POINTER_OUTPUT:
		fatal("SU_ERROR_NULL_POINTER_OUTPUT when trying to get aspect ratio of view");
		break;
	}
	

	view->setViewHorizontal(aspectRatio * viewHeight);

	//return
	return true;
}

bool SKPReader::SUViewToView(SUSceneRef suView, View * view) 
{
	// get the name of the view
	
	SUStringRef viewName = SU_INVALID;
	if (!checkSUResult(
		SUStringCreate(&viewName),
		"SUStringCreate",
		"Creating View name SUStringRef"
	)) return false;
	
	if (!checkSUResult(
		SUSceneGetName(suView, &viewName),
		"SUSceneGetName",
		"Getting view name"
	)) return false;
	
	size_t stringLength;
	if (!checkSUResult(
		SUStringGetUTF8Length(viewName, &stringLength),
		"SUStringGetUTF8Length",
		"Getting view name length"
	)) return false;
	
	char cViewName[MAX_STRING_LENGTH];
	if (!checkSUResult(
		SUStringGetUTF8(viewName, stringLength, cViewName, &stringLength),
		"SUStringGetUTF8",
		"Getting view name"
	)) return false;
	
	if (!checkSUResult(
		SUStringRelease(&viewName),
		"SUStringRelease",
		"Releasing SUStringRef of view name"
	)) return false;
	
	char asciiViewName[MAX_STRING_LENGTH];
	utf8toASCII(cViewName, stringLength, asciiViewName, &stringLength);
	fixString(asciiViewName, stringLength);


	// Get the camera
	SUCameraRef camera = SU_INVALID;
	if (!checkSUResult(
		SUSceneGetCamera(suView,&camera),
		"SUSceneGetCamera",
		"Gettin view camera"
	)) return false;

	if (!SUCameraToView(std::string(asciiViewName), camera, view)) {
		SUCameraRelease(&camera);
		return false;
	}
		
	return true;

}

bool SKPReader::loadViews(GroundhogModel * model, bool verbose) 
{
	
	// get the current view
	SUCameraRef activeCamera;

	if (!checkSUResult(
		SUModelGetCamera(suModel, &activeCamera),
		"SUModelGetActiveScene",
		"Retrieving active view"
	)) return false;

	View * activeView = new View();
	SUCameraToView("view", activeCamera, activeView);
	model->addView(activeView);

	// load stored views
	size_t countScenes;

	if (!checkSUResult(
		SUModelGetNumScenes(suModel, &countScenes),
		"SUModelGetNumScenes",
		"Retrieving number of views"
	)) return false;
	
	if (countScenes == 0) {
		return true;
	}
	std::vector<SUSceneRef> views(countScenes);

	if (!checkSUResult(
		SUModelGetScenes(suModel, countScenes, &views[0],&countScenes),
		"SUModelGetScenes",
		"Retrieving stored views"
	)) return false;

	for (size_t i = 0; i < countScenes; i++) {
		View * storedView = new View();
		SUViewToView(views[i], storedView);
		model->addView(storedView);
	}

	return true;
}

bool SKPReader::loadLayers(GroundhogModel * model, bool verbose) 
{

	// count layers
	size_t countLayers = 0;
	size_t retrievedLayers = 0;
	if(!checkSUResult(
		SUModelGetNumLayers(suModel, &countLayers),
		"SUModelGetNumLayers",
		"Retrieving layers"
	)) return false;

	//get those layers
	std::vector<SULayerRef> layers(countLayers);
	if(!checkSUResult(
		SUModelGetLayers(suModel, countLayers, &layers[0], &countLayers),
		"SUModelGetLayers",
		"retrieving layers"
	)) return false;

	// inform layer status
	inform("Counted layers: " + size_tToString(countLayers), verbose);

	// create and load the layers
	for (unsigned int i = 0; i < layers.size(); i++) {		
		SUStringRef layerName = SU_INVALID;		
		if(!checkSUResult(
			SUStringCreate(&layerName),
			"SUStringCreate",
			"Creating Layer name SUStringRef"
		)) return false;

		if(!checkSUResult(
			SULayerGetName(layers[i], &layerName),
			"SULayerGetName",
			"Getting layer name"
		)) return false;

		size_t stringLength;
		if(!checkSUResult(
			SUStringGetUTF8Length(layerName, &stringLength),
			"SUStringGetUTF8Length",
			"Getting layer name length"
		)) return false;

		char cLayerName[MAX_STRING_LENGTH];		
		if(!checkSUResult(
			SUStringGetUTF8(layerName,	stringLength, cLayerName, &stringLength),
			"SUStringGetUTF8",
			"Getting layer name"
		)) return false;
		
		if(!checkSUResult(
			SUStringRelease(&layerName),
			"SUStringRelease",
			"Releasing SUStringRef of layer name"
		)) return false;

		char asciiLayerName[MAX_STRING_LENGTH];
		utf8toASCII(cLayerName, stringLength, asciiLayerName, &stringLength);
		fixString(asciiLayerName, stringLength);

		model->addLayer(&std::string(asciiLayerName));
		inform("Layer " + std::string(asciiLayerName) + " added",verbose);
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
		"Retrieving name of component definition"
	)) return false;

	if (!checkSUResult(
		SUComponentDefinitionGetName(definition, &suComponentName),
		"SUComponentInstanceGetName",
		"Retrieving name of component definition"
	)) return false;

	size_t cStringLength;
	if (!checkSUResult(
		SUStringGetUTF8Length(suComponentName, &cStringLength),
		"SUStringGetUTF8Length",
		"Retrieving name of component definition"
	)) return false;

	char cComponentName[MAX_STRING_LENGTH];
	if (!checkSUResult(
		SUStringGetUTF8(suComponentName, cStringLength, cComponentName, &cStringLength),
		"SUStringGetUTF8Length",
		"Retrieving name of component definition"
	)) return false;

	if (!checkSUResult(
		SUStringRelease(&suComponentName),
		"SUStringRelease",
		"Retrieving name of component definition"
	)) return false;

	char asciiComponentName[MAX_STRING_LENGTH];
	utf8toASCII(cComponentName, cStringLength, asciiComponentName, &cStringLength);
	fixString(asciiComponentName, cStringLength);


	*name = std::string(asciiComponentName);
	return true;
}


bool SKPReader::addComponentInstanceToVector(std::vector <ComponentInstance * > * dest, SUComponentInstanceRef suComponentInstance, GroundhogModel * model) 
{

	// get definition
	SUComponentDefinitionRef definition;
	if (!checkSUResult(
		SUComponentInstanceGetDefinition(suComponentInstance, &definition),
		"SUComponentInstanceGetDefinition",
		"Retrieving component instance when filling layers"
	)) return false;

	std::string definitionName;
	getSUComponentDefinitionName(definition, &definitionName);

	// get the definition
	ComponentDefinition * definitionRef = model->getComponentDefinitionByName(&definitionName);
	if (definitionRef == NULL) {
		fatal("Impossible to find " + definitionName + " when adding an instance... ignoring instance");
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

bool SKPReader::addFaceToVector(std::vector <Face * > * dest, SUFaceRef suFace) 
{

	// build the polygon
	Polygon3D * polygon = new Polygon3D();
	if (!SUFaceToPolygon3D(suFace, polygon))
		return false;

	// get the name of the face
	std::string name;
	getSUFaceName(suFace, &name); // this will allways put something

	//build the face
	Face * face = new Face(name);
	face->setPolygon(polygon);

	//add the component
	dest->push_back(face);

	return true;
}

bool SKPReader::bulkFacesIntoVector(std::vector <Face * > * dest, SUEntitiesRef entities) 
{

	// count faces in these entities
	size_t numFaces = 0;
	if (!checkSUResult(
		SUEntitiesGetNumFaces(entities, &numFaces),
		"SUEntitiesGetNumFaces",
		"Counting faces in component definitions"
	)) return false;

	// get the faces
	std::vector<SUFaceRef> faces(numFaces);
	if (!checkSUResult(
		SUEntitiesGetFaces(entities, numFaces, &faces[0], &numFaces),
		"SUEntitiesGetFaces",
		"Retrieving faces from component entities"
	)) return false;

	// import faces
	for (size_t i = 0; i < numFaces; i++) {
		addFaceToVector(dest, faces[i]);
	}
	return true;
}

bool SKPReader::loadComponentDefinition(SUComponentDefinitionRef definition, GroundhogModel * model) 
{
	//get the name
	std::string definitionName;
	if (!getSUComponentDefinitionName(definition, &definitionName)) {
		warn("Impossible to get name of component");
		return false;
	};

	// get entities
	SUEntitiesRef entities;
	if (!checkSUResult(
		SUComponentDefinitionGetEntities(definition, &entities),
		"SUComponentDefinitionGetEntities",
		"Retrieving entities from component definitions"
	)) return false;

	// Create the definition
	ComponentDefinition * componentDefinition = new ComponentDefinition(&definitionName);

	model->addComponentDefinition(componentDefinition);

	// Load faces
	bulkFacesIntoVector(componentDefinition->getFacesRef(), entities);

	// load instances
	bulkComponentInstancesIntoVector(componentDefinition->getComponentInstancesRef(), entities, model);

	return true;
}

bool SKPReader::loadComponentDefinitions(GroundhogModel * model, bool verbose) 
{
	// count the component definitions
	size_t countDefinitions = 0;
	if (!checkSUResult(
		SUModelGetNumComponentDefinitions(suModel, &countDefinitions),
		"SUModelGetNumComponentDefinitions",
		"Counting component definitions"
	)) return false;

	// return if none
	if (countDefinitions == 0) {
		inform("No component definitions in model",verbose);
		return true; // success
	}
	inform(std::to_string(countDefinitions) + " definitions in model",verbose);

	// get the component definitions
	std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
	if (!checkSUResult(
		SUModelGetComponentDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions),
		"SUModelGetComponentDefinitions",
		"Retrieving component definitions"
	)) return false;

	// Now, load One by One
	for (size_t i = 0; i < countDefinitions; i++) {
		if (!loadComponentDefinition(definitions[i], model)) {
			warn("Impossible to load component definition to model");
			continue;
		}

	}
	return true;
}

bool SKPReader::loadLayersContent(GroundhogModel * model, bool verbose) 
{
	// Get the entity container of the model.
	SUEntitiesRef entities = SU_INVALID;
	if (!checkSUResult(
		SUModelGetEntities(suModel, &entities),
		"SUModelGetEntities",
		"Retrieving entities"
	)) return false;


	// count faces
	size_t faceCount = 0;
	if (!checkSUResult(
		SUEntitiesGetNumFaces(entities, &faceCount),
		"SUModelGetEntities",
		"Retrieving entities"
	)) return false;

	if (faceCount == 0) {
		warn("No faces in model");
		return true; //success, though.
	}

	inform("Counted Faces: " + size_tToString(faceCount), verbose);

	std::vector<SUFaceRef> faces(faceCount);
	if (!checkSUResult(
		SUEntitiesGetFaces(entities, faceCount, &faces[0], &faceCount),
		"SUEntitiesGetFaces",
		"Retrieving faces"
	)) return false;
	
	for (size_t i = 0; i < faceCount; i++) {
		
		// CHECK LABEL OF FACE
		std::string faceLabel;
		bool hasLabel = getSUFaceLabel(faces[i], &faceLabel);
		
		if ( hasLabel ) {
			// if it is workplane
			if (faceLabel == SKP_WORKPLANE) {
				addWorkplaneToModel(faces[i],model);
			}
			else if (faceLabel == SKP_ILLUM) {
			// if it is illum

			}
			else if (faceLabel == SKP_WINDOW) {
			// if it is window

			}
		}
		
		// if has no label (i.e. is geometry face)		
		std::string layerName;		
		if (!getSUFaceLayerName(faces[i],&layerName))
			return false;

		Layer * layerRef = model->getLayerByName(&layerName);
		if (layerRef == NULL) {
			return false;
		}
		addFaceToVector(layerRef->getFacesRef(), faces[i]);

	} // end of iterating faces
	

	// load component instances
	size_t instanceCount;
	if (!checkSUResult(
		SUEntitiesGetNumInstances(entities, &instanceCount),
		"SUEntitiesGetNumInstances",
		"Retrieving faces"
	)) return false;

	if (instanceCount == 0)
		return true;

	std::vector<SUComponentInstanceRef> instances(instanceCount);
	if (!checkSUResult(
		SUEntitiesGetInstances(entities, instanceCount, &instances[0], &instanceCount),
		"SUEntitiesGetInstances",
		"Retrieving model component instances"
	)) return false;

	// fill layers with the instances
	for (size_t i = 0; i < instanceCount; i++) {
		
		// get name of layer
		SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instances[i]);
		std::string layerName;
		if (!getSUDrawingElementLayerName(drawingElement, &layerName)) 
			return false;

		Layer * layerRef = model->getLayerByName(&layerName);
		if (layerRef == NULL) {
			return false;
		}

		addComponentInstanceToVector(layerRef->getComponentInstancesRef(), instances[i],model);

	}

	return true;
} // end of Load Faces

bool SKPReader::SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon) 
{
	const std::string moment = "SUFaceToPolygon3D";
	// get area
	double area;
	if (!checkSUResult(
		SUFaceGetArea(face,&area),
		"SUFaceGetArea",
		moment
	)) return false;
	polygon->setArea(TO_M2(area));
	
	// Get the normal
	SUVector3D normal;
	if (!checkSUResult(
		SUFaceGetNormal(face, &normal),
		"SUFaceGetArea",
		moment
	)) return false;
	polygon->setNormal(new Vector3D(normal.x, normal.y, normal.z));

	// get the outer loop
	SULoopRef suOuterLoop = SU_INVALID;
	if (!checkSUResult(
		SUFaceGetOuterLoop(face,&suOuterLoop),
		"SUFaceGetOuterLoop",
		moment
	)) return false;

	// translate outer loop
	if (!SULoopToLoop(suOuterLoop, polygon->getOuterLoopRef()))
		return false;

	// get number of inner loops
	size_t countInnerLoops;
	if (!checkSUResult(
		SUFaceGetNumInnerLoops(face, &countInnerLoops),
		"SUFaceGetNumInnerLoops",
		moment
	)) return false;

	// Get and translate those loops, if at least one
	if (countInnerLoops > 0) {
		// get them
		std::vector<SULoopRef> innerLoops(countInnerLoops);

		if (!checkSUResult(
			SUFaceGetInnerLoops(face, countInnerLoops, &innerLoops[0],&countInnerLoops),
			"SUFaceGetInnerLoops",
			moment
		)) return false;

		// iterate them
		for (int j = 0; j < countInnerLoops; j++) {			
			if (!SULoopToLoop(innerLoops[j], polygon->addInnerLoop()))
				return false;
		}//end of iterating inner loops

	} // end of if there is an inner loop

	// clean the polygon
	if (!polygon->clean())
		return false;

	return true;
}

bool SKPReader::SULoopToLoop(SULoopRef suLoop, Loop * loop) 
{
	std::string moment = "converting SULoop into Loop";

	// First, count vertices
	size_t vertexCount;
	if (!checkSUResult(
		SULoopGetNumVertices(suLoop,&vertexCount),
		"SULoopGetNumVertices",
		moment
	)) return false;

	
	// Second, retrieve vertices
	std::vector < SUVertexRef > vertices(vertexCount);	
	
	if (!checkSUResult(
		SULoopGetVertices(suLoop, vertexCount, &vertices[0], &vertexCount),
		"SULoopGetVertices",
		moment
	)) return false;


	// Third, translate each vertex
	for (int i = 0; i < vertexCount; i++) {
		SUPoint3D position;
		if (!checkSUResult(
			SUVertexGetPosition(vertices[i], &position),
			"SUVertexGetPosition",
			moment
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
		"retrieving layer when getting element layer name"
	)) return false;

	// Create string
	SUStringRef layerName = SU_INVALID;
	if (!checkSUResult(
		SUStringCreate(&layerName),
		"SUStringCreate",
		"retrieving layer when getting element layer name"
	)) return false;

	// get the value

	if (!checkSUResult(
		SULayerGetName(layer,&layerName),
		"SULayerGetName",
		"retreiving Layer's name"
	)) return false;

	// get final length
	size_t layerNameLength;
	if (!checkSUResult(
		SUStringGetUTF8Length(layerName,&layerNameLength),
		"SUStringGetUTF8Length",
		"retrieving layer name length when getting element layer name"
	)) return false;


	char cLayerName[MAX_STRING_LENGTH];
	if (!checkSUResult(
		SUStringGetUTF8(layerName, layerNameLength, cLayerName, &layerNameLength),
		"SUStringGetUTF8",
		"retrieving layer name length when getting element layer name"
	)) return false;

	// Release string back
	if (!checkSUResult(
		SUStringRelease(&layerName),
		"SUStringRelease",
		"retrieving layer when getting element layer name"
	)) return false;

	char asciiLayerName[MAX_STRING_LENGTH];
	utf8toASCII(cLayerName, layerNameLength, asciiLayerName, &layerNameLength);
	fixString(asciiLayerName, layerNameLength);

	*name = std::string(asciiLayerName);
	return true;
};

bool SKPReader::getSUEntityName(SUEntityRef entity, std::string * name) 
{
	// check how many dictionaries
	size_t dictionaryCount;
	if (!checkSUResult(
		SUEntityGetNumAttributeDictionaries(entity,&dictionaryCount),
		"SUEntityGetNumAttributeDictionaries",
		"Counting dictionaries of face"
	)) return false;
	
	// if there are dictionaries, then
	if (dictionaryCount > 0) {
		
		//retrieve dictionaries
		std::vector <SUAttributeDictionaryRef> dictionaries(dictionaryCount);
		if (!checkSUResult(
			SUEntityGetAttributeDictionaries(entity, dictionaryCount, &dictionaries[0], &dictionaryCount),
			"SUEntityGetAttributeDictionaries",
			"Retrieving dictionaries for getting entity name"
		)) return false;
		
		// Check if it has a Groundhog dictionary
		for (int i = 0; i < dictionaryCount; i++) {
			
			SUStringRef dictionaryName = SU_INVALID;
			if (!checkSUResult(
				SUStringCreate(&dictionaryName),
				"SUStringCreate",
				"initializing dictionary name"
			)) return false;
			
			if (!checkSUResult(
				SUAttributeDictionaryGetName(dictionaries[i],&dictionaryName),
				"SUAttributeDictionaryGetName",
				"Getting dictionary name on getting entitiy name"
			)) return false;
			
			int result;
			if(!checkSUResult(
				SUStringCompare(dictionaryName, groundhogDictionaryName, &result),
				"SUStringCompare",
				"Checking if dictionary matches " + std::string(SKP_GROUNDHOG_DICTIONARY)
			)) return false;

			if (!checkSUResult(
				SUStringRelease(&dictionaryName),
				"SUStringRelease",
				"releasing dictionary name"
			)) return false;

			if (result == 0) {
			
				//retrieve the value
				SUTypedValueRef value = SU_INVALID;
				if (!checkSUResult(
					SUTypedValueCreate(&value),
					"SUTypedValueCreate",
					"retrieving value from "+ std::string(SKP_GROUNDHOG_DICTIONARY)+" dictionary"
				)) return false;

				SUResult hasName = SUAttributeDictionaryGetValue(dictionaries[i], SKP_NAME, &value);
				if (hasName == SU_ERROR_NONE) {
					//does have its own name
					if (!checkSUResult(
						hasName,
						"SUAttributeDictionaryGetValue",
						"retrieving value from " + std::string(SKP_GROUNDHOG_DICTIONARY) + " dictionary"
					)) return false;


					SUStringRef suStringEntityName = SU_INVALID;
					if (!checkSUResult(
						SUStringCreate(&suStringEntityName),
						"SUStringCreate",
						"initializing initializing"
					)) return false;


					if (!checkSUResult(
						SUTypedValueGetString(value, &suStringEntityName),
						"SUTypedValueGetString",
						"retrieving string from SUString"
					)) {
						SUStringRelease(&suStringEntityName);
						return false;
					}


					size_t nameLength;
					if (!checkSUResult(
						SUStringGetUTF8Length(suStringEntityName, &nameLength),
						"SUStringGetUTF8Length",
						"SUStringGetUTF8Length"
					)) return false;


					char cStringEntityName[MAX_STRING_LENGTH];
					if (!checkSUResult(
						SUStringGetUTF8(suStringEntityName, nameLength, cStringEntityName, &nameLength),
						"SUStringGetUTF8",
						"SUStringGetUTF8"
					)) return false;


					if (!checkSUResult(
						SUStringRelease(&suStringEntityName),
						"SUStringRelease",
						"SUStringRelease"
					))return false;

					char asciiEntityName[MAX_STRING_LENGTH];
					utf8toASCII(cStringEntityName, nameLength, asciiEntityName, &nameLength);
					fixString(asciiEntityName, nameLength);

					*name = std::string(asciiEntityName);

					return true;

				} // enf of "hasName == SU_ERROR_NONE"		
			} // end of "if it is Groundhog"
		} //end of iterating through dictionaries				
	}
	else {
		// if not, check for a SketchUp Assigned name

		// else, set ID
		int32_t id;
		if (!checkSUResult(
			SUEntityGetID(entity, &id),
			"SUEntityGetID",
			"SUEntityGetID"
		)) return false;

		*name = std::to_string(id);
		return true;
	}
	
	return false;
} // end of Get Entity Name


bool SKPReader::bulkComponentInstancesIntoVector(std::vector <ComponentInstance * > * dest, SUEntitiesRef  entities, GroundhogModel * model) {
	// load component instances
	size_t instanceCount;
	if (!checkSUResult(
		SUEntitiesGetNumInstances(entities, &instanceCount),
		"SUEntitiesGetNumInstances",
		"Retrieving faces"
	)) return false;

	if (instanceCount == 0) {
		return true;
	}

	std::vector<SUComponentInstanceRef> instances(instanceCount);
	if (!checkSUResult(
		SUEntitiesGetInstances(entities, instanceCount, &instances[0], &instanceCount),
		"SUEntitiesGetInstances",
		"Retrieving component instances"
	)) return false;

	// fill layers with the instances
	for (size_t i = 0; i < instanceCount; i++) {
		// get name of layer
		SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instances[i]);
		std::string layerName;
		if (!getSUDrawingElementLayerName(drawingElement, &layerName))
			return false;
		
		addComponentInstanceToVector(dest, instances[i], model);

	}

	return true;
}

bool SKPReader::fillComponentInstanceLocation(ComponentInstance * instance, SUComponentInstanceRef suInstance) {
	
	SUTransformation transform;

	if (!checkSUResult(
		SUComponentInstanceGetTransform(suInstance, &transform),
		"SUEntitiesGetInstances",
		"Retrieving component instance Transform"
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
	// check how many dictionaries
	size_t dictionaryCount;
	if (!checkSUResult(
		SUEntityGetNumAttributeDictionaries(entity, &dictionaryCount),
		"SUEntityGetNumAttributeDictionaries",
		"Counting dictionaries of face"
	)) return false;

	// if there are no dictionaries, then return.
	if (dictionaryCount == 0)
		return false;

	//retrieve dictionaries
	std::vector <SUAttributeDictionaryRef> dictionaries(dictionaryCount);
	if (!checkSUResult(
		SUEntityGetAttributeDictionaries(entity, dictionaryCount, &dictionaries[0], &dictionaryCount),
		"SUEntityGetAttributeDictionaries",
		"Retrieving dictionaries for getting entity name"
	)) return false;

	// Check if it has a Groundhog dictionary
	for (int i = 0; i < dictionaryCount; i++) {

		SUStringRef dictionaryName = SU_INVALID;
		if (!checkSUResult(
			SUStringCreate(&dictionaryName),
			"SUStringCreate",
			"initializing dictionary name"
		)) return false;

		if (!checkSUResult(
			SUAttributeDictionaryGetName(dictionaries[i], &dictionaryName),
			"SUAttributeDictionaryGetName",
			"Getting dictionary name on getting entitiy name"
		)) return false;

		int result;
		if (!checkSUResult(
			SUStringCompare(dictionaryName, groundhogDictionaryName, &result),
			"SUStringCompare",
			"Checking if dictionary matches " + std::string(SKP_GROUNDHOG_DICTIONARY)
		)) return false;

		if (!checkSUResult(
			SUStringRelease(&dictionaryName),
			"SUStringRelease",
			"releasing dictionary name"
		)) return false;

		if (result == 0) { // then, it is a Groundhog dictionary

			//retrieve the value
			SUTypedValueRef value = SU_INVALID;
			if (!checkSUResult(
				SUTypedValueCreate(&value),
				"SUTypedValueCreate",
				"retrieving label from " + std::string(SKP_GROUNDHOG_DICTIONARY) + " dictionary"
			)) return false;

			SUResult hasValue = SUAttributeDictionaryGetValue(dictionaries[i], SKP_LABEL, &value);
			if (hasValue == SU_ERROR_NO_DATA) {
				return false; // No error... only, not any label
			}
			if (!checkSUResult(
				hasValue,
				"SUAttributeDictionaryGetValue",
				"retrieving label from " + std::string(SKP_GROUNDHOG_DICTIONARY) + " dictionary"
			)) return false;


			SUStringRef suStringEntityLabel= SU_INVALID;
			if (!checkSUResult(
				SUStringCreate(&suStringEntityLabel),
				"SUStringCreate",
				"initializing initializing"
			)) return false;


			if (!checkSUResult(
				SUTypedValueGetString(value, &suStringEntityLabel),
				"SUTypedValueGetString",
				"retrieving string from SUString"
			)) {
				SUStringRelease(&suStringEntityLabel);
				return false;
			}


			size_t nameLength;
			if (!checkSUResult(
				SUStringGetUTF8Length(suStringEntityLabel, &nameLength),
				"SUStringGetUTF8Length",
				"SUStringGetUTF8Length"
			)) return false;


			char cStringEntityName[MAX_STRING_LENGTH];
			if (!checkSUResult(
				SUStringGetUTF8(suStringEntityLabel, nameLength, cStringEntityName, &nameLength),
				"SUStringGetUTF8",
				"SUStringGetUTF8"
			)) return false;


			if (!checkSUResult(
				SUStringRelease(&suStringEntityLabel),
				"SUStringRelease",
				"SUStringRelease"
			)) return false;

			char asciiEntityName[MAX_STRING_LENGTH];
			utf8toASCII(cStringEntityName, nameLength, asciiEntityName, &nameLength);
			fixString(asciiEntityName, nameLength);

			*name = std::string(asciiEntityName);

			return true;
		} // end of "if it is Groundhog"
	} //end of iterating through dictionaries		

	return false; // return false if there was no Groundhog dictionary
	
}

bool SKPReader::addWorkplaneToModel(SUFaceRef suFace, GroundhogModel * model) {

	// get the name of the face
	std::string name;
	bool hasName = getSUFaceName(suFace, &name);
	if (!hasName) {
		fatal("Invalid workplane: has no name");
		return false;
	}

	// Build the polygon
	Polygon3D * polygon = new Polygon3D();
	if (!SUFaceToPolygon3D(suFace, polygon))
		return false;

	model->addPolygonToWorkplane(&name, polygon);
	return true;
}
