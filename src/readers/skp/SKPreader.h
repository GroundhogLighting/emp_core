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
	bool loadFaces(GroundhogModel * model, bool verbose);
	bool getSUFaceName(SUFaceRef face, std::string * name);
	bool getSUEntityName(SUEntityRef entity, std::string * name);
	bool getSUFaceLayerName(SUFaceRef face, std::string * name);
	bool getSUDrawingElementLayerName(SUDrawingElementRef element, std::string * name);
	bool SUFaceToPolygon3D(SUFaceRef face, Polygon3D * polygon);
	bool SULoopToLoop(SULoopRef suLoop, Loop * loop);

	
};