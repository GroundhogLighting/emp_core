

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

#include "./CalculateStaticIlluminance.h"
#include "taskmanager/static_simulation_task.h"

class CheckLUXCompliance : public StaticSimulationTask {
    
public:
    
    
    CheckLUXCompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, OconvOptions * theOconvOptions, std::string sky, double min, double max)
    {
        model = theModel;
        workplane = wp;
        minLux = min;
        maxLux = max;
        
        // Dependency
        CalculateStaticIlluminance * dep = new CalculateStaticIlluminance(theModel, theOptions, wp, theOconvOptions, sky);
        addDependency(dep);
        
        depResults = &(dep->result); 
        
        // Set the name
        setName(&name);
    }
    
    CheckLUXCompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays, OconvOptions * theOconvOptions, std::string sky, double min, double max)
    {
        model = theModel;
        rays = theRays;
        minLux = min;
        maxLux = max;
        
        // Dependency
        CalculateStaticIlluminance * dep = new CalculateStaticIlluminance(theModel, theOptions, theRays, theOconvOptions, sky);
        addDependency(dep);
        
        depResults = &(dep->result);
        
        // Set the name
        setName(&name);
    }
    
    
};

extern CheckLUXCompliance checkLux;

