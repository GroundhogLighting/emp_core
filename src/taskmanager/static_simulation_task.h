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


#include "../config_constants.h"
#include "./compliance.h"


class StaticSimulationTask : public Task {
    
public:
    
    bool generatesResults = true; //!< Does generate results
    float compliance = 0; //!< Percentage of space that is over daylit
    EmpModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the metric will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    Matrix * depResults = nullptr; //!< The dependency results
    double minLux = 0; //!< The minimum illuminance allowed
    double maxLux = EMP_HUGE; //!< The maximum illuminance allowed
    
    bool isEqual(Task * t)
    {
        return false;
    }
    
    
    bool solve()
    {
        depResults = getDependencyResults();
        
        if(depResults == nullptr)
            throw "Trying to solve a StaticSimulatonTask task with NULL dependency results";
        
        if(workplane == nullptr){
            compliance = calcRaysCompliance(rays,minLux,maxLux,depResults);
        }else{
            // Get the workplane's triangulation
            TriangulateWorkplane aux = TriangulateWorkplane(workplane);
            TaskManager * p = getParent();
            TriangulateWorkplane * triangulate = static_cast<TriangulateWorkplane *>(p->findTask(&aux));
            std::vector <Triangle *> * triangles = &(triangulate->triangles);
            
            compliance = calcWorkplaneCompliance(triangles, minLux,maxLux,depResults);
        }
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return false; // Mutex with all Daylight Factor calculations
    }
    
    bool submitResults(json * results)
    {
        return true;
    }
    
    virtual Matrix * getDependencyResults() = 0;    
    
};

//extern StaticSimulationTask staticSimulationTask;


