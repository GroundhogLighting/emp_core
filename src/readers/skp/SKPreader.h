#pragma once

#include "../../groundhogmodel/groundhogmodel.h"
#include "../../common/geometry/polygon.h"
#include "../../common/geometry/loop.h"

#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/attribute_dictionary.h>

#include <vector>


class SKPReader {
private:
	SUModelRef suModel;
	SUStringRef groundhogDictionary;

public:
	SKPReader();
	~SKPReader();
	bool checkSUResult(SUResult res, std::string functionName, std::string location);
	bool parseSKPModel(std::string, GroundhogModel * model, bool verbose);
	bool loadLayers(GroundhogModel * model, bool verbose);
	bool getSUComponentDefinitionName(SUComponentDefinitionRef definition, std::string * name);
	bool loadComponentDefinition(SUComponentDefinitionRef definition, GroundhogModel * model);
	bool loadComponentDefinitions(GroundhogModel * model, bool verbose);
	bool loadLayersContent(GroundhogModel * model, bool verbose);
	bool getSUFaceName(SUFaceRef face, std::string * name);
	bool getSUEntityName(SUEntityRef entity, std::string * name);
	bool getSUFaceLayerName(SUFaceRef face, std::string * name);
	bool getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name);
	bool SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon);
	bool SULoopToLoop(SULoopRef suLoop, Loop * loop);

	bool bulkFacesIntoVector(std::vector <Face * > * dest, SUEntitiesRef  entities);
	bool bulkComponentInstancesIntoVector(std::vector <ComponentInstance * > * dest, SUEntitiesRef  entities, GroundhogModel * model);

	bool addFaceToVector(std::vector <Face * > * dest, SUFaceRef face);


	bool addComponentInstanceToVector(std::vector <ComponentInstance * > * dest, SUComponentInstanceRef suComponentInstance, GroundhogModel * model);

	bool fillComponentInstanceLocation(ComponentInstance * instance, SUComponentInstanceRef suInstance);


	bool loadViews(GroundhogModel * model, bool verbose);

	bool SUViewToView(SUSceneRef suView, View * view);

	bool SUCameraToView(std::string viewName, SUCameraRef suCamera, View * view);
	bool loadModelInfo(GroundhogModel * model, bool verbose);
	bool getDoubleFromShadowInfo(SUShadowInfoRef shadowInfo, char * key, double * value);
	bool getStringFromShadowInfo(SUShadowInfoRef shadowInfo, char * key, char * value);
	bool getTimeFromShadowInfo(SUShadowInfoRef shadowInfo, int64_t * value);

};