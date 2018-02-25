
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
#include "OconvTask.h"
#include "./CreateDaylightFactorOctree.h"

class CalculateDaylightFactor : public Task {
    
public:
    GroundhogModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    RTraceOptions * rtraceOptions; //!< The options passed to rcontrib
    std::string ambientFileName; //!< The name of the ambient file used
    
    CalculateDaylightFactor(GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp)
    {
        model = theModel;
        rtraceOptions = theOptions;
        workplane = wp;
        
        // Dependency 0
        CreateDaylightFactorOctree * oconvTask = new CreateDaylightFactorOctree(model);
        addDependency(oconvTask);
        
        // Dependency 1
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp);
        addDependency(triangulateWorkplaneTask);
        
        // Set the name
        std::string name = "DaylightFactor";
        ambientFileName = name + ".amb";
        setName(&name);
    }
    
    CalculateDaylightFactor(GroundhogModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays)
    {
        model = theModel;
        rtraceOptions = theOptions;
        rays = theRays;
        
        // Dependency 0
        CreateDaylightFactorOctree * oconvTask = new CreateDaylightFactorOctree(model);
        addDependency(oconvTask);
        
        // Set the name
        std::string name = "DaylightFactor";
        ambientFileName = name + ".amb";
        setName(&name);
    }
    
    ~CalculateDaylightFactor()
    {
        remove(&ambientFileName[0]);
    }
    
    bool isEqual(Task * t)
    {
        
        return (
                rtraceOptions->isEqual(static_cast<CalculateDaylightFactor *>(t)->rtraceOptions) &&
                workplane == static_cast<CalculateDaylightFactor *>(t)->workplane &&
                rays == static_cast<CalculateDaylightFactor *>(t)->rays
                );
    }
    
    bool solve()
    {
                        
        std::string octName = (static_cast<CreateDaylightFactorOctree *>(getDependencyRef(0))->octreeName);
     
        result.resize(rays->size(),1);
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        rtrace_I(rtraceOptions, &octName[0], ambientFileName, rays, &result);
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return true; // Mutex with all Daylight Factor calculations
    }
    
    bool submitResults(json * results)
    {
        return true;
    }
    
};

extern CalculateDaylightFactor calcDF;

