/*****************************************************************************
	Emp

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANEMP_TABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include "config_constants.h"
#include "versions.h"
#include "common/utilities/io.h"
#include "common/utilities/file.h"
#include "common/geometry/triangulation.h"
#include "groundhogmodel/groundhogmodel.h"
#include "./radexporter.h"
#include "os_definitions.h"

#include <fstream>


RadExporter::RadExporter(GroundhogModel * the_model, std::string the_exportDir, bool the_verbose) 
{
	model = the_model;
	exportDir = the_exportDir;
	verbose = the_verbose;
}

RadExporter::~RadExporter() 
{
	
}

bool RadExporter::exportModel() 
{
	inform("Beggining Radiance export", verbose);
	
	// Create the directory
	if (!createdir(exportDir)) {
		FATAL(errorMessage,"Imposible to create Output directory");
		return false;
	}

    // Create the directory
    if (!createdir("./" + exportDir + "/" + EMP_WORKPLANES_SUBFOLDER)) {
      FATAL(errorMessage, "Imposible to create Output directory");
      return false;
    }
    
	// Write layers
	if (!writeLayers(EMP_LAYERS_SUBFOLDER)) {
		FATAL(errorMessage,"Error when exporing Layers");
		return false;
	}

	// Write component definitions
	if(!writeComponentDefinitions(EMP_COMPONENTS_SUBFOLDER)) {
		FATAL(errorMessage,"Error when exporing Layers");
		return false;
	}

	// write views
	if(!writeViews(EMP_VIEWS_SUBFOLDER)) {
		FATAL(errorMessage,"Error when exporing Layers");
		return false;
	}

	// write north correction
	if(!writeModelInfo(EMP_MODEL_INFO_FILE)) {
		FATAL(errorMessage,"Error when exporing Layers");
		return false;
	}

	// write windows
	if(!writeWindows(EMP_WINDOWS_SUBFOLDER)) {
		FATAL(errorMessage,"Error when exporting Layers");
		return false;
	}

	
	// write materials
	if (!writeMaterials(EMP_MATERIALS_SUBFOLDER)) {
		FATAL(errorMessage,"Error when exporing materials");
		return false;
	}

	// Write sky
	if (!writeSky(EMP_SKY_SUBFOLDER)) {
		FATAL(errorMessage,"Error when writing the Sky");
		return false;
	}

	// Write weather
	if (!writeWeather(EMP_SKY_SUBFOLDER)) {
		FATAL(errorMessage,"Error when writing the Weather file");
		return false;
	}

	// Write Scene file
	if (!writeSceneFile(EMP_SCENE_FILE)) {
		FATAL(errorMessage,"Error when writing the Scene file");
		return false;
	}

	// write photosensors
	if (!writePhotosensors(EMP_PHOTOSENSORS_SUBFOLDER)) {
		FATAL(errorMessage,"Error when exporting Photosensors");
		return false;
	}


	return true;
}

bool RadExporter::writeModelInfo(const char * filename)
{
	// create and open file
	std::ofstream file;
	file.open(exportDir + "/" + filename);

	Date * d = model->getDate();
	Location * loc = model->getLocation();

	file << "country," << loc->getCountry() << "\n";
	file << "city," << loc->getCity() << "\n";
	file << "latitude," << loc->getLatitude() << "\n";
	file << "longitude," << loc->getLongitude() << "\n";
	file << "time zone," << loc->getTimeZone() << "\n";
	file << "month," << d->getMonth() << "\n";
	file << "day," << d->getDay() << "\n";
	file << "hour," << d->getHour() << "\n";
	file << "minute," << d->getMinute() << "\n";
	file << "north correction,"<< model->getNorthCorrection() << "\n";
	file.close();

	return true;
}

bool RadExporter::writeViews(const char * dir)
{
	size_t numViews = model->getNumViews();
	
	if (numViews == 0)
		return true;

	// create Views directory
	std::string baseDir = exportDir + "/" + dir;
	createdir(baseDir);

	// export each view
	for (size_t i = 0; i < numViews; i++) {

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
			FATAL(errorMessage,"Unkown type of view coded " + view->getViewType());
			return false;
		}

		// create and open file
		std::ofstream file;
        std::string * viewName = view->getName();
		file.open(baseDir + "/" + *viewName + ".vf");

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
		file << "\n";

		// close file
		file.close();
	}
	return true;
}

bool RadExporter::writeComponentDefinitions(const char * dir)
{
	size_t numDefinitions= model->getNumComponentDefinitions();

	if (numDefinitions == 0)
		return true;

	// create components directory
	std::string baseDir = exportDir + "/" + dir;
	createdir(baseDir);

	for (size_t i = 0; i < numDefinitions; i++) {
		ComponentDefinition * definition = model->getComponentDefinitionRef(i);
		size_t numObjects = definition->getNumObjects();
		std::string * componentName = definition->getName();

		// create the file
        std::string fileName = baseDir + "/" + *componentName + ".rad";
        
        FOPEN(file,&fileName[0], "w");

		// get instances within the model
		std::vector < ComponentInstance * > * instances = definition->getComponentInstancesRef();
		size_t numInstances = instances->size();
		
		// export faces
		if (numObjects < 1 && numInstances < 1) {
			WARN(wMsg,"Empty component '" + *componentName + "'");
			continue;
		}

		// write instances within the model
		if (numInstances > 0) {
			for (size_t j = 0; j < numInstances; j++) {
				writeComponentInstance(file, definition->getComponentInstanceRef(j));
			}
            fprintf(file, "\n\n");
		}

        

		for (size_t j = 0; j < numObjects; j++) {
          Otype * object = definition->getObjectRef(j);
          // Select Material
          Material * mat = object->getMaterial();
          if (mat == NULL) {

          }
          std::string * material = object->getMaterial()->getName();
          definition->getObjectRef(j)->writeInRadianceFormat(file,&(material->at(0)), NULL);
		}// end of iterating faces

		// Close the file
        fclose(file);
	} // end of iterating definitions
	return true;
}

bool RadExporter::writeLayers(const char * dir)
{

	size_t numLayers = model->getNumLayers();
	if (numLayers == 0) {
		warn("No layers in model");
		return true;
	}

	// create geometry directory
	std::string baseDir = exportDir + "/" + dir;
	createdir(baseDir);
	
	for (size_t i = 0; i < numLayers; i++) {
		// get the layer
		Layer * layer = model->getLayerRef(i);	
		std::string * layerName = layer->getName();
		if (layer->isEmpty()) {
			WARN(wMsg,"Skipping layer '" + *layerName + "' when writing, because it is empty.");
			continue;
		}

        std::string flnm = baseDir + "/" + *layerName + ".rad";
        
        //FILE * file = fopen(&flnm[0], "w");
        FOPEN(file, &flnm[0], "w");

		// write instances
		std::vector < ComponentInstance * > * instances = layer->getComponentInstancesRef();
		size_t numInstances = instances->size();
		for (size_t j = 0; j < numInstances; j++) {
			writeComponentInstance(file, layer->getComponentInstanceRef(j));
		}

        fprintf(file, "\n\n");		
	
		std::vector < Otype * > * objects = layer->getObjectsRef();
		size_t numObjs= objects->size();
		// write all faces
		for (size_t j = 0; j < numObjs; j++) {
          Otype * object = layer->getObjectRef(j);
          Material * mat = object->getMaterial();
          if (mat == NULL) {
            std::string * name = object->getName();   
            std::string * type = object->getType();
            warnNoMaterial(&(type->at(0)), &(name->at(0)));
            continue;
          }
          std::string * material = object ->getMaterial() ->getName();
          object->writeInRadianceFormat(file,&(material->at(0)), NULL);          
		}
		
		// Close the file
        fclose(file);
		//file.close();
	}
	return true;
}


bool RadExporter::writeLayers(FILE * file, const char * newMaterial)
{

  size_t numLayers = model->getNumLayers();
  
  for (size_t i = 0; i < numLayers; i++) {
    // get the layer
    Layer * layer = model->getLayerRef(i);
    if (layer->isEmpty()) {      
      continue;
    }
        
    
    // write instances

    // Create a transformation
    Transform * transform = new Transform();

    std::vector < ComponentInstance * > * instances = layer->getComponentInstancesRef();
    size_t numInstances = instances->size();
    for (size_t j = 0; j < numInstances; j++) {
      writeComponentInstance(file, layer->getComponentInstanceRef(j),transform,newMaterial);
    }

    // delete this transform
    delete transform;

    fprintf(file, "\n\n");

    std::vector < Otype * > * objects = layer->getObjectsRef();
    size_t numObjs = objects->size();
    // write all faces
    for (size_t j = 0; j < numObjs; j++) {

      // Get object
      Otype * object = layer->getObjectRef(j);

      // Select Material
      const char * material;

      if (newMaterial == NULL) {
        // get the material
        Material * mat = object->getMaterial();
        if (mat == NULL) {
          std::string * oName = object->getName();
          WARN(wMsg,"Face " + *oName + " has no Material... it has been ignored");
          continue;
        }
        material = &(mat->getName()->at(0));
      } else {
        material = newMaterial;
      }
      object->writeInRadianceFormat(file, material, NULL);
    }

  }
  return true;
}



void RadExporter::writeComponentInstance(FILE * file, ComponentInstance * instance) 
{	
	ComponentDefinition * definition = instance->getDefinitionRef();
	if (definition == NULL) {
		WARN(wMsg,"Trying to export an instance with NULL definition... instance ignored.");
		return;
	}

    std::string * instanceName = instance->getDefinitionRef()->getName();

    fprintf(file, "!xform -s %f -rz %f -ry %f -rx %f -t %f %f %f ../Components/%s.rad\n",
      instance->getScale(),
      instance->getRotationZ(),
      instance->getRotationY(),
      instance->getRotationX(),
      instance->getX(),
      instance->getY(),
      instance->getZ(),
      &(instanceName->at(0))
    );   
}


void RadExporter::writeComponentInstance(FILE * file, ComponentInstance * instance, Transform * parentTransform, const char * newMaterial)
{
  
  ComponentDefinition * definition = instance->getDefinitionRef();
  if (definition == NULL) {
    warn("Trying to export an instance with NULL definition... instance ignored.");
    return;
  }

  // Write the definition
  size_t numObjects = definition->getNumObjects();
  std::string * componentName = definition->getName();
  

  // get instances within the model
  std::vector < ComponentInstance * > * instances = definition->getComponentInstancesRef();
  size_t numInstances = instances->size();

  // export faces
  if (numObjects < 1 && numInstances < 1) {
    WARN(wMsg,"Empty component '" + *componentName + "'");
    return;
  }

  // Create a transformation with this instance's location
  Transform * transform = instance->getTransform(); 

  // Add the parent transform
  transform->preMultiply(parentTransform);

  // write instances within the model
  if (numInstances > 0) {
    for (size_t j = 0; j < numInstances; j++) {
      writeComponentInstance(file, definition->getComponentInstanceRef(j), transform, newMaterial);
    }
    fprintf(file, "\n\n");
  }

  for (size_t j = 0; j < numObjects; j++) {
    // Get object
    Otype * object = definition->getObjectRef(j);

    // Select Material
    const char * material;

    if (newMaterial == NULL) {
      // get the material
      Material * mat = object->getMaterial();
      if (mat == NULL) {
        std::string * oName = object->getName();
        WARN(wMsg,"Face " + *oName + " has no Material... it has been ignored");
        continue;
      }
      material = &(mat->getName()->at(0));
    }
    else {
      material = newMaterial;
    }
    object->writeInRadianceFormat(file, material, transform);
  }// end of iterating faces

  delete transform;
  
}


bool RadExporter::writeWindows(const char * dir) {
	size_t numGroups = model->getNumWindowGroups();
	if (numGroups == 0)
		return true;

	// create directory
	std::string baseDir = exportDir + "/" + dir;
	createdir(baseDir);
	std::ofstream mainFile;
	mainFile.open(baseDir + "/windows.rad");
	for (size_t i = 0; i < numGroups; i++) {
		
		WindowGroup * group = model->getWindowGroupRef(i);
		std::string name = group->getName();

		size_t numWindows = group->getNumWindows();
		if (numWindows <= 0) {
			WARN(wMsg,"Empty WindowGroup " + name);
			continue;
		}

		std::string fileName = baseDir + "/" + name + ".wingroup";

		// create and open file
        //FILE * file = fopen(&fileName[0], "w");
        FOPEN(file, &fileName[0], "w");

		mainFile << "!xform ./"<< dir <<"/" << name << ".wingroup" << "\n";

		for (size_t j = 0; j < numWindows; j++) {
			Face * window = group->getWindowRef(j);

            Material * mat = window->getMaterial();

            std::string * material = mat->getName();
            if (mat == NULL) {
              WARN(wMsg,"Window " + *material + " has not material... it will be ignored");
              continue;
            }

            window->writeInRadianceFormat(file,&(material->at(0)), NULL);
		}

        fclose(file);
	}

	mainFile.close();
	return true;
}


bool RadExporter::writeWindows(FILE * file) {
  size_t numGroups = model->getNumWindowGroups();
  
  for (size_t i = 0; i < numGroups; i++) {

    WindowGroup * group = model->getWindowGroupRef(i);
    std::string name = group->getName();

    size_t numWindows = group->getNumWindows();
    if (numWindows <= 0) {
      WARN(wMsg,"Empty WindowGroup " + name);
      continue;
    }
    
    for (size_t j = 0; j < numWindows; j++) {
      Face * window = group->getWindowRef(j);

      Material * mat = window->getMaterial();
      std::string * material = mat->getName();
      if (mat == NULL) {
        WARN(wMsg,"Window " + *material + " has not material... it will be ignored");
        continue;
      }

      window->writeInRadianceFormat(file,&(material->at(0)), NULL);
    }
    
  }

  
  return true;
}


bool RadExporter::writeMaterials(const char * dir)
{
	size_t numMaterials = model->getNumMaterials();
	if (numMaterials == 0)
		return true;

	// create directory
	std::string baseDir = exportDir + "/" + dir;
	createdir(baseDir);

	std::ofstream mainFile;
	mainFile.open(baseDir + "/materials.mat");

	for (size_t i = 0; i < numMaterials; i++) {

		Material * mat = model->getMaterialRef(i);

        std::string * name  = mat->getName();
        std::string filename = "./"+std::string(baseDir) + "/" + *name + ".mat";
        
        
        //FILE * file = fopen(&filename[0],"w");
        FOPEN(file, &filename[0], "w");

        mat->writeInRadianceFormat(file);
        fclose(file);

		mainFile << "!xform ./" << dir << "/" << *name << ".mat" << "\n";
	}
	return true;
}

bool RadExporter::writeMaterials(FILE * file)
{
  size_t numMaterials = model->getNumMaterials(); 
 
  for (size_t i = 0; i < numMaterials; i++) {
    Material * mat = model->getMaterialRef(i);    
    mat->writeInRadianceFormat(file);
  }
  return true;
}

bool RadExporter::writeSky(const char * dir)
{
  // create directory
  std::string baseDir = exportDir + "/" + dir;
  createdir(baseDir);
  std::string filename = baseDir + "/sky.rad";
  FOPEN(file, &filename[0], "w");

  writeSky(file);

  fclose(file);
  return true;
}

bool RadExporter::writeSky(FILE * file)
{
  
	Location * loc = model->getLocation();
	Date * d = model->getDate();

    fprintf(file, "!gensky %d %d %d -g %f -a %f -o %f -m %f +s \n",
      d->getMonth(),
      d->getDay(),
      d->getHour(),
      loc->getAlbedo(),
      loc->getLatitude(),
      loc->getLongitude(),
      loc->getTimeZone()*15.0
      );
    

	fprintf(file, RADIANCE_SKY_COMPLEMENT );

	return true;
}

bool RadExporter::writeSceneFile(const char * dir)
{

	std::ofstream file;
	file.open(exportDir + "/" + dir);

	// Write Header
	file << "###############" << "\n";
	file << "## Scene exported using " << EMP_VERSION << "\n";
	file << "###############" << "\n";

	file << "\n" << "\n" << "\n";

	// Write Geometry
	file << "###### GEOMETRY" << "\n" << "\n";
	for (size_t i = 0; i < model->getNumLayers(); i++) {
		std::string * name = model->getLayerRef(i)->getName();
		file << "!xform ./Geometry/" << *name << ".rad" << "\n";
	}

	file.close();

	return true;
}


bool RadExporter::writePhotosensors(const char * dir)
{
	size_t numPhotosensors = model->countPhotosensors();
	
	if (numPhotosensors == 0)
		return true;

	// create directory
	std::string baseDir = exportDir + "/" + dir;
	createdir(baseDir);

	std::ofstream mainFile;
	mainFile.open(baseDir + "/sensors.pts");

	std::ofstream dictionary;
	dictionary.open(baseDir + "/sensor_dictionary.txt");

	for (size_t i = 0; i < numPhotosensors; i++) {
		Photosensor * ph = model->getPhotosensorRef(i);
		std::string name = ph->getName();
		Point3D position = ph->getPosition();
		Vector3D direction = ph->getDirection();

		// Create its own file
		std::ofstream file;
		file.open(baseDir + "/" + name + ".pt");

		file << position.getX() << EMP_TAB;
		file << position.getY() << EMP_TAB;
		file << position.getZ() << EMP_TAB;
		file << direction.getX() << EMP_TAB;
		file << direction.getY() << EMP_TAB;
		file << direction.getZ() << "\n";
		file.close();

		// Add the sensor to the main file
		mainFile << position.getX() << EMP_TAB;
		mainFile << position.getY() << EMP_TAB;
		mainFile << position.getZ() << EMP_TAB;
		mainFile << direction.getX() << EMP_TAB;
		mainFile << direction.getY() << EMP_TAB;
		mainFile << direction.getZ() << "\n";

		// add the Sensor to the dictionary
		dictionary << i << "," << name << "\n";
	}

	dictionary.close();
	mainFile.close();

	return true;
}



bool RadExporter::writeWeather(const char * dir)
{
	std::ofstream file;
	std::string baseDir = exportDir + "/" + dir;
	file.open(baseDir + "/weather.wea");

	Location * loc = model->getLocation();
	if (!loc -> hasWeather())
		return true;

	file << "place " << loc->getCity() << "\n";
	file << "latitude " << loc->getLatitude() << "\n";
	file << "longitude " << -loc->getLongitude() << "\n";
	file << "time_zone " << loc->getTimeZone() << "\n";
	file << "site_elevation " << loc->getElevation() << "\n";
	file << "weather_data_file_units 1" << "\n";

	for (size_t i = 0; i < 8760; i++) {
		HourlyData * h = loc->getHourlyData(i);
		file << h->month << " ";
		file << h->day << " ";
		file << h->hour << " ";
		file << h->direct_nomal << " ";
		file << h->diffuse_horizontal << "\n";
	}

	file.close();

	return true;
}
