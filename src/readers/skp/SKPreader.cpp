#include "./SKPreader.h"
#include "../../common/utilities/io.h"
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

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define GROUNDHOG_DICTIONARY "Groundhog"
#define WINDOW "Window"
#define WORKPLANE "Workplane"
#define ILLUM "Illum"

#define TO_M(x) x*0.0254
#define TO_M2(x) x*0.00064516

SKPReader::SKPReader() {
	DEBUG_MSG("Creating SKPReader");
	suModel = SU_INVALID;

	groundhogDictionary = SU_INVALID;
	checkSUResult(
		SUStringCreateFromUTF8(&groundhogDictionary, GROUNDHOG_DICTIONARY),
		"SUStringCreateFromUTF8",
		"initializing SKPReader"
	);
};

SKPReader::~SKPReader() {
	DEBUG_MSG("Destroying SKPReader");
	
	//release dictionary
	checkSUResult(
		SUStringRelease(&groundhogDictionary),
		"SUStringRelease",
		"destroying SKP reader"
	);
	
	// Must release the model or there will be memory leaks
	SUModelRelease(&suModel);
	
	// Always terminate the API when done using it
	SUTerminate();
};


bool SKPReader::checkSUResult(SUResult res, std::string functionName, std::string location) {
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


bool SKPReader::parseSKPModel(std::string inputFile, GroundhogModel * model, bool verbose){
	
	//initialize
	SUInitialize();

	//Load model
	if (!checkSUResult(
		SUModelCreateFromFile(&suModel, inputFile.c_str()),
		"SUModelCreateFromFile",
		"Loading model"
	)) return false;

	// Load layers	
	if(!loadLayers(model, verbose))
		return false;

	// Load faces, discrimitating between
	// workplanes, windows, etc.
	if (!loadFaces(model, verbose))
		return false;

	// Load views

	// Load date

	// Load ...


	
	return true;
};


bool SKPReader::loadLayers(GroundhogModel * model, bool verbose) {

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

		char cLayerName[100];		
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
		model->addLayer(&std::string(cLayerName));
		inform("Layer " + std::string(cLayerName) + " added",verbose);
	};

	return true;
} // end of Load Layers


bool SKPReader::loadFaces(GroundhogModel * model, bool verbose) {
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

		// if ignore
		//next

		// get the name of the face
		std::string name;
		getSUFaceName(faces[i],&name);

		// build the polygon
		Polygon3D * polygon = new Polygon3D();
		
		if (!SUFaceToPolygon3D(faces[i], polygon))
			return false;
		
		// if it is workplane

		// if it is illum


		// if it is window

		// if has no label (i.e. is geometry face)
		std::string layerName;		
		if (!getSUFaceLayerName(faces[i],&layerName))
			return false;

		Face * face = new Face(name);
		face->setPolygon(polygon);

		if(!model->addFaceToLayer(&layerName, face))
			return false;

	}
		
	return true;
} // end of Load Faces

bool SKPReader::SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon) {
	std::string moment = "transforming face to Polygon3D";
	// get area
	double area;
	if (!checkSUResult(
		SUFaceGetArea(face,&area),
		"SUFaceGetArea",
		moment
	)) return false;
	polygon->setArea(TO_M2(area));
	
	// get the outer loop
	SULoopRef suOuterLoop = SU_INVALID;
	if (!checkSUResult(
		SUFaceGetOuterLoop(face,&suOuterLoop),
		"SUFaceGetOuterLoop",
		moment
	)) return false;

	// translate outer loop
	if (!SULoopToLoop(suOuterLoop, polygon->getOuterLoopReference()))
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

	return true;
}

bool SKPReader::SULoopToLoop(SULoopRef suLoop, Loop * loop) {
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

bool SKPReader::getSUFaceName(SUFaceRef face, std::string * name) {
	return getSUEntityName(SUFaceToEntity(face), name);
}

bool SKPReader::getSUFaceLayerName(SUFaceRef face, std::string * name) {	
	return getSUDrawingElementLayerName( SUFaceToDrawingElement(face), name);
}

bool SKPReader::getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name) {
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


	char cLayerName[100];
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

	*name = std::string(cLayerName);
	return true;
};

bool SKPReader::getSUEntityName(SUEntityRef entity, std::string * name) {
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
				SUStringCompare(dictionaryName, groundhogDictionary, &result),
				"SUStringCompare",
				"Checking if dictionary matches " + std::string(GROUNDHOG_DICTIONARY)
			)) return false;

			if (!checkSUResult(
				SUStringRelease(&dictionaryName),
				"SUStringRelease",
				"releasing dictionary name"
			))return false;

			if (result == 0) {
			
				//retrieve the value
				SUTypedValueRef value = SU_INVALID;
				if (!checkSUResult(
					SUTypedValueCreate(&value),
					"SUTypedValueCreate",
					"retrieving value from "+ std::string(GROUNDHOG_DICTIONARY)+" dictionary"
				)) return false;

				
				if (!checkSUResult(
					SUAttributeDictionaryGetValue(dictionaries[i],"Name",&value),
					"SUAttributeDictionaryGetValue",
					"retrieving value from " + std::string(GROUNDHOG_DICTIONARY) + " dictionary"
				)) return false;

				
				SUStringRef suStringEntityName = SU_INVALID;
				if(!checkSUResult(
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
					SUStringGetUTF8Length(suStringEntityName,&nameLength),
					"SUStringGetUTF8Length",
					"SUStringGetUTF8Length"
				)) return false;

				
				char cStringEntityName[100];
				if (!checkSUResult(
					SUStringGetUTF8(suStringEntityName, nameLength,cStringEntityName,&nameLength),
					"SUStringGetUTF8",
					"SUStringGetUTF8"
				)) return false;

				
				if(!checkSUResult(
					SUStringRelease(&suStringEntityName),
					"SUStringRelease",
					"SUStringRelease"
				))return false;

				*name = std::string(cStringEntityName);

				return true;
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

