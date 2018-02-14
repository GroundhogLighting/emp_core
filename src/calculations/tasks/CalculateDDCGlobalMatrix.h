
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

#include "./CreateDDCGlobalOctree.h"

class CalculateDDCGlobalMatrix : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart sky subdivition scheme
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    RTraceOptions * options; //!< The options passed to RContrib
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    CalculateDDCGlobalMatrix(GroundhogModel * theModel, Workplane * wp, int theMF, RTraceOptions * theOptions)
    {
        
        std::string name = "DDC Global Matrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        options = theOptions;
        
        // Dependency 0: oconv task
        CreateDDCGlobalOctree * oconvTask = new CreateDDCGlobalOctree(model);
        addDependency(oconvTask);
        
        // Dependecy 1: Triangulate workplane        
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp);
        addDependency(triangulateWorkplaneTask);
        
        
        workplane = wp;
        
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    CalculateDDCGlobalMatrix(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF, RTraceOptions * theOptions)
    {
        
        std::string name = "DDC Global Matrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        options = theOptions;
        
        // Dependency 0: oconv task
        CreateDDCGlobalOctree * oconvTask = new CreateDDCGlobalOctree(model);
        addDependency(oconvTask);
        
        // Set the rays
        rays = theRays;
        
    }
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDDCGlobalMatrix *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDDCGlobalMatrix *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDDCGlobalMatrix *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDDCGlobalMatrix *>(t)->rays);
        }
        
        FATAL(e,"CalculateDDCGlobalMatrix with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        std::string octname = static_cast<CreateDDCGlobalOctree *>(getDependencyRef(0))->octreeName;
        
        // If there is a workplane, get rays from triangulation
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        result.resize(rays->size(),nReinhartBins(mf));
        rcontrib(options, &octname[0], false, true, rays, mf, "ground_glow", false, &result);
        
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
};
