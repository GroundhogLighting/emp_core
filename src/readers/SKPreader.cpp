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

#ifndef AVOID_SKP

#include "./SKPreader.h"
#include "../config_constants.h"
#include "../common/utilities/io.h"
#include "../common/utilities/stringutils.h"
#include "../groundhogmodel/groundhogmodel.h"
#include "../groundhogmodel/src/otype.h"
#include "../common/geometry/polygon.h"
#include "../groundhogmodel/src/photosensor.h"
#include "../groundhogmodel/src/componentinstance.h"


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
#include <SketchUpAPI/model/model.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define ASSERT_SU_RESULT(x) if(!checkSUResult(x,"x",__LINE__)){ throw "Error when calling SKP API"; }

SKPreader::SKPreader(GroundhogModel * ghmodel, bool newVerbose)
{
	
	//initialize API
	SUInitialize();

	suModel = SU_INVALID;

	groundhogDictionaryName = SU_INVALID;

    ASSERT_SU_RESULT(SUStringCreateFromUTF8(&groundhogDictionaryName, SKP_GROUNDHOG_DICTIONARY));

	model = ghmodel;
	verbose = newVerbose;
};

SKPreader::~SKPreader()
{
	
	
	//release dictionary
    ASSERT_SU_RESULT(SUStringRelease(&groundhogDictionaryName));
	
	// Must release the model or there will be memory leaks
	SUModelRelease(&suModel);
	
	// Always terminate the API when done using it
	SUTerminate();
};


bool SKPreader::checkSUResult(SUResult res, std::string functionName, int ln) const
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

    std::string errorMessage = "function '" + functionName + "' returned '" + error + "  -- FILE: " + __FILE__ + " -- LINE: " + std::to_string(ln) ;
    std::cout << errorMessage << std::endl;
	return false;
}


bool SKPreader::parseSKPModel(std::string inputFile)
{
	//Load model
	ASSERT_SU_RESULT(SUModelCreateFromFile(&suModel, inputFile.c_str()));    
    
	// Load layers	
    loadLayers();
    
	// load components and groups
    loadComponentDefinitions();
		
    loadGroupDefinitions();

    fillComponentDefinitions();

    fillGroupDefinitions();

	// Fill layers and load related surfaces, discrimitating between
	// workplanes, windows, etc.
    loadLayersContent();

	// Load views
	loadViews();

	// Load model info (location, date, etc).
    loadModelInfo();
	
	// Load weather
    loadWeather();
    
    // Load the workplanes preferences
    loadWorkplanesPreferences();

    // Load tasks
    loadTasks();
    
	
	return true;
};


bool SKPreader::getStringFromShadowInfo(SUShadowInfoRef shadowInfo, const char * key, std::string * value) const
{
	SUTypedValueRef suValue = SU_INVALID;
    
    // Create value
    ASSERT_SU_RESULT(SUTypedValueCreate(&suValue));

    // Get value
    ASSERT_SU_RESULT(SUShadowInfoGetValue(shadowInfo, key, &suValue));

	SUStringRef suString = SU_INVALID;

    ASSERT_SU_RESULT(SUStringCreate(&suString));

    ASSERT_SU_RESULT(SUTypedValueGetString(suValue, &suString));
	
    SUStringtoString(suString, value);
	
	return true;
}

bool SKPreader::getDoubleFromShadowInfo(SUShadowInfoRef shadowInfo, const char * key, double * value) const
{
	SUTypedValueRef suValue= SU_INVALID;
    
    ASSERT_SU_RESULT(SUTypedValueCreate(&suValue));
    ASSERT_SU_RESULT(SUShadowInfoGetValue(shadowInfo, key, &suValue));
    ASSERT_SU_RESULT(SUTypedValueGetDouble(suValue, value));
    ASSERT_SU_RESULT(SUTypedValueRelease(&suValue));

	return true;
}


bool SKPreader::getTimeFromShadowInfo(SUShadowInfoRef shadowInfo, int64_t * value) const
{
	SUTypedValueRef suValue = SU_INVALID;
	
    ASSERT_SU_RESULT(SUTypedValueCreate(&suValue));
    ASSERT_SU_RESULT(SUShadowInfoGetValue(shadowInfo, "ShadowTime", &suValue));
    ASSERT_SU_RESULT(SUTypedValueGetTime(suValue, value));
    ASSERT_SU_RESULT(SUTypedValueRelease(&suValue));

	return true;
}

bool SKPreader::loadModelInfo() const
{
	// load north correction
	double northC;
    ASSERT_SU_RESULT(SUModelGetNorthCorrection(suModel, &northC));
	model->setNorthCorrection(northC);

	// load date, longitude and latitude.
	SUShadowInfoRef shadowInfo;
    ASSERT_SU_RESULT(SUModelGetShadowInfo(suModel, &shadowInfo));

	// Get the location
	Location * loc = model->getLocation();

	// Set latitude
	double latitude;
	getDoubleFromShadowInfo(shadowInfo, "Latitude", &latitude);
	loc->setLatitude(latitude);

	// Set longitude
	double longitude;
	getDoubleFromShadowInfo(shadowInfo, "Longitude", &longitude);
	loc->setLongitude(-longitude); // Negative due to conventions

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
    
    // Albedo
    SUTypedValueRef albedo = SU_INVALID;
    if (!getValueFromModelGHDictionary(SKP_ALBEDO, &albedo))
        return true; // return if no albedo or error.
    
    double value;
    ASSERT_SU_RESULT(SUTypedValueGetDouble(albedo, &value));
    loc->setAlbedo(value);

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

bool SKPreader::SUCameraToView(std::string * viewName, SUCameraRef camera, View * view) const
{

	// set the name
	view->setName(viewName);

	// get view point and up.
	SUPoint3D position;
	SUPoint3D target; //this is useless
	SUVector3D up;
    ASSERT_SU_RESULT(SUCameraGetOrientation(camera, &position, &target, &up));
    
	view->setViewPoint(new Point3D(TO_M(position.x), TO_M(position.y), TO_M(position.z)));
	view->setViewUp(Vector3D(up.x, up.y, up.z));

	// get and set the view direction
	SUVector3D direction;
    ASSERT_SU_RESULT(SUCameraGetDirection(camera, &direction));
    
	view->setViewDirection(Vector3D(direction.x, direction.y, direction.z));

	// get and set type
	bool perspective;
    ASSERT_SU_RESULT(SUCameraGetPerspective(camera, &perspective));

	int type = perspective ? PERSPECTIVE_VIEW : PARALLEL_VIEW;
	view->setViewType(type);

	double viewHeight;
	if (type == PERSPECTIVE_VIEW) {
        ASSERT_SU_RESULT(SUCameraGetPerspectiveFrustumFOV(camera, &viewHeight));
	}
	else { // PARALLEL
        ASSERT_SU_RESULT(SUCameraGetOrthographicFrustumHeight(camera, &viewHeight));
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

bool SKPreader::SUViewToView(SUSceneRef suView, View * view) const
{
	// get the name of the view
	
	SUStringRef suViewName = SU_INVALID;
    ASSERT_SU_RESULT(SUStringCreate(&suViewName));
	
    ASSERT_SU_RESULT(SUSceneGetName(suView, &suViewName));
	
	std::string viewName;

    SUStringtoString(suViewName,&viewName);

	// Get the camera
	SUCameraRef camera = SU_INVALID;
    ASSERT_SU_RESULT(SUSceneGetCamera(suView,&camera));

    SUCameraToView(&viewName, camera, view);
		
	return true;
}

bool SKPreader::loadViews() const
{
	
	// get the current view
	SUCameraRef activeCamera;

    ASSERT_SU_RESULT(SUModelGetCamera(suModel, &activeCamera));

	View * activeView = new View();
    std::string viewName = "view";
	SUCameraToView(&viewName, activeCamera, activeView);
	model->addView(activeView);

	// load stored views
	size_t countScenes;

    ASSERT_SU_RESULT(SUModelGetNumScenes(suModel, &countScenes));
	
	if (countScenes == 0) {
		return true;
	}
	std::vector<SUSceneRef> views(countScenes);

    ASSERT_SU_RESULT(SUModelGetScenes(suModel, countScenes, &views[0],&countScenes));

	for (size_t i = 0; i < countScenes; i++) {
		View * storedView = new View();
		SUViewToView(views[i], storedView);
		model->addView(storedView);
	}

	return true;
}

bool SKPreader::loadLayers() const
{

	// count layers
	size_t countLayers = 0;
    ASSERT_SU_RESULT(SUModelGetNumLayers(suModel, &countLayers));

	//get those layers
	std::vector<SULayerRef> layers(countLayers);
    ASSERT_SU_RESULT(SUModelGetLayers(suModel, countLayers, &layers[0], &countLayers));

	// inform layer status
	INFORM(auxString,"Counted layers: " + size_tToString(countLayers), verbose);

	// create and load the layers
	for (size_t i = 0; i < layers.size(); i++) {
		SUStringRef suLayerName = SU_INVALID;		
        ASSERT_SU_RESULT(SUStringCreate(&suLayerName));

        ASSERT_SU_RESULT(SULayerGetName(layers[i],&suLayerName));

		std::string layerName;

        SUStringtoString(suLayerName,&layerName);
		
		model->addLayer(&layerName);
		INFORM(informMessage,"Layer " + layerName + " added",verbose);
	};	

	return true;
} // end of Load Layers

bool SKPreader::getSUComponentDefinitionName(SUComponentDefinitionRef definition, std::string * name) const
{
	//define a SUString
	SUStringRef suComponentName = SU_INVALID;

    ASSERT_SU_RESULT(SUStringCreate(&suComponentName));

    ASSERT_SU_RESULT(SUComponentDefinitionGetName(definition, &suComponentName));

	std::string componentName;
    SUStringtoString(suComponentName,&componentName);

	*name = componentName;
	return true;
}


bool SKPreader::addComponentInstanceToVector(std::vector <ComponentInstance * > * dest, SUComponentInstanceRef suComponentInstance) const
{

	// get definition
	SUComponentDefinitionRef definition;
    ASSERT_SU_RESULT(SUComponentInstanceGetDefinition(suComponentInstance, &definition));

	std::string definitionName;
	getSUComponentDefinitionName(definition, &definitionName);

	// get the definition
	ComponentDefinition * definitionRef = model->getComponentDefinitionByName(&definitionName);
	if (definitionRef == nullptr)
		FATAL(errorMessage,"Impossible to find " + definitionName + " when adding an instance... ignoring instance");
		

	//create the instance
	ComponentInstance * instance = new ComponentInstance(definitionRef);

	//Fill the position
    fillComponentInstanceLocation(instance, suComponentInstance);

	//add and return
	dest->push_back(instance);
	return true;
}


bool SKPreader::bulkFacesIntoVector(std::vector <Otype * > * const dest, SUEntitiesRef entities) const
{

	// count faces in these entities
	size_t numFaces = 0;
    ASSERT_SU_RESULT(SUEntitiesGetNumFaces(entities, &numFaces));

	if (numFaces == 0)
		return true;

	// get the faces
	std::vector<SUFaceRef> faces(numFaces);
    ASSERT_SU_RESULT(SUEntitiesGetFaces(entities, numFaces, &faces[0], &numFaces));

	// import faces
	for (size_t i = 0; i < numFaces; i++) {	
		Face * f = SUFaceToFace(faces[i]);

		if (f == nullptr)
            throw "Trying to process a NULL face";

		dest->push_back(f);
	}
	return true;
}


bool SKPreader::loadComponentDefinition(SUComponentDefinitionRef definition) const
{
    // Check if it has label
    int label = getSUEntityLabel(SUComponentDefinitionToEntity(definition));
  

    if (label == SKP_SOLVED_WORKPLANE)
        return true;

    if (label == SKP_PHOTOSENSOR) {
        addPhotosensorsToModel(definition);
    }


    //get the name
    std::string definitionName;
    getSUComponentDefinitionName(definition, &definitionName);

    
    // add a component definition to the model
    model->addComponentDefinition(&definitionName);

    return true;
}

bool SKPreader::loadComponentDefinitions() const
{
    // count the component definitions
    size_t countDefinitions = 0;
    ASSERT_SU_RESULT(SUModelGetNumComponentDefinitions(suModel, &countDefinitions));

    // return if none
    if (countDefinitions == 0) {
        inform("No component definitions in model", verbose);
        return true; // success
    }
    INFORM(i, std::to_string(countDefinitions) + " component definitions in model", verbose);

    // get the component definitions
    std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
    ASSERT_SU_RESULT(SUModelGetComponentDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions));

    // Now, load One by One
    for (size_t i = 0; i < countDefinitions; i++) {
        loadComponentDefinition(definitions[i]);
    }
    return true;
}



bool SKPreader::fillComponentDefinitions() const
{
	// count the component definitions
	size_t countDefinitions = 0;
    ASSERT_SU_RESULT(SUModelGetNumComponentDefinitions(suModel, &countDefinitions));

	// return if none
	if (countDefinitions == 0) {
		inform("No component definitions in model",verbose);
		return true; // success
	}

	// get the component definitions
	std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
    ASSERT_SU_RESULT(SUModelGetComponentDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions));

	// Now, load One by One
	for (size_t i = 0; i < countDefinitions; i++) {
      
        //get the name
        std::string definitionName;
        getSUComponentDefinitionName(definitions[i], &definitionName);

        ComponentDefinition * ghDefinition = model->getComponentDefinitionByName(&definitionName);

        // get entities
        SUEntitiesRef entities;
        ASSERT_SU_RESULT(SUComponentDefinitionGetEntities(definitions[i], &entities));


      // Load faces
      bulkFacesIntoVector(ghDefinition->getModifiableObjectsRef(), entities);

      // load instances
      bulkComponentInstancesIntoVector(ghDefinition->getModifiableComponentInstancesRef(), entities);
      
	}
	return true;
  
}



bool SKPreader::fillGroupDefinitions() const
{
    // count the component definitions
    size_t countDefinitions = 0;
    ASSERT_SU_RESULT(SUModelGetNumGroupDefinitions(suModel, &countDefinitions));

    // return if none
    if (countDefinitions == 0) {
        inform("No component definitions in model", verbose);
        return true; // success
    }

    // get the component definitions
    std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
    ASSERT_SU_RESULT(SUModelGetGroupDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions));

    // Now, load One by One
    for (size_t i = 0; i < countDefinitions; i++) {

        std::string definitionName;
        getSUComponentDefinitionName(definitions[i], &definitionName);

        ComponentDefinition * ghDefinition = model->getComponentDefinitionByName(&definitionName);

        // If the definition does not exist, it means that it was skipped
        // (i.e. it was labelled as solve_workplane)
        if(ghDefinition == nullptr)
            continue;
        
        // get entities
        SUEntitiesRef entities;
        ASSERT_SU_RESULT(SUComponentDefinitionGetEntities(definitions[i], &entities));
        
        
        // Load faces
        bulkFacesIntoVector(ghDefinition->getModifiableObjectsRef(), entities);

        // load instances
        bulkComponentInstancesIntoVector(ghDefinition->getModifiableComponentInstancesRef(), entities);

    }
    return true;

}

bool SKPreader::loadInstance(SUComponentInstanceRef instance) const
{
  
  
    // ignore instances with certain labels
    int label = getSUEntityLabel(SUComponentInstanceToEntity(instance));
  
    if (label == SKP_PHOTOSENSOR)
        return true;

    if (label == SKP_SOLVED_WORKPLANE)
        return true;
  

    // get name of layer
    SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instance);
    std::string layerName;
    getSUDrawingElementLayerName(drawingElement, &layerName);
    
    Layer * layerRef = model->getLayerByName(&layerName);
    if (layerRef == nullptr)
        throw "Trying to put an instance in inexistent layer when loading SKP file";
    

    addComponentInstanceToVector(layerRef->getModifiableComponentInstancesRef(), instance);

    return true;
}


bool SKPreader::loadGroupDefinitions() const
{
  
    // count the component definitions
    size_t countDefinitions = 0;
    ASSERT_SU_RESULT(SUModelGetNumGroupDefinitions(suModel, &countDefinitions));

      // return if none
      if (countDefinitions == 0) {
          inform("No Group definitions in model", verbose);
          return true; // success
      }
      INFORM(i, std::to_string(countDefinitions) + " group definitions in model", verbose);

    // get the component definitions
    std::vector<SUComponentDefinitionRef> definitions(countDefinitions);
    ASSERT_SU_RESULT(SUModelGetGroupDefinitions(suModel, countDefinitions, &definitions[0], &countDefinitions));

    // Now, load One by One
    for (size_t i = 0; i < countDefinitions; i++) {
        loadComponentDefinition(definitions[i]);
    }
  
    return true;
}

bool SKPreader::loadLayersContent() const
{
	// Get the entity container of the model.
	SUEntitiesRef entities = SU_INVALID;
    ASSERT_SU_RESULT(SUModelGetEntities(suModel, &entities));


	// count and load faces
	size_t faceCount = 0;
    ASSERT_SU_RESULT(SUEntitiesGetNumFaces(entities, &faceCount));

	if (faceCount > 0) {
		INFORM(i,"Counted Faces: " + size_tToString(faceCount), verbose);

		std::vector<SUFaceRef> faces(faceCount);
        ASSERT_SU_RESULT(SUEntitiesGetFaces(entities, faceCount, &faces[0], &faceCount));
	
		for (size_t i = 0; i < faceCount; i++) {

			// CHECK LABEL OF FACE
			int faceLabel = getSUFaceLabel(faces[i]);
		
			if ( faceLabel >= 0 ) {
				if (faceLabel == SKP_WORKPLANE) {
                    // if it is workplane
					addWorkplaneToModel(faces[i]);
				}
				else if (faceLabel == SKP_ILLUM) {
                    // if it is illum
                    addIllumToModel(faces[i]);
				}
				else if (faceLabel == SKP_WINDOW) {
                    // if it is window
					addWindowToModel(faces[i]);
				}
				continue;
			}
		
			// if has no label (i.e. is geometry face)		
			std::string layerName;		
            getSUFaceLayerName(faces[i],&layerName);

			Layer * layerRef = model->getLayerByName(&layerName);
			if (layerRef == nullptr)
                throw "Reference to an inexistent layer when parsing SKP file";
			
		
			Face * face = SUFaceToFace(faces[i]);
			if (face == nullptr)
				throw "Reference to an inexistent face when parsing SKP file";

			// add the face
			layerRef->getModifiableObjectsRef()->push_back(face);

		} // end of iterating faces
	            
	}
	else {
		// Do not return... there may be groups or component instances
		warn("No faces in model");		
	}


	// load component instances
	size_t instanceCount = 0;
    ASSERT_SU_RESULT(SUEntitiesGetNumInstances(entities, &instanceCount));

    if (instanceCount > 0) {
	  std::vector<SUComponentInstanceRef> instances(instanceCount);
        ASSERT_SU_RESULT(SUEntitiesGetInstances(entities, instanceCount, &instances[0], &instanceCount));

	  // fill layers with the instances
	  for (size_t i = 0; i < instanceCount; i++) {		
        loadInstance(instances[i]);
	  }      

    } // end loading component instances.

    // Load group instances   
    size_t groupCount = 0;
    ASSERT_SU_RESULT(SUEntitiesGetNumGroups(entities, &groupCount));

    if (groupCount > 0) {
        // Declare the vector
        std::vector<SUGroupRef> groups(groupCount);
      
        // Fill vector
        ASSERT_SU_RESULT(SUEntitiesGetGroups(entities, groupCount, &groups[0], &groupCount));

        // fill layers with the instances
        for (size_t i = 0; i < groupCount; i++) {
            SUComponentInstanceRef instance = SUGroupToComponentInstance(groups[i]);
            loadInstance(instance);
        }

    } // end loading groups

	return true;
} // end of Load Faces

bool SKPreader::SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon) const
{

	// get area
	double area;
    ASSERT_SU_RESULT(SUFaceGetArea(face,&area));
    
	polygon->setArea(TO_M2(area));
	
	// Get the normal
	SUVector3D normal;
    ASSERT_SU_RESULT(SUFaceGetNormal(face, &normal));
    
	polygon->setNormal(Vector3D(normal.x, normal.y, normal.z));

	// get the outer loop
	SULoopRef suOuterLoop = SU_INVALID;
    ASSERT_SU_RESULT(SUFaceGetOuterLoop(face,&suOuterLoop));

	// translate outer loop
    SULoopToLoop(suOuterLoop, polygon->getOuterLoopRef());

	// get number of inner loops
	size_t countInnerLoops;
	ASSERT_SU_RESULT(SUFaceGetNumInnerLoops(face, &countInnerLoops));

	// Get and translate those loops, if at least one
	if (countInnerLoops > 0) {
		// get them
		std::vector<SULoopRef> innerLoops(countInnerLoops);

		//ASSERT_SU_RESULT(SUFaceGetNumInnerLoops(face, &countInnerLoops));
        ASSERT_SU_RESULT(SUFaceGetInnerLoops(face, countInnerLoops, &innerLoops[0], &countInnerLoops));
        
		// iterate them
		for (int j = 0; j < countInnerLoops; j++)
            SULoopToLoop(innerLoops[j], polygon->addInnerLoop());

	} // end of if there is an inner loop

	// clean the polygon
	polygon->clean();
	
	return true;
}

bool SKPreader::SULoopToLoop(SULoopRef suLoop, Loop * loop) const
{
    // Not sure why this happens!
    if(suLoop.ptr == nullptr)
        return true;
    
	// First, count vertices
	size_t vertexCount;
	ASSERT_SU_RESULT(SULoopGetNumVertices(suLoop,&vertexCount));

	
	// Second, retrieve vertices
	std::vector < SUVertexRef > vertices(vertexCount);	
	
	ASSERT_SU_RESULT(SULoopGetVertices(suLoop, vertexCount, &vertices[0], &vertexCount));


	// Third, translate each vertex
	for (int i = 0; i < vertexCount; i++) {
		SUPoint3D position;
		ASSERT_SU_RESULT(SUVertexGetPosition(vertices[i], &position));

		loop->addVertex(new Point3D(TO_M(position.x),TO_M(position.y),TO_M(position.z)));		
	}

	return true;
}

bool SKPreader::getSUFaceName(SUFaceRef face, std::string * name) const
{
	return getSUEntityName(SUFaceToEntity(face), name);
}

bool SKPreader::getSUFaceLayerName(SUFaceRef face, std::string * name) const
{	
	return getSUDrawingElementLayerName( SUFaceToDrawingElement(face), name);
}

bool SKPreader::getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name) const
{
	SULayerRef layer = SU_INVALID;
	ASSERT_SU_RESULT(SUDrawingElementGetLayer(element,&layer));

	// Create string
	SUStringRef layerName = SU_INVALID;
	ASSERT_SU_RESULT(SUStringCreate(&layerName));

	// retrieve the value
	ASSERT_SU_RESULT(SULayerGetName(layer,&layerName));

	// get final length
	
	return SUStringtoString(layerName,name);
};

bool SKPreader::getSUEntityName(SUEntityRef entity, std::string * name) const
{
	SUTypedValueRef suValue = SU_INVALID;
	if (getValueFromEntityGHDictionary(entity, SKP_NAME, &suValue)) {
		// There was, indeed, a Groundhog name
        getFromSUTypedValue(suValue, name);
			
		return true;
	}

	// else, retrieve ID.
	int32_t id = getEntityID(entity);
	if (id < 0)
        throw "Impossible to get name from SKP entity... not even by ID";
    
	*name = std::to_string(id);
	return true;

} // end of Get Entity Name


bool SKPreader::bulkComponentInstancesIntoVector(std::vector <ComponentInstance * > * const dest, SUEntitiesRef  entities) const
{

  /* LOAD THE COMPONENT INSTANCES FIRST */	
	size_t instanceCount = 0;
	ASSERT_SU_RESULT(SUEntitiesGetNumInstances(entities, &instanceCount));

	if (instanceCount != 0) {		
	  std::vector<SUComponentInstanceRef> instances(instanceCount);
	  ASSERT_SU_RESULT(SUEntitiesGetInstances(entities, instanceCount, &instances[0], &instanceCount));

	  // fill layers with the instances
	  for (size_t i = 0; i < instanceCount; i++) {
		  // get name of layer
		  SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instances[i]);
		  std::string layerName;
          getSUDrawingElementLayerName(drawingElement, &layerName);
		
		  addComponentInstanceToVector(dest, instances[i]);

	  }
	}

    /* THEN LOAD THE GROUPS */
    size_t groupInstanceCount = 0;
    ASSERT_SU_RESULT(SUEntitiesGetNumGroups(entities, &groupInstanceCount));


    if (groupInstanceCount != 0) {
      
      std::vector<SUGroupRef> groups(groupInstanceCount);
      ASSERT_SU_RESULT(SUEntitiesGetGroups(entities, groupInstanceCount, &groups[0], &groupInstanceCount));

      // fill layers with the instances
      for (size_t i = 0; i < groupInstanceCount; i++) {
          // Get drawing element
          SUGroupRef group = groups[i];
          SUComponentInstanceRef instance = SUGroupToComponentInstance(group);
          SUDrawingElementRef drawingElement = SUComponentInstanceToDrawingElement(instance);
      
          // get name of layer
          std::string layerName;
          getSUDrawingElementLayerName(drawingElement, &layerName);
          

          addComponentInstanceToVector(dest, instance);

        }
    }

	return true;
}

bool SKPreader::fillComponentInstanceLocation(ComponentInstance * instance, SUComponentInstanceRef suInstance) const
{
	
	SUTransformation transform;

	ASSERT_SU_RESULT(SUComponentInstanceGetTransform(suInstance, &transform));

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


int SKPreader::getSUFaceLabel(SUFaceRef face) const
{
	return getSUEntityLabel(SUFaceToEntity(face));
}


int SKPreader::getSUEntityLabel(SUEntityRef entity) const
{
	SUTypedValueRef suValue = SU_INVALID;
	if (!getValueFromEntityGHDictionary(entity, SKP_LABEL, &suValue))
		return -1;

    int label;
    if(SUTypedValueGetInt32(suValue, &label) != SU_ERROR_NO_DATA){
        return label;
    }else{
        return -1;
    }
}

bool SKPreader::addWorkplaneToModel(SUFaceRef suFace) const
{

	// get the name of the face
	std::string name;
	bool hasName = getSUFaceName(suFace, &name);
	if (!hasName)
		FATAL(errorMessage,"Invalid workplane: has no name");

	// Build the polygon
	Polygon3D * polygon = new Polygon3D();
    SUFaceToPolygon3D(suFace, polygon);

	model->addPolygonToWorkplane(&name, polygon);
	return true;
}

bool SKPreader::addWindowToModel(SUFaceRef suFace) const
{
	// get the name of the face
	std::string name;
	if (!getSUFaceName(suFace, &name)) // this should allways put something in 'name'
		throw "Impossible to get name from Window";
    

	// Create the face
	Face * face = SUFaceToFace(suFace);
	if (face == nullptr)
        throw "Trying to add an inexistent face to window group when parsing SKP model";
		
	// Add the window
	model->addWindowToGroup(&name, face);
	return true;
}


bool SKPreader::addIllumToModel(SUFaceRef suFace) const
{
    // get the name of the face
    std::string name;
    if (!getSUFaceName(suFace, &name)) // this should allways put something in 'name'
        throw "Impossible to get name from Window";
    
    // Create the Polygon3D
    Polygon3D * polygon = new Polygon3D();
    SUFaceToPolygon3D(suFace, polygon);
    
    // Add the window
    model->addIllumToGroup(&name, polygon);
    return true;
}


int32_t SKPreader::getEntityID(SUEntityRef entity) const
{
	// if not, check for a SketchUp Assigned name		
	// else, set ID
	int32_t id;
    ASSERT_SU_RESULT(SUEntityGetID(entity, &id));

	return id;
}


bool SKPreader::getValueFromEntityGHDictionary(SUEntityRef entity, const char * key, SUTypedValueRef * value) const
{
	// check how many dictionaries
	size_t dictionaryCount;
	ASSERT_SU_RESULT(SUEntityGetNumAttributeDictionaries(entity, &dictionaryCount));

	// if there are no dictionaries, then return.
	if (dictionaryCount == 0)
        return false; // No dictionary

	//retrieve dictionaries
	std::vector <SUAttributeDictionaryRef> dictionaries(dictionaryCount);
	ASSERT_SU_RESULT(SUEntityGetAttributeDictionaries(entity, dictionaryCount, &dictionaries[0], &dictionaryCount));

	// Check if it has a Groundhog dictionary
	for (int i = 0; i < dictionaryCount; i++) {
		SUStringRef dictionaryName = SU_INVALID;
		ASSERT_SU_RESULT(SUStringCreate(&dictionaryName));

		ASSERT_SU_RESULT(SUAttributeDictionaryGetName(dictionaries[i], &dictionaryName));


		int result;
		ASSERT_SU_RESULT(SUStringCompare(dictionaryName, groundhogDictionaryName, &result));

		ASSERT_SU_RESULT(SUStringRelease(&dictionaryName));

		if (result == 0) { // then, it is a Groundhog dictionary

			//retrieve the value			
			ASSERT_SU_RESULT(SUTypedValueCreate(value));


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
				throw "Error when getting value from Groundhog dictionary while parsing SKP file";
			}			
		}
	}
	return false; //should not reach here.
}

void SKPreader::loadTasks() const
{
    SUTypedValueRef tasksJSON = SU_INVALID;
    if (!getValueFromModelGHDictionary(SKP_TASKS, &tasksJSON))
        return ; // return if no tasks or error.
    
    std::string value;
    
    getFromSUTypedValue(tasksJSON, &value);
    json j = json::parse(value);
    
    // Check it is an array
    if(! j.is_array())
        FATAL(err, "Incorrect format in Tasks JSON in SketchUp Model");
    
    // Iterate array
    for (json task : j.get<json>()) {
        model->addTask(task);
    }
}


bool SKPreader::SUStringtoString(SUStringRef suString, std::string * string) const
{

	size_t stringLength;
	ASSERT_SU_RESULT(SUStringGetUTF8Length(suString, &stringLength));

	std::string utf8String;
	utf8String.reserve(stringLength);

	ASSERT_SU_RESULT(SUStringGetUTF8(suString, stringLength, &utf8String[0], &stringLength));

	ASSERT_SU_RESULT(SUStringRelease(&suString));

	string->reserve(stringLength);
	string->resize(stringLength);

	utf8toASCII(&utf8String[0], stringLength, &(*string)[0], &stringLength);

	return true;

}

bool SKPreader::getFromSUTypedValue(SUTypedValueRef suValue, std::string * value) const
{
	// Create a SU String
	SUStringRef suString = SU_INVALID;
	ASSERT_SU_RESULT(SUStringCreate(&suString));

	// Retrieve the String
    ASSERT_SU_RESULT(SUTypedValueGetString(suValue, &suString));
	SUStringtoString(suString, value);
	
	return true;
}


Material * SKPreader::addMaterialToModel(SUMaterialRef material) const
{	
	
	// create the json
	json j;

	SUEntityRef entityMat = SUMaterialToEntity(material);

    // Check if the material exists
    std::string name;
    getSUMaterialName(material, &name);

    Material * m = model->hasMaterial(&name);
    
    if (m != NULL)
      return m;

	// Check if it is a Radiance Material
	int label = getSUEntityLabel(entityMat);
    
	if (label >= 0) {

		// it is a Radiance material
		if(label != SKP_MATERIAL)
            throw "Material with unknown label code " + std::to_string(label);
		
		// get the value
		std::string value;
		if (!getGHValueFromEntity(entityMat, &value, false)) {
            throw "Error when getting";
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

bool SKPreader::getGHValueFromEntity(SUEntityRef entity, std::string * value, bool fix) const
{
	SUTypedValueRef suValue = SU_INVALID;
    
    // Check if there is a dictionary
    if(!getValueFromEntityGHDictionary(entity, SKP_VALUE, &suValue))
        return false;
    
    getFromSUTypedValue(suValue, value);

	return true;
}

bool SKPreader::guessMaterial(SUMaterialRef material, json * j) const
{
	std::string name;

    getSUMaterialName(material,&name);

	WARN(message,"Guessing material " + name);
	
	// set the name
	(*j)["name"] = name;

	// set alpha
	double alpha;
	ASSERT_SU_RESULT(SUMaterialGetOpacity(material,&alpha));

	//(*j)["alpha"] = alpha;

	// set type
	(*j)["class"] = (alpha < 1 ? "glass" : "plastic");

	// set color
	SUColor color;
	ASSERT_SU_RESULT(SUMaterialGetColor(material, &color));

		
    (*j)["color"] = {
        {"r", alpha*color.red / 255.0},{"g", alpha*color.green / 255.0},{"b", alpha*color.blue / 255.0}
    };
	
    if(alpha == 1){
        (*j)["specularity"] = 0;
        (*j)["roughness"] = 0;
    }
    
	return true;
}

bool SKPreader::getFaceMaterial(SUFaceRef face, SUMaterialRef * mat) const
{
	// Check the front material
	SUMaterialRef frontMat;
	SUResult frontRes = SUFaceGetFrontMaterial(face, &frontMat);
	if (frontRes == SU_ERROR_NONE) { // it has front material
		// Check if it has physical information 
		int label = getSUEntityLabel(SUMaterialToEntity(frontMat));
		if (label >= 0) {
			// if it has, do not bother and return.
            if (label != SKP_MATERIAL)
                FATAL(e,"Weird material label code" + std::to_string(label));
            
			*mat = frontMat;
			return true; 
		}
	}

	// Check the back material
	SUMaterialRef backMat;
	SUResult backRes = SUFaceGetBackMaterial(face, &backMat);
	if (backRes == SU_ERROR_NONE) { // it has back material
		// Check if it has physical information 
        int label = getSUEntityLabel(SUMaterialToEntity(backMat));
		if (label >= 0) {
            if (label != SKP_MATERIAL)
                FATAL(e,"Weird material label code " + std::to_string(label));
            
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


Face * SKPreader::SUFaceToFace(SUFaceRef suFace) const
{

	// build the polygon
	Polygon3D * polygon = new Polygon3D();
    SUFaceToPolygon3D(suFace, polygon);

	// get the name of the face
	std::string name;

    getSUFaceName(suFace, &name); // this will allways put something		

	//build the face
	Face * face = new Face(&name);
	face->setPolygon(polygon);	

	// retrieve and set the material
	SUMaterialRef suMat;
	Material * mat;
    
    int faceLabel = getSUFaceLabel(suFace);
    
    if(faceLabel == SKP_WINDOW){
        // Add default glass to window
        mat = model->addDefaultGlass();
    } else if (getFaceMaterial(suFace, &suMat)) {
		// add default material to un-labeled surface.
		mat = addMaterialToModel(suMat);
	} else {
		// add default material
		mat = model->addDefaultMaterial();
	}

	face->setMaterial(mat);

	return face;
}

bool SKPreader::getSUMaterialName(SUMaterialRef material, std::string * name) const
{
	SUStringRef suName = SU_INVALID;

	ASSERT_SU_RESULT(SUStringCreate(&suName));

	ASSERT_SU_RESULT(SUMaterialGetName(material, &suName));
	
	// this fixes the name as well, and releases the SUString
    SUStringtoString(suName, name);

	return true;
}

bool SKPreader::addPhotosensorsToModel(SUComponentDefinitionRef definition) const
{
	// count instances
	size_t numInstances;
	ASSERT_SU_RESULT(SUComponentDefinitionGetNumInstances(definition, &numInstances));

	if (numInstances == 0)
		return true;

	std::vector <SUComponentInstanceRef> instances(numInstances);

	// Get the actual instances
    ASSERT_SU_RESULT(SUComponentDefinitionGetInstances(definition,numInstances,&instances[0],&numInstances));

	for (size_t i = 0; i < numInstances; i++) {

		//get the name
		std::string name;
        getSUEntityName(SUComponentInstanceToEntity(instances[i]), &name);

		// create the photosensor object
		Photosensor * ph = new Photosensor(name);

		// Get the transformation
		SUTransformation transform;

		ASSERT_SU_RESULT(SUComponentInstanceGetTransform(instances[i], &transform));

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


bool SKPreader::getValueFromModelGHDictionary(const char * key, SUTypedValueRef * value) const
{
	// check how many dictionaries
	size_t dictionaryCount;
	ASSERT_SU_RESULT(SUModelGetNumAttributeDictionaries(suModel, &dictionaryCount));

	// if there are no dictionaries, then return.
	if (dictionaryCount == 0)
        throw "Groundhog Dictionary not found when parsing SKP model";

	//retrieve dictionaries
	std::vector <SUAttributeDictionaryRef> dictionaries(dictionaryCount);
	ASSERT_SU_RESULT(SUModelGetAttributeDictionaries(suModel, dictionaryCount, &dictionaries[0], &dictionaryCount));

	// Check if it has a Groundhog dictionary
	for (int i = 0; i < dictionaryCount; i++) {
		SUStringRef dictionaryName = SU_INVALID;
		ASSERT_SU_RESULT(SUStringCreate(&dictionaryName));
		ASSERT_SU_RESULT(SUAttributeDictionaryGetName(dictionaries[i], &dictionaryName));


		int result;
		ASSERT_SU_RESULT(SUStringCompare(dictionaryName, groundhogDictionaryName, &result));

		ASSERT_SU_RESULT(SUStringRelease(&dictionaryName));

		if (result == 0) { // then, it is a Groundhog dictionary

			//retrieve the value			
			ASSERT_SU_RESULT(SUTypedValueCreate(value));


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
				throw "Error when getting value from GH Dictionary";
			}
		}
	}
	return false; //should not reach here.
}


bool SKPreader::loadWeather() const
{

	SUTypedValueRef suWeather = SU_INVALID;
	if (!getValueFromModelGHDictionary(SKP_WEATHER, &suWeather))
		return true; // return if no weather or error.

	std::string value;

    getFromSUTypedValue(suWeather, &value);
	
	json j = json::parse(value);

	Location * loc = model->getLocation();

	return loc->fillWeatherFromJSON(&j);
}

void SKPreader::loadWorkplanesPreferences() const
{
    SUTypedValueRef workplanesJSON = SU_INVALID;
    if (!getValueFromModelGHDictionary(SKP_WORKPLANES, &workplanesJSON))
        return ; // return if no value or error.

    std::string value;
    
    getFromSUTypedValue(workplanesJSON, &value);
    json j = json::parse(value);
    
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::string name = (*it)["name"].get<std::string>();
        double size = (*it)["pixel_size"].get<double>();
        json tasks = (*it)["tasks"].get<json>();
        if(!tasks.is_array())
            FATAL(e, "Tasks object assigned in a worplane should be an array");
        
        Workplane * wp = model->getWorkplaneByName(&name);
        wp->setMaxArea(size);
        
        // Iterate array
        for (json task : tasks.get<json>()) {            
            wp->addTask(task);
        }
        
    }
    
}

#endif // #ifndef AVOID_SKP
