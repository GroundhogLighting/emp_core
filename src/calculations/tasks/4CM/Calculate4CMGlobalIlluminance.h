

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

#include "../DDC/CalculateDDCGlobalComponent.h"
#include "../DDC/CalculateDDCDirectSunPatchComponent.h"
#include "../DirectSun/CalculateDirectSunComponent.h"

class Calculate4CMGlobalIlluminance : public Task {
public:
    GroundhogModel * model; //!< The model
    int skyMF; //!< The Reinhart subdivition scheme for the sky
    int sunMF; //!< The Reinhart subdivition scheme for the sun
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    RTraceOptions * options; //!< The options passed to rcontrib procsses
    ColorMatrix result; //!< The resulting matrix
    int interp; //!< The interpolation scheme
    
    Calculate4CMGlobalIlluminance(GroundhogModel * theModel, Workplane * wp, int theSunMF, int theSkyMF, RTraceOptions * theOptions, int interpolation)
    {
        
        std::string name = "doDC4";
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
        
        // Dependency 1: Direct sun patch illuminance
        CalculateDDCDirectSunPatchComponent * directSunPatchTask = new CalculateDDCDirectSunPatchComponent(model, workplane, skyMF, options,interp);
        addDependency(directSunPatchTask);
        
        // Dependency 2: Direct sharp-sun illuminance
        CalculateDirectSunComponent * directSunTask = new CalculateDirectSunComponent(model, workplane, sunMF, options,interp);
        addDependency(directSunTask);
        
    }
    
    
    Calculate4CMGlobalIlluminance(GroundhogModel * theModel,  std::vector<RAY> * theRays, int theSunMF, int theSkyMF, RTraceOptions * theOptions, int interpolation)
    {
        
        std::string name = "Calculate4CMGlobalIlluminance";
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
        
        // Dependency 1: Direct sun patch illuminance
        CalculateDDCDirectSunPatchComponent * directSunPatchTask = new CalculateDDCDirectSunPatchComponent(model, rays, skyMF, options,interp);
        addDependency(directSunPatchTask);
        
        // Dependency 2: Direct sharp-sun illuminance
        CalculateDirectSunComponent * directSunTask = new CalculateDirectSunComponent(model, rays, sunMF, options,interp);
        addDependency(directSunTask);
    }
    
    
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<Calculate4CMGlobalIlluminance *>(t)->model);
        bool sameMF = (sunMF == static_cast<Calculate4CMGlobalIlluminance *>(t)->sunMF && (skyMF == static_cast<Calculate4CMGlobalIlluminance *>(t)->skyMF));
        
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<Calculate4CMGlobalIlluminance *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<Calculate4CMGlobalIlluminance *>(t)->rays);
        }
        
        FATAL(e,"CalculateDDCGlobalIlluminance with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        size_t nSensors = rays->size();
        size_t nTimesteps = model->getLocation()->getWeatherSize();
        
        int k=0;
        ColorMatrix * global = &(static_cast<CalculateDDCGlobalComponent *>(getDependencyRef(k++))->result);
        ColorMatrix * directSunPatch = &(static_cast<CalculateDDCDirectSunPatchComponent *>(getDependencyRef(k++))->result);
        ColorMatrix * directSun = &(static_cast<CalculateDirectSunComponent *>(getDependencyRef(k++))->result);
        
        // Resize to fit
        result.resize(nSensors,nTimesteps);
        
        // Calculate
        const Matrix * globalRed = global->redChannel();
        const Matrix * globalGreen = global->greenChannel();
        const Matrix * globalBlue = global->blueChannel();
        
        const Matrix * directSunPatchRed = directSunPatch->redChannel();
        const Matrix * directSunPatchGreen = directSunPatch->greenChannel();
        const Matrix * directSunPatchBlue = directSunPatch->blueChannel();
        
        const Matrix * directSunRed = directSun->redChannel();
        const Matrix * directSunGreen = directSun->greenChannel();
        const Matrix * directSunBlue = directSun->blueChannel();
        
        Matrix * red = result.r();
        Matrix * green = result.g();
        Matrix * blue = result.b();
        
        for(size_t col=0; col < nTimesteps; col++){
            for(size_t row=0; row < nSensors; row++){
                double r = globalRed->getElement(row,col) - directSunPatchRed->getElement(row,col) + directSunRed->getElement(row,col);
                double g = globalGreen->getElement(row,col) - directSunPatchGreen->getElement(row,col) + directSunGreen->getElement(row,col);
                double b = globalBlue->getElement(row,col) - directSunPatchBlue->getElement(row,col) + directSunBlue->getElement(row,col);
                
                red->setElement(row,col,r);
                green->setElement(row,col,g);
                blue->setElement(row,col,b);
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

extern Calculate4CMGlobalIlluminance doThe4CM;
