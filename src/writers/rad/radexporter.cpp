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


#include "../../common/utilities/io.h"
#include "../../common/utilities/file.h"
#include "../../common/geometry/triangulation.h"
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
		fatal("Export directory '" + exportDir + "' alredy exists... please delete it.", __LINE__, __FILE__);
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

	// write windows
	writeWindows();

	// write workplanes
	writeWorkplanes();

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
			fatal("Unkown type of view coded " + view->getViewType(), __LINE__, __FILE__);
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

		if (point == NULL)
			continue;

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

bool RadExporter::writeWindows() {
	size_t numGroups = model->getNumWindowGroups();
	if (numGroups == 0)
		return true;

	// create directory
	std::string baseDir = exportDir + "/Windows";
	createdir(baseDir);
	std::ofstream mainFile;
	mainFile.open(baseDir + "/windows.rad");
	for (size_t i = 0; i < numGroups; i++) {
		
		WindowGroup * group = model->getWindowGroupRef(i);
		std::string name = group->getName();

		size_t numWindows = group->getNumWindows();
		if (numWindows < 0) {
			warn("Empty WindowGroup " + name);
			continue;
		}

		std::string fileName = baseDir + "/" + name + ".wingroup";

		// create and open file
		std::ofstream file;
		file.open(fileName);

		mainFile << "!xform ./Windows/" << name << ".wingroup" << std::endl;

		for (size_t j = 0; j < numWindows; j++) {
			Face * window = group->getWindowRef(j);
			writeFace(&file, window);
		}

		file.close();

	}

	mainFile.close();
	return true;
}


bool RadExporter::writeWorkplanes() {	
	size_t numWorkplanes = model->getNumWorkplanes();
	inform("Exporting "+std::to_string(numWorkplanes)+" workplanes", verbose);
	if (numWorkplanes == 0)
		return true;
	// create directory
	std::string baseDir = exportDir + "/Workplanes";
	createdir(baseDir);
	
	for (size_t i = 0; i < numWorkplanes; i++) {

		Workplane * wp = model->getWorkplaneRef(i);
		std::string name = wp->getName();

		size_t numPolygons = wp->getNumPolygons();
		if (numPolygons < 0) {
			warn("Empty Workplane " + name);
			continue;
		}

		std::string ptsFileName = baseDir + "/" + name + ".pts";
		std::string pxlFileName = baseDir + "/" + name + ".pxl";

		// create and open files
		std::ofstream ptsFile;
		ptsFile.open(ptsFileName);
		std::ofstream pxlFile;
		pxlFile.open(pxlFileName);
		
		bool success;
		for (size_t j = 0; j < numPolygons; j++) {
			Polygon3D * p = wp->getPolygonRef(j);
			success = writeWorkplane(&ptsFile, &pxlFile, p);
		}
		pxlFile.close();
		ptsFile.close();

		// Return false if something went wrong
		if (!success) {
			return false;
		}
	}
	return true;
}

bool RadExporter::writeWorkplane(std::ofstream * ptsFile, std::ofstream * pxlFile, Polygon3D * wp) {
	Vector3D normal = wp->getNormal();
	double nx = normal.getX();
	double ny = normal.getY();
	double nz = normal.getZ();

	Triangulation * t = new Triangulation(wp);
	t->mesh(0.5);

	size_t nTriangles = t->getNumTriangles();
	
	for (size_t i = 0; i < nTriangles; i ++ ) {
		Triangle * triangle = t->getTriangleRef(i);
		if (triangle == NULL)
			continue;

		double x = 0;
		double y = 0;
		double z = 0;
		for (int j = 0; j < 3; j++) {
			Point3D * p = triangle->getVertex(j);
			double px = p->getX();
			double py = p->getY();
			double pz = p->getZ();
			x += px;
			y += py;
			z += pz;
			*pxlFile << px << TAB << py << TAB << pz << TAB;
		}
		*pxlFile << std::endl;
		*ptsFile << x / 3 << TAB << y / 3 << TAB << z / 3 << TAB << nx << TAB << ny << TAB << nz << std::endl;
	}

	delete t;
	return true;
}