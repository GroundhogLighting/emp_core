#include "../../common/utilities/io.h"
#include "../../common/utilities/file.h"
#include "../../groundhogmodel/groundhogmodel.h"
#include "./radexporter.h"

#include <fstream>

#define TAB "\t"

RadExporter::RadExporter() {
	DEBUG_MSG("Creating Rad Exporter");

}

RadExporter::~RadExporter() {

	DEBUG_MSG("Destroying Rad Exporter");
}


void RadExporter::setExportDir(std::string outDir) {
	exportDir = outDir;
}

bool RadExporter::exportModel(GroundhogModel * model, bool verbose) {
	inform("Beggining Radiance export", verbose);

	// Check if directory exists
	if ((dexist(exportDir) && isDir(exportDir))) {
		fatal("Export directory '" + exportDir + "' alredy exists... please delete it.");
		return false;
	}

	// Create the directory
	createdir(exportDir);

	// Write layers
	writeLayers(model,exportDir);


	return true;
}

bool RadExporter::writeLayers(GroundhogModel * model, std::string exportDir) {
	// create geometry directory
	std::string baseDir = exportDir + "/Geometry";
	createdir(baseDir);
	size_t numLayers = model->getNumLayers();
	for (unsigned int i = 0; i < numLayers; i++) {
		// get the layer
		Layer * layer = model->getLayerRef(i);		
		std::string layerName = layer->getName();

		// check if there are faces... continue if not.
		std::vector < Face * > * faces = layer->getFacesRef();
		size_t numFaces = faces->size();
		if (numFaces < 1) {
			warn("Empty layer '" + layerName + "'");
			continue;
		}

		// create the file
		std::ofstream file;
		file.open(baseDir + "/" + layerName + ".rad");

		// write all faces
		for (unsigned int j = 0; j < numFaces; j++) {
			writeFace(&file, layer->getFaceRef(j));
		}

		// Close the file
		file.close();
	}
	return true;
}

void RadExporter::writeLoop(std::ofstream * file, Loop * loop) {
	// Print the loop
	size_t numVertices = loop->getNumVertices();

	for (int i = 0; i < numVertices; i++) {
		Point3D * point = loop->getVertex(i);

		*file << TAB;
		*file << point->getX() << TAB;
		*file << point->getY() << TAB;
		*file << point->getZ() << std::endl;
	}
}

void RadExporter::writeClosedFace(std::ofstream * file, Face * face) {
	// get the name of the face
	std::string faceName = face->getName();

	// define the loop that will be written
	Loop * finalLoop = NULL;
	bool needToDelete = false;
	if (face->hasInnerLoops()) {
		finalLoop = face -> getClosedLoop();		
		needToDelete = true;
	}
	else {
		finalLoop = face->getOuterLoopRef();
	}
	
	*file << "Default_material" << TAB << "polygon" << TAB << faceName << std::endl;
	*file << "0" << std::endl;
	*file << "0" << std::endl;
	*file << std::to_string(3 * finalLoop->getNumVertices()) << std::endl;
	writeLoop(file, finalLoop);

	*file << std::endl;
	if (needToDelete) {
		delete finalLoop;
	}
}

void RadExporter::writeFace(std::ofstream * file, Face * face) {
	if (face->hasTooManyInnerLoops()) {
		warn("Ignoring face '" + face->getName() + "' because it has TOO MANY inner loops.");
		// writeTriangulatedFace(file,face);
		return;
	}
	else {
		writeClosedFace(file,face);		
	}
	
}