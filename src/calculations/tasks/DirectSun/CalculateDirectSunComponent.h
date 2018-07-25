

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
#include "./CalculateDirectSunMatrix.h"

class CalculateDirectSunComponent : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart sky subdivition scheme for the sun
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    RTraceOptions * options; //!< Options passed to rcontrib
    int interp; //!< The interpolation scheme
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    CalculateDirectSunComponent(GroundhogModel * theModel, Workplane * wp, int theMF, RTraceOptions * theOptions, int interpolation)
    {
        std::string name = "Direct Sun Component "+ wp->getName();
        setName(&name);
        model = theModel;
        mf = theMF;
        workplane = wp;
        options = theOptions;
        interp = interpolation;
        
        // Dependency 0: matrix task
        CalculateDirectSunMatrix * calcMatrixTask = new CalculateDirectSunMatrix(model,workplane,mf,options);
        addDependency(calcMatrixTask);
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    CalculateDirectSunComponent(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF, RTraceOptions * theOptions, int interpolation)
    {
        std::string name = "DDC Direct Sun Illuminance";
        setName(&name);
        model = theModel;
        mf = theMF;
        options = theOptions;
        interp = interpolation;
        
        // Set the rays
        rays = theRays;
        
        // Dependency 0: matrix task
        CalculateDirectSunMatrix * calcMatrixTask = new CalculateDirectSunMatrix(model,rays,mf,options);
        addDependency(calcMatrixTask);
        
    }
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDirectSunComponent *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDirectSunComponent *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDirectSunComponent *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDirectSunComponent *>(t)->rays);
        }
        
        FATAL(e,"CalculateDirectSunComponent with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        ColorMatrix * DC = &(static_cast<CalculateDirectSunMatrix *>(getDependencyRef(0))->result);
        interpolatedDCTimestep(interp, model, DC, true, true, &result);
        
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

extern CalculateDirectSunComponent calcDirectSunComponent;
