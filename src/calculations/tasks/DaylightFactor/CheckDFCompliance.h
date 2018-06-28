

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

#include "./CalculateDaylightFactor.h"

class CheckDFCompliance : public Task {
    
public:
    
    double compliance = 0; //!< The percentage of space that satisfies the asked DF
    double minPercent; //!< The minimum DF asked
    double maxPercent; //!< The maximum DF asked
    
    
    CheckDFCompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, double min, double max)
    {
        generatesResults = true;
                                
        minPercent = min;
        maxPercent = max;
        
        // Dependency
        CalculateDaylightFactor * dep = new CalculateDaylightFactor(theModel, theOptions, wp);
        addDependency(dep);
        
        // Set the name
        setName(&name);
    }
    
    CheckDFCompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays, double min, double max)
    {
        generatesResults = true;
        
        minPercent = min;
        maxPercent = max;
        
        // Dependency
        CalculateDaylightFactor * dep = new CalculateDaylightFactor(theModel, theOptions, theRays);
        addDependency(dep);
        
        // Set the name
        setName(&name);
    }
    
    ~CheckDFCompliance()
    {
        
    }
    
    bool isEqual(Task * t)
    {
        return false; // Always calculate this task
    }
    
    bool solve()
    {
        
        
        CalculateDaylightFactor * dep = static_cast<CalculateDaylightFactor *>(getDependencyRef(0));
        Matrix * depResults = &(dep->result);
        
        if(dep->workplane == nullptr){
            // Analyse percentage of rays
            
            size_t nrays = dep->rays->size();
            
            double v;
            for(size_t i = 0; i<nrays; i++){
                v = depResults->getElement(i,0);
                
                if(v >= minPercent && v <= maxPercent)
                    compliance += 1.0;
            }
            compliance /= ((float)nrays/100.0);
        }else{
            std::vector <Triangle *> * triangles = &(static_cast<TriangulateWorkplane *>(dep->getDependencyRef(1))->triangles);
            
            size_t nTriangles = triangles->size();
            float totalArea = 0;
            
            double v;
            for(size_t i = 0; i<nTriangles; i++){
                Triangle * t = triangles->at(i);
                double area = t->getArea();
                totalArea += area;
                
                v = depResults->getElement(i,0);
                
                if(v >= minPercent && v <= maxPercent)
                    compliance += area;
            }
            compliance /= (totalArea/100.0);
        }
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return true; // Mutex with all Daylight Factor calculations
    }
    
    bool submitResults(json * results)
    {
        CalculateDaylightFactor * dep = static_cast<CalculateDaylightFactor *>(getDependencyRef(0));
        Matrix * depResults = &(dep->result);
        
        size_t nrows = depResults->nrows();
        
        std::string name = *getName();
        
        (*results)[name] = json::array();
        for(size_t row = 0; row < nrows; row++){
            (*results)[name].push_back(depResults->getElement(row,0));
        }
        
        
        return true;
    }
    
};

extern CheckDFCompliance checkDF;

