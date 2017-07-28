#include "../../common/utilities/io.h"
#include "../../common/utilities/file.h"
#include "../../groundhogmodel/groundhogmodel.h"
#include "./radexporter.h"

#include <fstream>

#define TAB "\t"

RadExporter::RadExporter(GroundhogModel * the_model, std::string the_exportDir, bool the_verbose) 
{
	model = the_model;
	exportDir = the_exportDir;
	verbose = the_verbose;
	DEBUG_MSG("Creating Rad Exporter");

}

RadExporter::~RadExporter() 
{

	DEBUG_MSG("Destroying Rad Exporter");
}



bool RadExporter::exportModel() 
{
	inform("Beggining Radiance export", verbose);

	// Check if directory exists
	if ((dexist(exportDir) && isDir(exportDir))) {
		fatal("Export directory '" + exportDir + "' alredy exists... please delete it.");
		return false;
	}
	// Create the directory
	createdir(exportDir);

	// Write layers
	writeLayers();

	// Write component definitions
	writeComponentDefinitions();	

	// write views
	writeViews();

	// write north correction
	writeModelInfo();

	return true;
}

void RadExporter::writeModelInfo()
{
	// create and open file
	std::ofstream file;
	file.open(exportDir + "/model_info.txt");

	file << "country," << model->getCountry() << std::endl;
	file << "city," << model->getCity() << std::endl;
	file << "latitude," << model->getLatitude() << std::endl;
	file << "longitude," << model->getLongitude() << std::endl;
	file << "time zone," << model->getTimeZone() << std::endl;
	file << "month," << model->getMonth() << std::endl;
	file << "day," << model->getDay() << std::endl;
	file << "hour," << model->getHour() << std::endl;
	file << "minute," << model->getMinute() << std::endl;
	file << "north correction,"<< model->getNorthCorrection() << std::endl;
	file.close();

}

bool RadExporter::writeViews() 
{
	// create Views directory
	std::string baseDir = exportDir + "/Views";
	createdir(baseDir);

	// export each view
	for (size_t i = 0; i < model->getNumViews(); i++) {

		View * view = model->getViewRef(i);

		std::string vt="   ";
		switch (view->getViewType()) {
		case PARALLEL_VIEW:
			vt = "vtv";
			break;
		case PERSPECTIVE_VIEW:
			vt = "vtl";
			break;
		default:
			fatal("Unkown type of view coded " + view->getViewType());
			return false;
		}

		// create and open file
		std::ofstream file;
		file.open(baseDir + "/" + view->getName() + ".vf");

		// initialize
		file << "rvu ";

		// view type
		file << vt;

		// view point
		Point3D * vp = view->getViewPoint();
		file << " -vp " << vp->getX() << " " << vp->getY() << " " << vp->getZ();

		// view direction
		Vector3D * vd = view->getViewDirection();
		file << " -vd " << vd->getX() << " " << vd->getY() << " " << vd->getZ();

		// view up
		Vector3D * up = view->getViewUp();
		file << " -vu " << up->getX() << " " << up->getY() << " " << up->getZ();

		// view horizontal
		file << " -vh " << view->getViewHorizontal();

		// view vertical
		file << " -vv " << view->getViewVertical();

		// close
		file << std::endl;

		// close file
		file.close();
	}
	return true;
}

bool RadExporter::writeComponentDefinitions() 
{
	// create components directory
	std::string baseDir = exportDir + "/Components";
	createdir(baseDir);

	size_t numDefinitions= model->getNumComponentDefinitions();
	for (unsigned int i = 0; i < numDefinitions; i++) {
		ComponentDefinition * definition = model->getComponentDefinitionRef(i);
		size_t numFaces = definition->getNumFaces();
		std::string componentName = definition->getName();

		// create the file
		std::ofstream file;
		file.open(baseDir + "/" + componentName + ".rad");

		// write instances
		std::vector < ComponentInstance * > * instances = definition->getComponentInstancesRef();
		size_t numInstances = instances->size();
		for (unsigned int j = 0; j < numInstances; j++) {
			writeComponentInstance(&file, definition->getComponentInstanceRef(j));
		}
		file << std::endl << std::endl;

		// export faces
		if (numFaces < 1 && numInstances < 1) {
			warn("Empty component '" + componentName + "'");
			continue;
		}
		for (unsigned int j = 0; j < numFaces; j++) {
			writeFace(&file, definition->getFaceRef(j));
		}// end of iterating faces

		// Close the file
		file.close();
	} // end of iterating definitions
	return true;
}

bool RadExporter::writeLayers() 
{

	// create geometry directory
	std::string baseDir = exportDir + "/Geometry";
	createdir(baseDir);
	size_t numLayers = model->getNumLayers();
	
	for (unsigned int i = 0; i < numLayers; i++) {
		// get the layer
		Layer * layer = model->getLayerRef(i);	
		if (layer->isEmpty()) {
			warn("Skipping layer '" + layer->getName() + "' when writing, because it is empty.");
			continue;
		}
		std::string layerName = layer->getName();

		// create the file
		std::ofstream file;
		file.open(baseDir + "/" + layerName + ".rad");
		
		// write instances
		std::vector < ComponentInstance * > * instances = layer->getComponentInstancesRef();
		size_t numInstances = instances->size();
		for (unsigned int j = 0; j < numInstances; j++) {
			writeComponentInstance(&file, layer->getComponentInstanceRef(j));
		}
		file << std::endl << std::endl;

		// check if there are faces... continue if not.
		std::vector < Face * > * faces = layer->getFacesRef();
		size_t numFaces = faces->size();
		if (numFaces < 1) {
			warn("Empty layer '" + layerName + "'");
			continue;
		}
	
		// write all faces
		for (unsigned int j = 0; j < numFaces; j++) {
			writeFace(&file, layer->getFaceRef(j));
		}
		
		// Close the file
		file.close();
	}
	return true;
}


void RadExporter::writeComponentInstance(std::ofstream * file, ComponentInstance * instance) 
{	
	ComponentDefinition * definition = instance->getDefinitionRef();
	if (definition == NULL) {
		warn("Trying to export an instance with NULL definition... instance ignored.");
		return;
	}
	*file << "!xform";  
	*file << " -s " << instance->getScale(); 
	*file << " -rz " << instance->getRotationZ(); 
	*file << " -ry " << instance->getRotationY(); 
	*file << " -rx " << instance->getRotationX(); 
	*file << " -t " << instance->getX() << " " << instance->getY() << " " << instance->getZ(); 	
	*file << " ./Geometry/" << instance->getDefinitionRef()->getName() << ".rad"; 
	*file << std::endl;
}

void RadExporter::writeLoop(std::ofstream * file, Loop * loop) 
{
	// Print the loop
	size_t numVertices = loop->size();

	for (int i = 0; i < numVertices; i++) {
		Point3D * point = loop->getVertexRef(i);

		*file << TAB;
		*file << point->getX() << TAB;
		*file << point->getY() << TAB;
		*file << point->getZ() << std::endl;
	}
}

void RadExporter::writeClosedFace(std::ofstream * file, Face * face) 
{
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
	*file << std::to_string(3 * finalLoop->size()) << std::endl;
	writeLoop(file, finalLoop);

	*file << std::endl;
	if (needToDelete) {
		delete finalLoop;
	}
}

void RadExporter::writeFace(std::ofstream * file, Face * face) 
{
	if (face->hasTooManyInnerLoops()) {
		warn("Ignoring face '" + face->getName() + "' because it has TOO MANY inner loops.");
		// writeTriangulatedFace(file,face);
		return;
	}
	else {
		writeClosedFace(file,face);		
	}
	
}