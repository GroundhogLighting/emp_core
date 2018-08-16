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

#include "./compliance.h"

float calcRaysCompliance(const std::vector<RAY> * rays, double minTime, double maxTime, const Matrix * result)
{
    float compliance = 0;
    
    // Analyse percentage of rays
    size_t nrays = rays->size();
    
    double v;
    for(size_t i = 0; i<nrays; i++){
        v = result->getElement(i,0);
        
        if(v >= minTime && v<= maxTime)
            compliance += 1.0;
    }
    
    return compliance / ((float)nrays/100.0f);
}

float calcWorkplaneCompliance(const std::vector <Triangle *> * triangles, double minTime, double maxTime, const Matrix * result)
{
    float compliance = 0;
    size_t nTriangles = triangles->size();
    float totalArea = 0;
    
    double v;
    for(size_t i = 0; i<nTriangles; i++){
        Triangle * t = triangles->at(i);
        double area = t->getArea();
        totalArea += (float)area;
        v = result->getElement(i,0);
        
        if(v >= minTime && v <= maxTime)
            compliance += (float)area;
    }
    
    return compliance / (totalArea/100.0f);
}

void bulkResultsIntoJSON(std::string taskName, std::string wpName, const Matrix * results, double compliance, json * j)
{
    
    
    size_t nrows = results->nrows();
    //size_t ncols = results->ncols();
    
    // Ensure existence of Summary and Details
    auto summary = (*j)["summary"];
    if( summary.is_null() )
        (*j)["summary"] = json::object();
    
    auto details = (*j)["details"];
    if( details.is_null() )
        (*j)["details"] = json::object();
    
    
    /* FILL SUMMARY */
    // Ensure it exists
    auto aux1 = (*j)["summary"][taskName];
    if(aux1.is_null())
        (*j)["summary"][taskName] = json::object();
    
    // Fill
    (*j)["summary"][taskName][wpName] = compliance;
    
    /* FILL DETAILS */
    // Ensure it exists
    auto aux2 = (*j)["details"][taskName];
    if(aux2.is_null())
        (*j)["details"][taskName] = json::object();
    
    // Fill
    (*j)["details"][taskName][wpName] = json::array();
    
    for(size_t row = 0; row < nrows; row++){
        (*j)["details"][taskName][wpName].push_back(results->getElement(row,0));
    }    
               
}
