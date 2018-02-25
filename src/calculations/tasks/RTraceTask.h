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

#include "../radiance.h"
#include "../oconv_options.h"
#include "./AddSkyToOctree.h"

class RTraceTask : public Task {
    
public:
    GroundhogModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    RTraceOptions * rtraceOptions; //!< The options passed to rcontrib
    OconvOptions * oconvOptions; //!< The OconvOptions
    std::string ambientFileName; //!< The name of the ambient file used
    std::string sky; //!< The sky to add to the octree
    
    RTraceTask(GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, OconvOptions * theOconvOptions, std::string theSky)
    {
        model = theModel;
        rtraceOptions = theOptions;
        oconvOptions = theOconvOptions;
        workplane = wp;
        sky = theSky;
        
        // Dependency 0
        AddSkyToOctree * oconvTask = new AddSkyToOctree(model, oconvOptions, sky);
        addDependency(oconvTask);
        
        // Dependency 1
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp);
        addDependency(triangulateWorkplaneTask);
        
        // Set the name
        std::string name = "Rtrace_"+ oconvTask->octreeName;
        ambientFileName = name + ".amb";
        setName(&name);
    }
    
    RTraceTask(GroundhogModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays , OconvOptions * theOconvOptions, std::string theSky)
    {
        model = theModel;
        rtraceOptions = theOptions;
        oconvOptions = theOconvOptions;
        rays = theRays;
        sky = theSky;
        
        // Dependency 0
        AddSkyToOctree * oconvTask = new AddSkyToOctree(model, oconvOptions, sky);
        addDependency(oconvTask);
        
        // Set the name
        std::string name = "Rtrace_"+ oconvTask->octreeName;
        ambientFileName = name + ".amb";
        setName(&name);
    }
    
    ~RTraceTask()
    {
        remove(&ambientFileName[0]);
    }
    
    bool isEqual(Task * t)
    {
        
        return (
                rtraceOptions->isEqual(static_cast<RTraceTask *>(t)->rtraceOptions) &&
                oconvOptions->isEqual(static_cast<RTraceTask *>(t)->oconvOptions) &&
                workplane == static_cast<RTraceTask *>(t)->workplane &&
                rays == static_cast<RTraceTask *>(t)->rays &&
                sky == static_cast<RTraceTask *>(t)->sky
                );
    }
    
    bool solve()
    {
        
        std::string octname = static_cast<AddSkyToOctree *>(getDependencyRef(0))->octreeName;
        ambientFileName = octname + ".amb";
        result.resize(rays->size(),1);
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        rtrace_I(rtraceOptions, &octname[0], ambientFileName, rays, &result);        
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return (
                oconvOptions->isEqual(static_cast<RTraceTask *>(t)->oconvOptions) &&
                sky == (static_cast<RTraceTask *>(t)->sky)
                );
    }
    
    bool submitResults(json * results)
    {
        return true;
    }
    
};

extern RTraceTask rTraceTask;

