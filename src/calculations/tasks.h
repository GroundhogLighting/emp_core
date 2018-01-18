
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
#include "./triangulate.h"


class OconvTask : public Task {
    
    
public:
    GroundhogModel * model;
    OconvOptions options;
    OconvTask(GroundhogModel * theModel, OconvOptions * theOptions)
    {
        options = *theOptions;
        std::string name = buildName();
        setName(&name);
        model = theModel;
        
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
        std::string octName = *getName() + ".oct";
        
        if (!oconv(octName, &options, exporter)) {
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
        
        /*
         ret += "_"+std::string(OCONV_INCLUDE_WINDOWS) + "-" + (options.getOption<bool>(std::string(OCONV_INCLUDE_WINDOWS)) ? "true" : "false");
         ret += "_"+std::string(OCONV_USE_BLACK_GEOMETRY) + "-" + (options.getOption<bool>(std::string(OCONV_USE_BLACK_GEOMETRY)) ? "true" : "false");
         ret += "_" + std::string(OCONV_INCLUDE_SKY) + "-" + (options.getOption<bool>(std::string(OCONV_INCLUDE_SKY)) ? "true" : "false");
         ret += "_" + std::string(OCONV_LIGHTS_ON) + "-" + (options.getOption<bool>(std::string(OCONV_LIGHTS_ON)) ? "true" : "false");
         ret += "_" + std::string(OCONV_SKY) + "-" + options.getOption<std::string>(std::string(OCONV_SKY)) ;
         */
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
        
        std::string octname = *(static_cast<OconvTask *>(getDependencyRef(0))->getName()) + ".oct";
        size_t nT = static_cast<TriangulateWorkplane *>(getDependencyRef(1))->triangulations.size();
        for (size_t i = 0; i < nT; i++) {
            Triangulation * t = static_cast<TriangulateWorkplane *>(getDependencyRef(1))->triangulations.at(i);
            size_t nrays = t->getNumTriangles();
            rays.push_back(std::vector<RAY>(nrays));
            rtrace_I(t, &rTraceOptions, &octname[0], ambient, &rays[i]);
        }
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
