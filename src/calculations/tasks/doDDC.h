
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

#include "./CalculateDDCGlobalIlluminance.h"
#include "./CalculateDDCDirectSunPatchIlluminance.h"
#include "./CalculateDirectSunIlluminance.h"

class doDDC : public Task {
public:
    GroundhogModel * model; //!< The model
    int skyMF; //!< The Reinhart subdivition scheme for the sky
    int sunMF; //!< The Reinhart subdivition scheme for the sun
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    RTraceOptions * options; //!< The options passed to rcontrib procsses
    Matrix result; //!< The resulting matrix
    
    doDDC(GroundhogModel * theModel, Workplane * wp, int theSunMF, int theSkyMF, RTraceOptions * theOptions)
    {
        
        std::string name = "DDC";
        setName(&name);
        model = theModel;
        sunMF = theSunMF;
        skyMF = theSkyMF;
        options = theOptions;
        workplane = wp;
        
        // Dependency 0: Global illuminance
        CalculateDDCGlobalIlluminance * globalIlluminanceTask = new CalculateDDCGlobalIlluminance(model, workplane, skyMF, options);
        addDependency(globalIlluminanceTask);
        
        // Dependency 1: Direct sun patch illuminance
        CalculateDDCDirectSunPatchIlluminance * directSunPatchTask = new CalculateDDCDirectSunPatchIlluminance(model, workplane, skyMF, options);
        addDependency(directSunPatchTask);
        
        // Dependency 2: Direct sharp-sun illuminance
        CalculateDirectSunIlluminance * directSunTask = new CalculateDirectSunIlluminance(model, workplane, sunMF, options);
        addDependency(directSunTask);
        
    }
    
    
    doDDC(GroundhogModel * theModel,  std::vector<RAY> * theRays, int theSunMF, int theSkyMF, RTraceOptions * theOptions)
    {
        
        std::string name = "DDC";
        setName(&name);
        model = theModel;
        sunMF = theSunMF;
        skyMF = theSkyMF;
        options = theOptions;
        rays = theRays;
        
        // Dependency 0: Global illuminance
        CalculateDDCGlobalIlluminance * globalIlluminanceTask = new CalculateDDCGlobalIlluminance(model, rays, skyMF, options);
        addDependency(globalIlluminanceTask);
        
        // Dependency 1: Direct sun patch illuminance
        CalculateDDCDirectSunPatchIlluminance * directSunPatchTask = new CalculateDDCDirectSunPatchIlluminance(model, rays, skyMF, options);
        addDependency(directSunPatchTask);
        
        // Dependency 2: Direct sharp-sun illuminance
        CalculateDirectSunIlluminance * directSunTask = new CalculateDirectSunIlluminance(model, rays, sunMF, options);
        addDependency(directSunTask);
    }
    
    
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<doDDC *>(t)->model);
        bool sameMF = (sunMF == static_cast<doDDC *>(t)->sunMF && (skyMF == static_cast<doDDC *>(t)->skyMF));
        
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<doDDC *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<doDDC *>(t)->rays);
        }
        
        FATAL(e,"doDDC with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        size_t nSensors = rays->size();
        size_t nTimesteps = model->getLocation()->getWeatherSize();
        
        int k=0;
        ColorMatrix * global = &(static_cast<CalculateDDCGlobalIlluminance *>(getDependencyRef(k++))->result);
        ColorMatrix * directSunPatch = &(static_cast<CalculateDDCDirectSunPatchIlluminance *>(getDependencyRef(k++))->result);
        ColorMatrix * directSun = &(static_cast<CalculateDirectSunIlluminance *>(getDependencyRef(k++))->result);
        
        // Resize to fit
        result.resize(nSensors,nTimesteps);
        
        // Calculate
        Matrix * globalRed =   global->redChannel();
        Matrix * globalGreen = global->greenChannel();
        Matrix * globalBlue =  global->blueChannel();
        
        Matrix * directSunPatchRed =   directSunPatch->redChannel();
        Matrix * directSunPatchGreen = directSunPatch->greenChannel();
        Matrix * directSunPatchBlue =  directSunPatch->blueChannel();
        
        Matrix * directSunRed =   directSun->redChannel();
        Matrix * directSunGreen = directSun->greenChannel();
        Matrix * directSunBlue =  directSun->blueChannel();
        
        double r,g,b;
        for(size_t col=0; col < nTimesteps; col++){
            for(size_t row=0; row < nSensors; row++){                
                r = globalRed->  getElement(row,col) - directSunPatchRed->  getElement(row,col) + directSunRed->  getElement(row,col);
                g = globalGreen->getElement(row,col) - directSunPatchGreen->getElement(row,col) + directSunGreen->getElement(row,col);
                b = globalBlue-> getElement(row,col) - directSunPatchBlue-> getElement(row,col) + directSunBlue-> getElement(row,col);
                result.setElement(row,col,47.5*r + 119.95*g + 11.60*b);
            }
        }
        
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

extern doDDC doTheDDC;
