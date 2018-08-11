
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

#include "../../taskmanager/taskmanager.h"
#include "./DDC/CalculateDDCGlobalComponent.h"

class Calculate2PhaseGlobalIlluminance : public Task {

private:
    EmpModel * model; //!< The model
    int skyMF; //!< The Reinhart subdivition scheme for the sky
    int sunMF; //!< The Reinhart subdivition scheme for the sun
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    RTraceOptions * options; //!< The options passed to rcontrib procsses
    Matrix result; //!< The resulting matrix
    int interp; //!< The interpolation scheme

public:
    Calculate2PhaseGlobalIlluminance(EmpModel * theModel, Workplane * wp, int theSunMF, int theSkyMF, RTraceOptions * theOptions, int interpolation)
    {
        
        std::string name = "2-Phase Illuminance "+wp->getName();
        setName(&name);
        model = theModel;
        sunMF = theSunMF;
        skyMF = theSkyMF;
        options = theOptions;
        workplane = wp;
        interp = interpolation;
        
        // Dependency 0: Global illuminance
        CalculateDDCGlobalComponent * globalIlluminanceTask = new CalculateDDCGlobalComponent(model, workplane, skyMF, options, interp);
        addDependency(globalIlluminanceTask);                
        
    }
    
    
    Calculate2PhaseGlobalIlluminance(EmpModel * theModel,  std::vector<RAY> * theRays, int theSunMF, int theSkyMF, RTraceOptions * theOptions, int interpolation)
    {
        
        std::string name = "2-Phase Iluminance";
        setName(&name);
        model = theModel;
        sunMF = theSunMF;
        skyMF = theSkyMF;
        options = theOptions;
        rays = theRays;
        interp = interpolation;
        
        // Dependency 0: Global illuminance
        CalculateDDCGlobalComponent * globalIlluminanceTask = new CalculateDDCGlobalComponent(model, rays, skyMF, options, interp);
        addDependency(globalIlluminanceTask);
                
    }
    
    
    
    Matrix * getResult()
    {
        return &result;
    }
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<Calculate2PhaseGlobalIlluminance *>(t)->model);
        bool sameMF = (sunMF == static_cast<Calculate2PhaseGlobalIlluminance *>(t)->sunMF && (skyMF == static_cast<Calculate2PhaseGlobalIlluminance *>(t)->skyMF));
        
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<Calculate2PhaseGlobalIlluminance *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<Calculate2PhaseGlobalIlluminance *>(t)->rays);
        }
        
        FATAL(e,"Calculate2PhaseGlobalIlluminance with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
                        
        const ColorMatrix * depResult = &(static_cast<CalculateDDCGlobalComponent *>(getDependencyRef(0))->result);
        
        // Resize to fit
        result.resize(depResult->nrows(),depResult->ncols());
        
        // Calculate illuminance
        depResult->calcIlluminance(&result);
                
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

extern Calculate2PhaseGlobalIlluminance do2PM;
