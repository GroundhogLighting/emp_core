
/*****************************************************************************
 Emp
 
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

#pragma once

#include "common/taskmanager/task.h"
#include "groundhogmodel/tasks.h"
#include "writers/rad/radexporter.h"


#include "groundhogmodel/groundhogmodel.h"
#include "common/utilities/file.h"

#include <fstream>


class WriteRadianceSceneFile : public Task {
public:
    std::string filename; //!< The name of the file to write
    GroundhogModel * model; //!< The model to export
    OptionSet options;
    
    //! Constructor
    /*!
    @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
    */
    WriteRadianceSceneFile(std::string theFilename, GroundhogModel * theModel, OptionSet * theOptions)
    {
        filename = theFilename;
        model = theModel;
        options = *theOptions;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
     bool isEqual(Task * t)
     {
         return (
             model == static_cast<WriteRadianceSceneFile *>(t)->model &&
             filename == static_cast<WriteRadianceSceneFile *>(t)->filename
         );
     }
     
     //! Solves this task
     /*!
     @author German Molina
     @return success
     */
     bool solve()
     {
         RadExporter r = RadExporter(model);
         return r.writeSceneFile(filename.c_str(), &options);
     }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
     
    
};


class WriteRadianceRifFile : public Task {
public:
    std::string filename; //!< The name of the file to write
    GroundhogModel * model; //!< The model to export
    OptionSet options;
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteRadianceRifFile(std::string theFilename, GroundhogModel * theModel, OptionSet * theOptions)
    {
        filename = theFilename;
        model = theModel;
        options = *theOptions;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteRadianceRifFile *>(t)->model &&
                filename == static_cast<WriteRadianceRifFile *>(t)->filename
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeRifFile(filename.c_str(), &options);
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};




class WriteModelInfo : public Task {
public:
    std::string filename; //!< The name of the file to write
    GroundhogModel * model; //!< The model to export
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteModelInfo(std::string theFilename, GroundhogModel * theModel)
    {
        filename = theFilename;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteRadianceSceneFile *>(t)->model &&
                filename == static_cast<WriteRadianceSceneFile *>(t)->filename
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        
        return r.writeModelInfo(filename.c_str());
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};



//! Exports a workplane in Radiance format
/*!
 This task depends on TriangulateWorkplane task.
 */
class WriteWorkplane : public Task {
    
public:
    Workplane * workplane; //!< The workplane to export
    double maxAspectRatio = 1.3; //!< The maximum aspect ratio allowed for this exportation
    double maxArea = 0.25; //!< The maximum area of the pixels (triangles) in the triangulation
    std::string exportName; //!< The name of the file to export
    
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] workplane The workplane to export
     @param[in] maxArea The maximum area allowed for the triangulation
     @param[in] maxAspectRatio the maximum aspect ratio allowed in the triangulation
     @param[in] name The name of the file to write
     */
    WriteWorkplane(Workplane * workplane, double maxArea, double maxAspectRatio, std::string name)
    {
        std::string n = "Export workplane " + *(workplane->getName());
        setName(&n);
        generatesResults = true;
        
        // Add the dependency 0
        TriangulateWorkplane * triangulateDependency = new TriangulateWorkplane(workplane);
        addDependency(triangulateDependency);
        exportName = name;
        
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                workplane == static_cast<WriteWorkplane *>(t)->workplane &&
                maxAspectRatio == static_cast<WriteWorkplane *>(t)->maxAspectRatio &&
                maxArea == static_cast<WriteWorkplane *>(t)->maxArea
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        
        // Retrieve the name
        std::string ptsFileName = "./"+exportName + ".pts";
        std::string pxlFileName = "./" + exportName + ".pxl";
        
        // Create the files
        std::ofstream ptsFile;
        ptsFile.open(ptsFileName);
        if (!ptsFile.is_open())
            throw 20;
        
        std::ofstream pxlFile;
        pxlFile.open(pxlFileName);
        if (!pxlFile.is_open())
            throw 20;
        
        // Get the dependency... has only one.
        TriangulateWorkplane * dependency = static_cast<TriangulateWorkplane *>(getDependencyRef(0));
        
        // Write down all polygons in triangulation
        size_t nRays = dependency->rays.size();
        
        // Iterate
        for (size_t i = 0; i < nRays; i++) {
            
            // Get the Triangle and ray
            Triangle * triangle = &(dependency->triangles.at(i));
            RAY * ray = &(dependency->rays.at(i));
            
            if (triangle == NULL)
                continue;
            
            // Write in Pixel file
            for (int l = 0; l < 3; l++) {
                Point3D * p = triangle->getVertex(l);
                double px = p->getX();
                double py = p->getY();
                double pz = p->getZ();
                pxlFile << px << EMP_TAB << py << EMP_TAB << pz << EMP_TAB;
            }
            pxlFile << "\n";
            
            ptsFile << ray->rorg[0] << EMP_TAB << ray->rorg[1] << EMP_TAB << ray->rorg[2] << EMP_TAB;
            ptsFile << ray->rdir[0] << EMP_TAB << ray->rdir[1] << EMP_TAB << ray->rdir[2] << "\n";
            
            
        }
            
        
        
        
        // Close the files
        ptsFile.close();
        pxlFile.close();
        
        
        return true;
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
};



class WriteComponentDefinitions : public Task {
public:
    std::string directory; //!< The name of the directory to write the components to
    GroundhogModel * model; //!< The model to export
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteComponentDefinitions(std::string theDir, GroundhogModel * theModel)
    {
        directory = theDir;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteComponentDefinitions *>(t)->model &&
                directory == static_cast<WriteComponentDefinitions *>(t)->directory
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeComponentDefinitions(directory.c_str());
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};




class WriteViews : public Task {
public:
    std::string directory; //!< The name of the directory to write the components to
    GroundhogModel * model; //!< The model to export
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteViews(std::string theDir, GroundhogModel * theModel)
    {
        directory = theDir;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteViews *>(t)->model &&
                directory == static_cast<WriteViews *>(t)->directory
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeViews(directory.c_str());
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};


class WriteCurrentSky : public Task {
public:
    std::string directory; //!< The name of the directory to write the Sky to
    GroundhogModel * model; //!< The model to export
    std::string filename; //!< The name of the file for the sky data
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteCurrentSky(std::string theDir, std::string theFilename, GroundhogModel * theModel)
    {
        directory = theDir;
        model = theModel;
        filename = theFilename;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteCurrentSky *>(t)->model &&
                directory == static_cast<WriteCurrentSky *>(t)->directory &&
                filename == static_cast<WriteCurrentSky *>(t)->filename
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeSky(directory.c_str(), filename);
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};


class WriteCurrentWeather : public Task {
public:
    std::string directory; //!< The name of the directory to write the weather in
    GroundhogModel * model; //!< The model to export
    std::string filename; //!< The name of the file for the Weather data
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteCurrentWeather(std::string theDir, std::string theFilename, GroundhogModel * theModel)
    {
        directory = theDir;
        filename = theFilename;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteCurrentWeather *>(t)->model &&
                directory == static_cast<WriteCurrentWeather *>(t)->directory &&
                filename == static_cast<WriteCurrentWeather *>(t)->filename
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeWeather(directory.c_str(), filename);
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};


class WriteMaterials : public Task {
public:
    std::string directory; //!< The name of the directory to write the Materials to
    GroundhogModel * model; //!< The model to export
    std::string filename; //!< The name of the file that will reference all materials
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteMaterials(std::string theDir, std::string theFilename, GroundhogModel * theModel)
    {
        directory = theDir;
        filename = theFilename;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteMaterials *>(t)->model &&
                directory == static_cast<WriteMaterials *>(t)->directory &&
                filename == static_cast<WriteMaterials *>(t)->filename
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeMaterials(directory.c_str(), filename);
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};



class WriteLayers : public Task {
public:
    std::string directory; //!< The name of the directory to write the components to
    GroundhogModel * model; //!< The model to export
    
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WriteLayers(std::string theDir, GroundhogModel * theModel)
    {
        directory = theDir;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WriteMaterials *>(t)->model &&
                directory == static_cast<WriteMaterials *>(t)->directory
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writeLayers(directory.c_str());
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};



class WritePhotosensors : public Task {
public:
    std::string directory; //!< The name of the directory to write the components to
    GroundhogModel * model; //!< The model to export
    
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] theFilename The file name
     @param[in] theModel The GroundhogModel object
     */
    WritePhotosensors(std::string theDir, GroundhogModel * theModel)
    {
        directory = theDir;
        model = theModel;
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<WritePhotosensors *>(t)->model &&
                directory == static_cast<WritePhotosensors *>(t)->directory
                );
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        RadExporter r = RadExporter(model);
        return r.writePhotosensors(directory.c_str());
    }
    
    //! Is mutex
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[in] t The other task
     @return true or false
     */
    bool isMutex(Task * t)
    {
        return false;
    }
    
    //! Submits the results into a json
    /*!
     This method checks whether this Task is mutual exclusive with another Task;
     but it is never mutual excusive, so it returns false
     
     @author German Molina
     @param[out] results The results json object
     @return true or false
     */
    bool submitResults(json * results)
    {
        return true;
    }
    
    
};

