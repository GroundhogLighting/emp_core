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
#include "../calculations/radiance.h"
#include "../common/geometry/triangle.h"

double calcRaysCompliance(std::vector<RAY> * rays, double minTime, double maxTime, Matrix * result)
{
    double compliance = 0;
    
    // Analyse percentage of rays
    size_t nrays = rays->size();
    
    double v;
    for(size_t i = 0; i<nrays; i++){
        v = result->getElement(i,0);
        
        if(v >= minTime && v<= maxTime)
            compliance += 1.0;
    }
    
    return compliance / ((float)nrays/100.0);
}

double calcWorkplaneCompliance(std::vector <Triangle *> * triangles, double minTime, double maxTime, Matrix * result)
{
    double compliance = 0;
    size_t nTriangles = triangles->size();
    float totalArea = 0;
    
    double v;
    for(size_t i = 0; i<nTriangles; i++){
        Triangle * t = triangles->at(i);
        double area = t->getArea();
        totalArea += area;
        v = result->getElement(i,0);
        
        if(v >= minTime && v <= maxTime)
            compliance += area;
    }
    
    return compliance / (totalArea/100.0);
}
