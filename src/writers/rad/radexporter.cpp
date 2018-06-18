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

#include "../../config_constants.h"
#include "../../versions.h"
#include "../../common/utilities/io.h"
#include "../../common/utilities/file.h"
#include "../../common/geometry/triangulation.h"
#include "../../groundhogmodel/groundhogmodel.h"
#include "./radexporter.h"
#include "../../os_definitions.h"

#include <fstream>


RadExporter::RadExporter(GroundhogModel * the_model)
{
	model = the_model;
}


bool RadExporter::writeModelInfo(const char * filename)
{
	// create and open file
	std::ofstream file;
	file.open(filename);

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
    std::string baseDir = std::string(dir);
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
			FATAL(errorMessage,"Unkown type of view coded " + std::to_string(view->getViewType()));
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
		Point3D vp = view->getViewPoint();
		file << " -vp " << vp.getX() << " " << vp.getY() << " " << vp.getZ();

		// view direction
		Vector3D vd = view->getViewDirection();
		file << " -vd " << vd.getX() << " " << vd.getY() << " " << vd.getZ();

		// view up
		Vector3D up = view->getViewUp();
		file << " -vu " << up.getX() << " " << up.getY() << " " << up.getZ();

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
    std::string baseDir = std::string(dir);
	createdir(baseDir);

	for (size_t i = 0; i < numDefinitions; i++) {
		ComponentDefinition * definition = model->getComponentDefinitionRef(i);
        size_t numObjects = definition->getObjectsRef()->size();
        
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

        
        // Iterate
        for (size_t j = 0; j < numObjects; j++) {
            // Get object
            writeOtype(definition->getObjectRef(j), file);
        }

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
    std::string baseDir = std::string(dir);
	createdir(baseDir);
	
    // Iterate all the layers
	for (size_t i = 0; i < numLayers; i++) {
        
		// get the layer
		Layer * layer = model->getLayerRef(i);	
		std::string * layerName = layer->getName();
        
		if (layer->isEmpty()) {
			WARN(wMsg,"Skipping layer '" + *layerName + "' when writing, because it is empty.");
			continue;
		}

        std::string flnm = baseDir + "/" + *layerName + ".rad";
        
        FOPEN(file, &flnm[0], "w");

		/* WRITE COMPONENT INSTANCES */
		std::vector < ComponentInstance * > * instances = layer->getComponentInstancesRef();
		size_t numInstances = instances->size();
		for (size_t j = 0; j < numInstances; j++) {
			writeComponentInstance(file, layer->getComponentInstanceRef(j));
		}

        fprintf(file, "\n\n");		
	
        /* WRITE OBJECTS */
        
        // Get the objects
		std::vector < Otype * > * objects = layer->getObjectsRef();
        
        // Count the objects
		size_t numObjs= objects->size();
        
        // Iterate
        for (size_t j = 0; j < numObjs; j++) {
            // Get object
            writeOtype(objects->at(j), file);
        }
        
		// Close the file
        fclose(file);
		
	}
	return true;
}


bool RadExporter::writeLayersInOneFile(FILE * file, const char * newMaterial)
{

    size_t numLayers = model->getNumLayers();

    for (size_t i = 0; i < numLayers; i++) {
        
        // get the layer
        Layer * layer = model->getLayerRef(i);
        
        /* WRITE COMPONENT INSTANCES */
        
        // Create an Identity transformation
        Transform transform = Transform();
        double scale = 1;
        // Get the instances
        std::vector < ComponentInstance * > * instances = layer->getComponentInstancesRef();
        
        // Iterate the instances
        size_t numInstances = instances->size();
        for (size_t j = 0; j < numInstances; j++) {
            writeComponentInstance(file, layer->getComponentInstanceRef(j), &transform, scale, newMaterial);
        }

        fprintf(file, "\n\n");

        /* WRITE THE OBJECTS */
        
        // Get the objects
        std::vector < Otype * > * objects = layer->getObjectsRef();
        
        // Count the objects
        size_t numObjs = objects->size();

        // Iterate
        for (size_t j = 0; j < numObjs; j++) {
            // Get object
            writeOtype(objects->at(j), file, newMaterial, &transform, 1);
        }

    }
    return true;
}

bool RadExporter::writeLayersInOneFile(FILE * file)
{
    return writeLayersInOneFile(file, nullptr);
}

void RadExporter::writeComponentInstance(FILE * file, ComponentInstance * instance) 
{	
	ComponentDefinition * definition = instance->getDefinitionRef();
	if (definition == nullptr) {
		WARN(wMsg,"Trying to export an instance with nullptr definition... instance ignored.");
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


void RadExporter::writeComponentInstance(FILE * file, ComponentInstance * instance, Transform * parentTransform, double scale, const char * newMaterial)
{
  
    ComponentDefinition * definition = instance->getDefinitionRef();
    if (definition == nullptr) {
        warn("Trying to export an instance with nullptr definition... instance ignored.");
        return;
    }

    // Get the definition Name
    std::string * componentName = definition->getName();

    // Count objects
    size_t numObjects = definition->getObjectsRef()->size();

    // get instances within the model
    std::vector < ComponentInstance * > * instances = definition->getComponentInstancesRef();
    size_t numInstances = instances->size();

    // export faces
    if (numObjects < 1 && numInstances < 1) {
        WARN(wMsg,"Empty ComponentDefinition '" + *componentName + "'");
        return;
    }

    // Create a transformation with this instance's location
    Transform * transform = instance->getTransform();

    // Add the parent transform
    transform->preMultiply(parentTransform);
    scale *= instance->getScale();
    
    // write instances within the model
    if (numInstances > 0) {
        for (size_t j = 0; j < numInstances; j++) {
          writeComponentInstance(file, definition->getComponentInstanceRef(j), transform, scale, newMaterial);
        }
        fprintf(file, "\n\n");
    }

    // Iterate
    for (size_t j = 0; j < numObjects; j++) {
        // Get object
        writeOtype(definition->getObjectRef(j), file, newMaterial, transform, scale);
    }
    
    delete transform;
  
}


bool RadExporter::writeWindows(const char * dir) {
	size_t numGroups = model->getNumWindowGroups();
	if (numGroups == 0)
		return true;

	// create directory
	std::string baseDir = std::string(dir);
	createdir(baseDir);
	std::ofstream mainFile;
	mainFile.open(baseDir + "/windows.rad");
	for (size_t i = 0; i < numGroups; i++) {
		
		WindowGroup * group = model->getWindowGroupRef(i);
		std::string name = group->getName();

		size_t numWindows = group->size();
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
            if (mat == nullptr) {
              WARN(wMsg,"Window " + *material + " has not material... it will be ignored");
              continue;
            }
            writeOtype(window, file, &(material->at(0)));
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

        size_t numWindows = group->size();
        if (numWindows <= 0) {
            WARN(wMsg,"Empty WindowGroup " + name);
            continue;
        }
    
        for (size_t j = 0; j < numWindows; j++) {
            Face * window = group->getWindowRef(j);

            Material * mat = window->getMaterial();
            std::string * material = mat->getName();
            if (mat == nullptr) {
                WARN(wMsg,"Window " + *material + " has not material... it will be ignored");
                continue;
            }
            writeOtype(window, file, &(material->at(0)));
        }
    }

  return true;
}


bool RadExporter::writeMaterials(const char * dir, const std::string flnm)
{
	size_t numMaterials = model->getNumMaterials();
	if (numMaterials == 0)
		return true;

	// create directory
	std::string baseDir = std::string(dir);
	createdir(baseDir);

	std::ofstream mainFile;
	mainFile.open("./" +baseDir + "/" + flnm);

    const char * materialsSubdir = strchr(&baseDir[0],'/');
    
	for (size_t i = 0; i < numMaterials; i++) {

		Material * mat = model->getMaterialRef(i);

        std::string * name  = mat->getName();
        std::string filename = "./"+std::string(baseDir) + "/" + *name + ".mat";
        
        
        //FILE * file = fopen(&filename[0],"w");
        FOPEN(file, &filename[0], "w");

        writeMaterial(mat,file);
        
        fclose(file);

		mainFile << "!xform ." << materialsSubdir << "/" << *name << ".mat" << "\n";
	}
    mainFile.close();
	return true;
}

bool RadExporter::writeMaterials(FILE * file)
{
  size_t numMaterials = model->getNumMaterials(); 
 
  for (size_t i = 0; i < numMaterials; i++) {
      Material * mat = model->getMaterialRef(i);
      writeMaterial(mat,file);
  }
  return true;
}

bool RadExporter::writeSky(const char * dir, const std::string flnm)
{
  // create directory
  std::string baseDir = std::string(dir);
  createdir(baseDir);
  std::string filename = baseDir + "/" + flnm;
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

bool RadExporter::writeSceneFile(const char * dir, OptionSet * options)
{

	std::ofstream file;
	file.open(dir);

	// Write Header
	file << "###############" << "\n";
	file << "## Scene exported using " << EMP_CORE_VERSION << "\n";
	file << "###############" << "\n";

	file << "\n" << "\n" << "\n";

    // Retrieve directories
    std::string layersLocation = options->getOption<std::string>("layers_directory");
    
	// Write Geometry
	file << "###### GEOMETRY" << "\n" << "\n";
	for (size_t i = 0; i < model->getNumLayers(); i++) {
		std::string * name = model->getLayerRef(i)->getName();
		file << "!xform ./" << layersLocation << "/" << *name << ".rad" << "\n";
	}

	file.close();

	return true;
}


bool RadExporter::writeRifFile(const char * dir, OptionSet * options)
{
    std::ofstream file;
    file.open(dir);

    // Retrieve directories
    std::string layersLocation = options->getOption<std::string>("layers_directory");
    std::string viewsLocation = options->getOption<std::string>("views_directory");
    std::string skiesLocation = options->getOption<std::string>("skies_directory");
    std::string materialsLocation = options->getOption<std::string>("materials_directory");
    std::string materialsFile = options->getOption<std::string>("materials_file");
    std::string sceneFile = options->getOption<std::string>("scene_file");
    std::string componentsLocation = options->getOption<std::string>("components_directory");
    std::string windowsLocation = options->getOption<std::string>("windows_directory");
    std::string illumsLocation = options->getOption<std::string>("illums_directory");
    
    
    file << "###############" << std::endl;
    file << "## RIF exported using " << EMP_CORE_VERSION << std::endl;
    file << "###############" << std::endl << std::endl << std::endl ;
    
    
    // Get BBOx
    file << "#ZONE= I #{min.x.to_m} #{max.x.to_m} #{min.y.to_m} #{max.y.to_m} #{min.z.to_m}  #{max.z.to_m}" << "\n";
    file << "UP=Z" << "\n";
    file << "scene=./" << skiesLocation << "/sky.rad ./" << sceneFile  << "\n";
    file << "materials=./" << materialsLocation << "/" << materialsFile << "\n";
    file << "QUAL=LOW" << "\n";
    file << "DETAIL=LOW" << "\n";
    file << "VAR=High" << "\n";
    file << "RESOLUTION=560 560" << "\n";
    file << "AMBFILE=ambient.amb" << "\n";
    file << "INDIRECT=3" << "\n";
    file << "PENUMBRAS=True" << "\n";
    file << "REPORT=2" << "\n";
    
    
    //then the pages
    file << "\n\n#VIEWS\n\n" << "\n";
    
    size_t nViews = model->getNumViews();
    for(size_t i=0; i<nViews;i++){
        View * view = model->getViewRef(i);
        std::string name = *(view->getName());
        file << "view=" << name << " -vf " << viewsLocation << "/" << name << ".vf" << "\n";
        
    }
    
        /*
    //Then the illums
        f.puts("\n\n#ILLUMS\n\n")
        illums.each do |ill|
            name=Labeler.get_fixed_name(ill)
            f.puts("illum=./Illums/"+name+".rad\n")
            end
            
            
            
    //Then the window groups
            f.puts("\n\n#WINDOW GROUPS\n\n")
            groups=Utilities.get_win_groups(windows)
            groups.each do |gr|
                f.puts("illum=./Windows/"+gr.tr(" ","_")+".rad\n")
                end
                
    //then the rest of the windows
                f.puts("\n\n#OTHER WINDOWS\n\n")
                nwin=1 #this will count the windows
                windows.each do |win|
                    c=Labeler.get_win_group(win)
                    if c==nil then # if the window has no group
                        
                        winname=win.get_attribute("Groundhog","Name") #get the name
                        if winname==nil then #if it does not have one
                            f.puts("./Windows/WindowSet_"+nwin.to_s+".rad\n")
                            nwin=nwin+1
                            else #if it has one
                                f.puts("./Windows/"+winname+".rad\n")
                                end
                                end
                                end
*/
    
    file.close();
    return true;
}

bool RadExporter::writePhotosensors(const char * dir)
{
	size_t numPhotosensors = model->countPhotosensors();
	
	if (numPhotosensors == 0)
		return true;

	// create directory
	std::string baseDir = std::string(dir);
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



bool RadExporter::writeWeather(const char * dir, const std::string filename)
{
    Location * loc = model->getLocation();
    if (!loc -> hasWeather())
        return true;

    std::ofstream file;
	std::string baseDir = std::string(dir);
	file.open(baseDir + "/" + filename);


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
		file << h->direct_normal << " ";
		file << h->diffuse_horizontal << "\n";
	}

	file.close();
    
    return true;
}

bool RadExporter::writeBubble(Bubble * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    fprintf(file, "%s %s %s\n", material, "bubble", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    Point3D center = object->center;
    double radius = object->radius;
    
    if (transform == nullptr) {
        fprintf(file, "4 %f %f %f %f\n", center.getX(), center.getY(), center.getZ(),radius*scale);
    }
    else {
        Point3D p = center.transform(transform);
        fprintf(file, "4 %f %f %f %f\n", p.getX(), p.getY(), p.getZ(), radius);
    }
    
    return true;
}

bool RadExporter::writeCone(Cone * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    fprintf(file, "%s %s %s\n", material, "cone", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    // Print arguments
    Point3D p0 = object->p0;
    Point3D p1 = object->p1;
    double r0 = object->r0;
    double r1 = object->r1;
    
    if (transform == nullptr) {
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", p0.getX(), p0.getY(), p0.getZ(), p1.getX(), p1.getY(), p1.getZ(), r0, r1);
    }
    else {
        Point3D tp0 = Point3D(scale*p0.getX(),scale*p0.getY(),scale*p0.getZ()).transform(transform);
        Point3D tp1 = Point3D(scale*p1.getX(),scale*p1.getY(),scale*p1.getZ()).transform(transform);
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", tp0.getX(), tp0.getY(), tp0.getZ(), tp1.getX(), tp1.getY(), tp1.getZ(), r0*scale, r1*scale);
    }
    
    return true;
}

bool RadExporter::writeCup(Cup * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    fprintf(file, "%s %s %s\n", material, "cup", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    // Print arguments
    Point3D p0 = object->p0;
    Point3D p1 = object->p1;
    double r0 = object->r0;
    double r1 = object->r1;
    
    if (transform == nullptr) {
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", p0.getX(), p0.getY(), p0.getZ(), p1.getX(), p1.getY(), p1.getZ(), r0, r1);
    }
    else {
        Point3D tp0 = Point3D(scale*p0.getX(),scale*p0.getY(),scale*p0.getZ()).transform(transform);
        Point3D tp1 = Point3D(scale*p1.getX(),scale*p1.getY(),scale*p1.getZ()).transform(transform);
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", tp0.getX(), tp0.getY(), tp0.getZ(), tp1.getX(), tp1.getY(), tp1.getZ(), r0*scale, r1*scale);
    }
    
    return true;
}

bool RadExporter::writeCylinder(Cylinder * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    fprintf(file, "%s %s %s\n", material, "cylinder", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    // Print arguments
    Point3D p0 = object->p0;
    Point3D p1 = object->p1;
    double radius = object->radius;
    
    
    if (transform == nullptr) {
        fprintf(file, "7 %f %f %f %f %f %f %f\n", p0.getX(), p0.getY(), p0.getZ(), p1.getX(), p1.getY(), p1.getZ(), radius);
    }
    else {
        Point3D tp0 = Point3D(scale*p0.getX(),scale*p0.getY(),scale*p0.getZ()).transform(transform);
        Point3D tp1 = Point3D(scale*p1.getX(),scale*p1.getY(),scale*p1.getZ()).transform(transform);
        fprintf(file, "7 %f %f %f %f %f %f %f\n", tp0.getX(), tp0.getY(), tp0.getZ(), tp1.getX(), tp1.getY(), tp1.getZ(), radius*scale);
    }
    
    return true;
    
}

bool RadExporter::writeFace(Face * face, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = face->getName();
    
    fprintf(file, "%s %s %s\n", material, "polygon", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    
    if (face->hasTooManyInnerLoops()) {
        WARN(warnMessage,"Ignoring face '" + *oName + "' because it has TOO MANY inner loops.");
        // writeTriangulatedFace(file,face);
        return true;
    }
    
    // define the loop that will be written
    Loop * finalLoop = NULL;
    bool needToDelete = false;
    if (face->hasInnerLoops()) {
        finalLoop = face->getClosedLoop();
        needToDelete = true;
    }
    else {
        finalLoop = face->getOuterLoopRef();
    }
    
    // Print number of 3 x vertices
    fprintf(file,"%zd\n",3 * finalLoop->realSize());
    
    // Print the loop
    size_t numVertices = finalLoop->size();
    
    for (int i = 0; i < numVertices; i++) {
        Point3D * point = finalLoop->getVertexRef(i);
        
        if (point == NULL)
            continue;
        
        fprintf(file, "\t");
        if (transform == nullptr) {
            fprintf(file, "%f %f %f\n", point->getX(), point->getY(), point->getZ());
        }
        else {
            Point3D p = Point3D(scale*point->getX(), scale*point->getY(), scale*point->getZ());
            p = p.transform(transform);
            fprintf(file, "%f %f %f\n", p.getX(), p.getY(), p.getZ());
        }
    }
    
    
    if (needToDelete) {
        delete finalLoop;
    }
    
    
    return true;
}

bool RadExporter::writeRing(Ring * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    fprintf(file, "%s %s %s\n", material, "ring", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    // Print arguments
    Point3D center = object->center;
    Vector3D direction = object->direction;
    double r0 = object->r0;
    double r1 = object->r1;
    
    if (transform == nullptr) {
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", center.getX(), center.getY(), center.getZ(), direction.getX(), direction.getY(), direction.getZ(), r0, r1);
    }
    else {
        Point3D tp0 = Point3D(scale*center.getX(),scale*center.getY(),scale*center.getZ()).transform(transform);
        Vector3D tp1 = direction.transform(transform);
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", tp0.getX(), tp0.getY(), tp0.getZ(), tp1.getX(), tp1.getY(), tp1.getZ(), r0*scale, r1*scale);
    }
    
    return true;
}


bool RadExporter::writeSource(Source * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    
    fprintf(file, "%s %s %s\n", material, "source", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    // Print arguments
    Vector3D direction = object->direction;
    double angle = object->angle;
    
    if (transform == nullptr) {
        fprintf(file, "4 %f %f %f %f\n", direction.getX(), direction.getY(), direction.getZ(), angle);
    }
    else {
        Vector3D tp1 = direction.transform(transform);
        fprintf(file, "4 %f %f %f %f\n", tp1.getX(), tp1.getY(), tp1.getZ(), angle*scale);
    }
    return true;
}

bool RadExporter::writeSphere(Sphere * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
    
    fprintf(file, "%s %s %s\n", material, "sphere", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    Point3D center = object->center;
    double radius = object->radius;
    
    if (transform == nullptr) {
        fprintf(file, "4 %f %f %f %f\n", center.getX(), center.getY(), center.getZ(),radius*scale);
    }
    else {
        Point3D p = center.transform(transform);
        fprintf(file, "4 %f %f %f %f\n", p.getX(), p.getY(), p.getZ(), radius);
    }
    
    
    return true;
}
    

bool RadExporter::writeTube(Tube * object, FILE * file, const char * material, Transform * transform, double scale)
{
    std::string * oName = object->getName();
 
    fprintf(file, "%s %s %s\n", material, "tube", oName->c_str());
    fprintf(file, "0\n");
    fprintf(file, "0\n");
    
    // Print arguments
    Point3D p0 = object->p0;
    Point3D p1 = object->p1;
    double radius = object->radius;
    
    
    if (transform == nullptr) {
        fprintf(file, "7 %f %f %f %f %f %f %f\n", p0.getX(), p0.getY(), p0.getZ(), p1.getX(), p1.getY(), p1.getZ(), radius);
    }
    else {
        Point3D tp0 = Point3D(scale*p0.getX(),scale*p0.getY(),scale*p0.getZ()).transform(transform);
        Point3D tp1 = Point3D(scale*p1.getX(),scale*p1.getY(),scale*p1.getZ()).transform(transform);
        fprintf(file, "7 %f %f %f %f %f %f %f\n", tp0.getX(), tp0.getY(), tp0.getZ(), tp1.getX(), tp1.getY(), tp1.getZ(), radius*scale);
    }
    
    return true;
}

bool RadExporter::writeOtype(Otype * object, FILE * file, const char * material, Transform * transform, double scale)
{
    // Check type of object
    std::string * type = object->getType();
    std::string * oName = object->getName();
    
    // Verify the material
    if(material == nullptr){
        Material * m = object->getMaterial();
        if(m == nullptr){
            WARN(wMsg,"Object '" + *oName + "' has no Material... it has been ignored");
            return false;
        }
        material = m->getName()->c_str();
    }
    
    // Write down
    if(strcmp(type->c_str(),"bubble") == 0){
        
        writeBubble(static_cast<Bubble *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"cone") == 0){
        
        writeCone(static_cast<Cone *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"cup") == 0){
        
        writeCup(static_cast<Cup *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"cylinder") == 0){
        
        writeCylinder(static_cast<Cylinder *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"polygon") == 0){
        
        writeFace(static_cast<Face *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"ring") == 0){
        
        writeRing(static_cast<Ring *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"source") == 0){
        
        writeSource(static_cast<Source *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"sphere") == 0){
        
        writeSphere(static_cast<Sphere *>(object), file, material, transform, scale);
        
    } else if(strcmp(type->c_str(),"tube") == 0){
        
        writeTube(static_cast<Tube *>(object), file, material, transform, scale);
        
    } else {
        std::string errMsg = "Unkown Otype '"+*type+"' in object called '"+*oName+"' when trying to write it in Radiance format... ignoring it";
        WARN(e,errMsg.c_str());
        return false;
    }
    fprintf(file,"\n");
    
    return true;
}

bool RadExporter::writeOtype(Otype * object, FILE * file)
{
    return writeOtype(object, file, nullptr, nullptr, 1);
}

bool RadExporter::writeOtype(Otype * object, FILE * file, const char * material)
{
    return writeOtype(object, file, material, nullptr, 1);
}


bool RadExporter::writeMaterial(Material * material, FILE * file)
{
    // Check type of material
    std::string * type = material->getType();
    std::string * name = material->getName();
    
    // Write down
    if(strcmp(type->c_str(),"dielectric") == 0){
        
        Dielectric * m = static_cast<Dielectric *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "5 %f %f %f %f %f\n", m->r, m->g, m->b, m->refractionIndex, m->hartmannConstant);
        
    } else if(strcmp(type->c_str(),"glass") == 0){
        
        Glass * m = static_cast<Glass *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "3 %f %f %f\n", m->r, m->g, m->b);
        
    } else if(strcmp(type->c_str(),"glow") == 0){
        
        Glow * m = static_cast<Glow *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "4 %f %f %f %f\n", m->r, m->g, m->b, m->maxRadius);
        
    } else if(strcmp(type->c_str(),"interface") == 0){
        
        Interface * m = static_cast<Interface *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "8 %f %f %f %f %f %f %f %f\n", m->r1, m->g1, m->b1, m->refractionIndex1,
                m->r2, m->g2, m->b2, m->refractionIndex2);
        
    } else if(strcmp(type->c_str(),"light") == 0){
        
        Light * m = static_cast<Light *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "3 %f %f %f\n", m->r, m->g, m->b);
        
    } else if(strcmp(type->c_str(),"metal") == 0){
        
        Metal * m = static_cast<Metal *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "5 %f %f %f %f %f\n", m->r, m->g, m->b, m->specularity, m->roughness);
        
    } else if(strcmp(type->c_str(),"plastic") == 0){
        
        Plastic * m = static_cast<Plastic *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "5 %f %f %f %f %f\n", m->r, m->g, m->b, m->specularity, m->roughness);
        
    } else if(strcmp(type->c_str(),"spotlight") == 0){
        
        Spotlight * m = static_cast<Spotlight *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "7 %f %f %f %f %f %f %f\n", m->r, m->g, m->b, m->angle, m->direction.getX(), m->direction.getY(), m->direction.getZ());
        
    } else if(strcmp(type->c_str(),"trans") == 0){
        
        Trans * m = static_cast<Trans *>(material);
        fprintf(file, "void %s %s\n0\n0\n", type->c_str(), name->c_str());
        fprintf(file, "7 %f %f %f %f %f %f %f\n", m->r, m->g, m->b, m->specularity, m->roughness, m->transmissivity, m->tspec);
        
    } else {
        std::string errMsg = "Unkown Material '"+*type+"' in material called '"+*name+"' when trying to write it in Radiance format... ignoring it";
        WARN(e,errMsg.c_str());
        return false;
    }
    
    fprintf(file,"\n");
    
    return true;
}

