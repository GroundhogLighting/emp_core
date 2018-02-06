
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

#include "groundhogmodel/tasks.h"
#include "common/taskmanager/task.h"
#include "common/utilities/stringutils.h"
#include "calculations/oconv_options.h"
#include "calculations/radiance.h"
#include "writers/rad/radexporter.h"
#include "reinhart.h"

class OconvTask : public Task {
    
public:
    
    GroundhogModel * model; //!< The model to Oconv
    OconvOptions options; //!< The Options passed to Oconv
    std::string octreeName; //!< The name of the created octree
    
    OconvTask(GroundhogModel * theModel, OconvOptions * theOptions)
    {
        options = *theOptions;
        std::string name = buildName();
        setName(&name);
        model = theModel;
    }
    
    ~OconvTask()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<OconvTask *>(t)->model &&
                options.isEqual(&static_cast<OconvTask *>(t)->options)
                );
    }
    
    bool solve()
    {
        RadExporter exporter = RadExporter(model);
        octreeName = *getName() + ".oct";
        //remove(&octreeName[0]);
        if (!oconv(octreeName, &options, exporter)) {
            FATAL(errmsg, "Impossible to oconv");
            return false;
        }
        
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
    
    std::string buildName()
    {
        std::string ret = "Oconv";
        
        ret += options.getOption<bool>(std::string(OCONV_INCLUDE_WINDOWS)) ? "1." : "0.";
        ret += options.getOption<bool>(std::string(OCONV_USE_BLACK_GEOMETRY)) ? "1." : "0.";
        ret += options.getOption<bool>(std::string(OCONV_INCLUDE_SKY)) ? "1." : "0.";
        ret += options.getOption<bool>(std::string(OCONV_LIGHTS_ON)) ? "1." : "0.";
        
        std::string sky = options.getOption<std::string>(std::string(OCONV_SKY));
        fixString(&sky[0],sky.size());
        ret += sky;
        
        return ret;
    }
};


#include "os_definitions.h"

class Create4CMDirectSkyOctree : public Task {
public:
    GroundhogModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    
    Create4CMDirectSkyOctree(GroundhogModel * theModel)
    {
        
        std::string name = "Create CreateDirectSkyOctree";
        setName(&name);
        model = theModel;
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);
        oconvOptions.setOption(OCONV_SKY, "current");
        oconvOptions.setOption(OCONV_INCLUDE_SKY, false);
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);// --> Dependency 0
        
    }
    
    ~Create4CMDirectSkyOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return model == static_cast<Create4CMDirectSkyOctree *>(t)->model;
    }
    
    bool solve()
    {
        
        const double pi = 3.141592654;
        const double desiredElementWidth = 0.183333; // Degrees
        const double dAngle = 0.4 * pi / 180.0;
        const double elementBrighness =  pi/0.51757717132568359;
        
        std::string octName = *(static_cast<OconvTask *>(getDependencyRef(0))->getName()) + ".oct";
        octreeName = "DIRECT_SKY_" + octName;
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        //remove(&octreeName[0]);
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "void light element 0 0 3 %f %f %f\n",elementBrighness,elementBrighness,elementBrighness);
        
        
        
        // define altitide
        double altitude = dAngle/2.0;
        
        
        // The numbr of sources that fit a band
        size_t nSourcesInBand;
        
        // Element width
        double elementWidth;
        
        // Iterate going up
        int band = 0;
        while (altitude < 1.57079632679){
            nSourcesInBand = floor(pi * cos(altitude) / tan(dAngle/2.0));
            
            // Increase the solid angle
            elementWidth = desiredElementWidth;
            
            // Write
            double dAz = 2*pi/nSourcesInBand;
            double az = dAz/2;
            for (int i = 0; i < nSourcesInBand; i++){
                fprintf(octree,"element source %d_%d 0 0 4 %f %f %f %f\n", band, i, sin(az)*cos(altitude),cos(az)*cos(altitude),sin(altitude),elementWidth);
                
                az += dAz;
            }
            band ++;
            altitude += dAngle;
        }
        
        PCLOSE(octree);
        
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



class CreateDirectSunOctree : public Task {
public:
    GroundhogModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    int mf = 1; //!< The reinhart subdivition sheme
    
    CreateDirectSunOctree(GroundhogModel * theModel, int theMf)
    {
        
        std::string name = "Create CreateDirectSunOctree";
        setName(&name);
        model = theModel;
        mf = theMf;
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);
        oconvOptions.setOption(OCONV_SKY, "current");
        oconvOptions.setOption(OCONV_INCLUDE_SKY, false);
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);// --> Dependency 0
        
    }
    
    ~CreateDirectSunOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return model == static_cast<CreateDirectSunOctree *>(t)->model;
    }
    
    bool solve()
    {
        std::string octName = *(static_cast<OconvTask *>(getDependencyRef(0))->getName()) + ".oct";
        octreeName = "DIRECT_SUN_" + octName;
        //remove(&octreeName[0]);
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "void light solar 0 0 3 1e6 1e6 1e6\n");
        size_t nbins = nReinhartBins(mf);
        Vector3D dir = Vector3D(0,0,0);
        for(size_t bin = 1; bin <= nbins; bin++){
            dir = reinhartCenterDir(bin,mf);
            fprintf(octree, "solar source sun 0 0 4 %f %f %f 0.533\n", dir.getX(), dir.getY(), dir.getZ());            
        }
        
        PCLOSE(octree);
        
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


class Create4CMNaiveDirectSkyOctree : public Task {
public:
    GroundhogModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    int mf = 1; //!< The reinhart subdivition sheme
    
    Create4CMNaiveDirectSkyOctree(GroundhogModel * theModel, int theMf)
    {
        
        std::string name = "Create CreateNaiveDirectSkyOctree";
        setName(&name);
        model = theModel;
        mf = theMf;
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);
        oconvOptions.setOption(OCONV_SKY, "current");
        oconvOptions.setOption(OCONV_INCLUDE_SKY, false);
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);// --> Dependency 0
        
    }
    
    ~Create4CMNaiveDirectSkyOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return model == static_cast<Create4CMNaiveDirectSkyOctree *>(t)->model;
    }
    
    bool solve()
    {
        std::string octName = *(static_cast<OconvTask *>(getDependencyRef(0))->getName()) + ".oct";
        octreeName = "NAIVE_DIRECT_SKY_" + octName;
        //remove(&octreeName[0]);
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "void light solar 0 0 3 1 1 1\n");
        size_t nbins = nReinhartBins(mf);
        Vector3D dir = Vector3D(0,0,0);
        for(size_t bin = 1; bin <= nbins; bin++){
            dir = reinhartCenterDir(bin,mf);
            fprintf(octree, "solar source sun 0 0 4 %f %f %f 11.000\n", dir.getX(), dir.getY(), dir.getZ());
        }
        
        PCLOSE(octree);
        
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


class RTraceTask : public Task {
    
public:
    GroundhogModel * model;
    RTraceOptions rTraceOptions;
    OptionSet otherOptions;
    Workplane * workplane;
    OconvOptions oconvOptions;
    std::vector < std::vector<RAY>> rays;
    
    RTraceTask(GroundhogModel * theModel, RTraceOptions * theOptions, OptionSet * theOtherOptions, Workplane * wp, OconvOptions * theOconvOptions)
    {
        model = theModel;
        rTraceOptions = *theOptions;
        oconvOptions = *theOconvOptions;
        workplane = wp;
        otherOptions = *theOtherOptions;
        
        // Create and add the OCONV dependency ---> 0
        OconvTask * oconvTask = new OconvTask(model, &oconvOptions);
        std::string octname = *(oconvTask->getName()) + ".oct";
        addDependency(oconvTask);
        otherOptions.addOption("ambient_file", octname + ".amb");
        
        // Create and add the TriangulateWorkplane dependency
        double maxArea = otherOptions.getOption<double>("max_area");
        double maxAspectRatio = otherOptions.getOption<double>("max_aspect_ratio");
        
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp, maxArea, maxAspectRatio);
        addDependency(triangulateWorkplaneTask); // Dependency 1
        
        
        // Set the name
        std::string name = "Rtrace "+(*wp->getName())+ "_"+ octname +"_"+std::to_string(maxArea) + "_" + std::to_string(maxAspectRatio);
        setName(&name);
    }
    
    
    bool isEqual(Task * t)
    {
        return (
                rTraceOptions.isEqual(& static_cast<RTraceTask *>(t)->rTraceOptions) &&
                oconvOptions.isEqual(& static_cast<RTraceTask *>(t)->oconvOptions) &&
                workplane == static_cast<RTraceTask *>(t)->workplane
                );
    }
    
    bool solve()
    {
        if (!otherOptions.hasOption("ambient_file")) {
            FATAL(err, "No 'ambient_file' option in RTraceTask");
            return false;
        };
        std::string ambient = otherOptions.getOption<std::string>("ambient_file");
        
        std::string octname = static_cast<OconvTask *>(getDependencyRef(0))->octreeName;
        
        
        std::vector<RAY> * rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        rtrace_I(&rTraceOptions, &octname[0], ambient, rays);
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        //return (typeid(*this) == typeid(*t));
        return oconvOptions.isEqual(&static_cast<RTraceTask *>(t)->oconvOptions);
    }
    
    bool submitResults(json * results)
    {
        // count total rays
        size_t nrays = 0;
        for (auto rayArray : rays) {
            nrays += rayArray.size();
        }
        std::vector<double> res = std::vector<double>(nrays);
        size_t i = 0;
        for (auto rayArray: rays) {
            for (auto ray : rayArray) {
                res[i++] = LIGHT( ray.rcol );
            }
        }
        std::string * name = getName();
        
        (*results)[*name] = res;
        
        return true;
    }
    
};


#include "./color_matrix.h"

class CalculateDirectSkyMatrix : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart sky subdivition scheme
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    CalculateDirectSkyMatrix(GroundhogModel * theModel, Workplane * wp, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        Create4CMDirectSkyOctree * oconvTask = new Create4CMDirectSkyOctree(model);
        addDependency(oconvTask);
        
        // Dependecy 1: Triangulate workplane
        double maxArea = 0.25; //otherOptions.getOption<double>("max_area");
        double maxAspectRatio = 1.3; //otherOptions.getOption<double>("max_aspect_ratio");
        
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp, maxArea, maxAspectRatio);
        addDependency(triangulateWorkplaneTask);
    
        
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    CalculateDirectSkyMatrix(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        Create4CMDirectSkyOctree * oconvTask = new Create4CMDirectSkyOctree(model);
        addDependency(oconvTask);
        
        // Set the rays
        rays = theRays;
        
    }
    
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDirectSkyMatrix *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDirectSkyMatrix *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDirectSkyMatrix *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDirectSkyMatrix *>(t)->rays);
        }
        
        FATAL(e,"CalculateDirectSkyMatrix with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        std::string octname = static_cast<Create4CMDirectSkyOctree *>(getDependencyRef(0))->octreeName;
        RTraceOptions options = RTraceOptions();
        options.setOption("ab",0);
        options.setOption("dc",1);
        options.setOption("dt",0);
        
        
        // If there is a workplane, get rays from triangulation
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        rcontrib(&options, &octname[0], false, true, rays, mf, "element", true, &result);
        
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



class CalculateDirectSunMatrix : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart sky subdivition scheme
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    
    CalculateDirectSunMatrix(GroundhogModel * theModel, Workplane * wp, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        CreateDirectSunOctree * oconvTask = new CreateDirectSunOctree(model, mf);
        addDependency(oconvTask);
        
        
        // Dependecy 1: Triangulate workplane
        double maxArea = 0.25; //otherOptions.getOption<double>("max_area");
        double maxAspectRatio = 1.3; //otherOptions.getOption<double>("max_aspect_ratio");
        
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp, maxArea, maxAspectRatio);
        addDependency(triangulateWorkplaneTask);
    }
    
    
    CalculateDirectSunMatrix(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        CreateDirectSunOctree * oconvTask = new CreateDirectSunOctree(model, mf);
        addDependency(oconvTask);
        
        rays = theRays;
    }
    
    
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDirectSunMatrix *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDirectSunMatrix *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDirectSunMatrix *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDirectSunMatrix *>(t)->rays);
        }
        
        FATAL(e,"CalculateDirectSunMatrix with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        
        
        std::string octname = static_cast<CreateDirectSunOctree *>(getDependencyRef(0))->octreeName;
        RTraceOptions options = RTraceOptions();
        options.setOption("ab",0);
        options.setOption("dc",1);
        options.setOption("dt",0);
        
        
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        rcontrib(&options, &octname[0], false, true, rays, mf, "element", true, &result);
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


class Calculate4CMNaiveDirectSkyMatrix : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart sky subdivition scheme
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    
    Calculate4CMNaiveDirectSkyMatrix(GroundhogModel * theModel, Workplane * wp, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        Create4CMNaiveDirectSkyOctree * oconvTask = new Create4CMNaiveDirectSkyOctree(model, mf);
        addDependency(oconvTask);
        
        // Dependecy 1: Triangulate workplane
        double maxArea = 0.25; //otherOptions.getOption<double>("max_area");
        double maxAspectRatio = 1.3; //otherOptions.getOption<double>("max_aspect_ratio");
        
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp, maxArea, maxAspectRatio);
        addDependency(triangulateWorkplaneTask);
        
    }
    
    Calculate4CMNaiveDirectSkyMatrix(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        Create4CMNaiveDirectSkyOctree * oconvTask = new Create4CMNaiveDirectSkyOctree(model, mf);
        addDependency(oconvTask);
        
        rays = theRays;
        
    }
    
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<Calculate4CMNaiveDirectSkyMatrix *>(t)->model);
        bool sameMF = (mf == static_cast<Calculate4CMNaiveDirectSkyMatrix *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<Calculate4CMNaiveDirectSkyMatrix *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<Calculate4CMNaiveDirectSkyMatrix *>(t)->rays);
        }
        
        FATAL(e,"Calculate4CMNaiveDirectSkyMatrix with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        
        
        std::string octname = static_cast<Create4CMNaiveDirectSkyOctree *>(getDependencyRef(0))->octreeName;
        RTraceOptions options = RTraceOptions();
        options.setOption("ab",0);
        options.setOption("dc",1);
        options.setOption("dt",0);
        
        
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        rcontrib(&options, &octname[0], false, true, rays, mf, "element", true, &result);
        
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


