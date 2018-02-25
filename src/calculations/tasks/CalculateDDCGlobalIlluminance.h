

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
#include "../radiance.h"
#include "CalculateDDCGlobalMatrix.h"

class CalculateDDCGlobalIlluminance : public Task {
public:
    GroundhogModel * model; //!< The model
    int mf; //!< The Reinhart subdivition scheme for the sky
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    RTraceOptions * options; //!< The options passed to rcontrib
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    CalculateDDCGlobalIlluminance(GroundhogModel * theModel, Workplane * wp, int theMF, RTraceOptions * theOptions)
    {
        std::string name = "DDC Global Illuminance";
        setName(&name);
        model = theModel;
        mf = theMF;
        workplane = wp;
        options = theOptions;
        
        // Dependency 0: matrix task
        CalculateDDCGlobalMatrix * calcMatrixTask = new CalculateDDCGlobalMatrix(model, workplane, mf, options);
        addDependency(calcMatrixTask);
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    CalculateDDCGlobalIlluminance(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF, RTraceOptions * theOptions)
    {
        std::string name = "DDC Global Illuminance";
        setName(&name);
        model = theModel;
        mf = theMF;
        options = theOptions;
        
        // Set the rays
        rays = theRays;
        
        // Dependency 0: matrix task
        CalculateDDCGlobalMatrix * calcMatrixTask = new CalculateDDCGlobalMatrix(model,rays,mf, options);
        addDependency(calcMatrixTask);
        
    }
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDDCGlobalIlluminance *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDDCGlobalIlluminance *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDDCGlobalIlluminance *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDDCGlobalIlluminance *>(t)->rays);
        }
        
        FATAL(e,"CalculateDDCGlobalIlluminance with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {        
        
        CalculateDDCGlobalMatrix * matrixTask = static_cast<CalculateDDCGlobalMatrix *>(getDependencyRef(0));
        ColorMatrix * DC = &(matrixTask->result);
        size_t nBins = DC->ncols();
        ColorMatrix skyVector = ColorMatrix(nBins,1);
        
        Location * location = model->getLocation();
        float albedo = location->getAlbedo();
        float latitude = location->getLatitude();
        float longitude = location-> getLongitude();
        float meridian = location->getTimeZone()*(-15.0);
        double rotation = model -> getNorthCorrection();
        
        // Make space for all results
        size_t nSensors = DC->nrows();
        size_t nSamples = location->getWeatherSize();
        if (nSamples == 0){
            FATAL(m,"No Weather Data when CalculateDDCGlobalIlluminance");
            return false;
        }
        result.resize(nSensors,nSamples);
                
        
        for(size_t timestep = 0 ; timestep < nSamples; timestep++ ){
            HourlyData * d = location->getHourlyData(timestep);
            
            if(d->diffuse_horizontal > 1e-4){
                // Is day... calculate
                genPerezSkyVector(d->month, d->day, d->hour, d->direct_normal, d->diffuse_horizontal, albedo, latitude, longitude, meridian, mf, false, false, rotation, &skyVector);
                
                // Multiply
                DC->multiplyToColumn(&skyVector,timestep,&result);
            } // Else nothing... the matrix already comes with Zeroes
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

extern CalculateDDCGlobalIlluminance calcDDCGlobalIlluminance;
