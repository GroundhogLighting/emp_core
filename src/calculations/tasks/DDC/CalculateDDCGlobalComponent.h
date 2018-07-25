

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
#include "CalculateDDCGlobalMatrix.h"

class CalculateDDCGlobalComponent : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart subdivition scheme for the sky
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    RTraceOptions * options; //!< The options passed to rcontrib
    int interp; //!< The interpolation scheme
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    CalculateDDCGlobalComponent(GroundhogModel * theModel, Workplane * wp, int theMF, RTraceOptions * theOptions, int interpolation)
    {
        std::string name = "DDC Global Component "+wp->getName();
        setName(&name);
        model = theModel;
        mf = theMF;
        workplane = wp;
        options = theOptions;
        interp = interpolation;
        
        // Dependency 0: matrix task
        CalculateDDCGlobalMatrix * calcMatrixTask = new CalculateDDCGlobalMatrix(model, workplane, mf, options);
        addDependency(calcMatrixTask);
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    CalculateDDCGlobalComponent(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF, RTraceOptions * theOptions, int interpolation)
    {
        std::string name = "DDC Global Illuminance";
        setName(&name);
        model = theModel;
        mf = theMF;
        options = theOptions;
        interp = interpolation;
        
        // Set the rays
        rays = theRays;
        
        // Dependency 0: matrix task
        CalculateDDCGlobalMatrix * calcMatrixTask = new CalculateDDCGlobalMatrix(model,rays,mf, options);
        addDependency(calcMatrixTask);
        
    }
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDDCGlobalComponent *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDDCGlobalComponent *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDDCGlobalComponent *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDDCGlobalComponent *>(t)->rays);
        }
        
        FATAL(e,"CalculateDDCGlobalComponent with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {        
        
        CalculateDDCGlobalMatrix * matrixTask = static_cast<CalculateDDCGlobalMatrix *>(getDependencyRef(0));
        ColorMatrix * DC = matrixTask->getResult();
        interpolatedDCTimestep(interp, model, DC, false, false, &result);
        
        
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

extern CalculateDDCGlobalComponent calcDDCGlobalComponent;
