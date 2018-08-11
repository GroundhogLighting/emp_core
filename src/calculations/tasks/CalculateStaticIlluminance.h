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

#include "../radiance.h"
#include "../oconv_options.h"
#include "./AddSkyToOctree.h"

class CalculateStaticIlluminance : public Task {
    
public:
    EmpModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    Matrix result; //!< The resulting matrix
    RTraceOptions * rtraceOptions; //!< The options passed to rcontrib
    OconvOptions * oconvOptions; //!< The OconvOptions
    std::string ambientFileName; //!< The name of the ambient file used
    std::string sky; //!< The sky to add to the octree
    
    CalculateStaticIlluminance(EmpModel * theModel, RTraceOptions * theOptions, Workplane * wp, OconvOptions * theOconvOptions, std::string theSky)
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
    
    CalculateStaticIlluminance(EmpModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays , OconvOptions * theOconvOptions, std::string theSky)
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
    
    ~CalculateStaticIlluminance()
    {
        remove(&ambientFileName[0]);
    }
    
    bool isEqual(Task * t)
    {
        
        return (
                rtraceOptions->isEqual(static_cast<CalculateStaticIlluminance *>(t)->rtraceOptions) &&
                oconvOptions->isEqual(static_cast<CalculateStaticIlluminance *>(t)->oconvOptions) &&
                workplane == static_cast<CalculateStaticIlluminance *>(t)->workplane &&
                rays == static_cast<CalculateStaticIlluminance *>(t)->rays &&
                sky == static_cast<CalculateStaticIlluminance *>(t)->sky
                );
    }
    
    bool solve()
    {
        
        std::string octname = static_cast<AddSkyToOctree *>(getDependencyRef(0))->octreeName;
        ambientFileName = octname + ".amb";
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        
        size_t nrays=rays->size();
        
        result.resize(nrays,1);
        
        ColorMatrix aux = ColorMatrix(nrays,1);
        
        rtrace_I(rtraceOptions, &octname[0], ambientFileName, rays, &aux);
        
        aux.calcIrradiance(&result);
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return (
                oconvOptions->isEqual(static_cast<CalculateStaticIlluminance *>(t)->oconvOptions) &&
                sky == (static_cast<CalculateStaticIlluminance *>(t)->sky)
                );
    }
    
    bool submitResults(json * results)
    {
        return true;
    }
    
};

extern CalculateStaticIlluminance calcStaticIlluminance;

