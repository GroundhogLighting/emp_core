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

#include "../TriangulateWorkplane.h"
#include "./Create4CMDirectSkyOctree.h"


class Calculate4CMDirectSkyMatrix : public Task {
public:
    EmpModel * model; //!< The model
    int mf; //!< The Reinhart sky subdivition scheme
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    Calculate4CMDirectSkyMatrix(EmpModel * theModel, Workplane * wp, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        Create4CMDirectSkyOctree * oconvTask = new Create4CMDirectSkyOctree(model);
        addDependency(oconvTask);
                
        TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp);
        addDependency(triangulateWorkplaneTask);
        
        workplane = wp;
    
        
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    Calculate4CMDirectSkyMatrix(EmpModel * theModel, std::vector<RAY> * theRays, int theMF)
    {
        
        std::string name = "Create DirectSkyMatrix";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Dependency 0: oconv task
        Create4CMDirectSkyOctree * oconvTask = new Create4CMDirectSkyOctree(model);
        addDependency(oconvTask);
        
        // Set the rays
        rays = theRays;
        
    }
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<Calculate4CMDirectSkyMatrix *>(t)->model);
        bool sameMF = (mf == static_cast<Calculate4CMDirectSkyMatrix *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<Calculate4CMDirectSkyMatrix *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<Calculate4CMDirectSkyMatrix *>(t)->rays);
        }
        
        FATAL(e,"Calculate4CMDirectSkyMatrix with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        std::string octname = static_cast<Create4CMDirectSkyOctree *>(getDependencyRef(0))->octreeName;
        RTraceOptions options = RTraceOptions();
        options.setOption("ab",0);
        options.setOption("dc",1);
        options.setOption("dt",0);
        
        
        // If there is a workplane, get rays from triangulation
        if(workplane != nullptr){
            rays = &(static_cast<TriangulateWorkplane *>(getDependencyRef(1))->rays);
        }
        rcontrib(&options, &octname[0], false, true, rays, mf, "element", true, &result);
        
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

extern Calculate4CMDirectSkyMatrix calc4CMDirect;
