
/*****************************************************************************
 Emp
 
 Copyright (C) 2018  German Molina (germolinal@gmail.com)
 
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

#include "../../radiance.h"
#include "../../oconv_options.h"
#include "../OconvTask.h"
#include "./CreateDaylightExposureOctree.h"

class CalculateDaylightExposure : public Task {
    
public:
    EmpModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    Matrix result; //!< The vector with the DF for each sensor
    RTraceOptions * rtraceOptions; //!< The options passed to rcontrib
    std::string ambientFileName; //!< The name of the ambient file used
    
    CalculateDaylightExposure(EmpModel * theModel, RTraceOptions * theOptions, Workplane * wp)
    {
        generatesResults = false;
        
        model = theModel;
        rtraceOptions = theOptions;
        workplane = wp;
        
        // Dependency 0
        CreateDaylightExposureOctree * oconvTask = new CreateDaylightExposureOctree(model);
        addDependency(oconvTask);
        
        // Dependency 1
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp);
        addDependency(triangulateWorkplaneTask);
        
        // Set the name
        std::string name = "DaylightExposure"+wp->getName();
        ambientFileName = name + ".amb";
        setName(&name);
    }
    
    CalculateDaylightExposure(EmpModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays)
    {
        generatesResults = false;
        
        model = theModel;
        rtraceOptions = theOptions;
        rays = theRays;
        
        // Dependency 0
        CreateDaylightExposureOctree * oconvTask = new CreateDaylightExposureOctree(model);
        addDependency(oconvTask);
        
        // Set the name
        std::string name = "DaylightExposure";
        ambientFileName = name + ".amb";
        setName(&name);
    }
    
    ~CalculateDaylightExposure()
    {
        remove(&ambientFileName[0]);
    }
    
    bool isEqual(Task * t)
    {
        
        return (
                rtraceOptions->isEqual(static_cast<CalculateDaylightExposure *>(t)->rtraceOptions) &&
                workplane == static_cast<CalculateDaylightExposure *>(t)->workplane &&
                rays == static_cast<CalculateDaylightExposure *>(t)->rays
                );
    }
    
    bool solve()
    {
        
        std::string octName = (static_cast<CreateDaylightExposureOctree *>(getDependencyRef(0))->octreeName);
        
        
        if(workplane != nullptr){
            TriangulateWorkplane * dep = static_cast<TriangulateWorkplane *>(getDependencyRef(1));
            rays = &(dep->rays);
        }
        
        size_t nrays = rays->size();
        result.resize(nrays,1);
        
        ColorMatrix aux = ColorMatrix(nrays,1);
        
        rtrace_I(rtraceOptions, &octName[0], ambientFileName, rays, &aux);
        
        aux.calcIrradiance(&result);
        
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

extern CalculateDaylightExposure calcDaylightExposure;



