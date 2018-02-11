
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
#include "radiance.h"

class CalculateDDCDirectSunPatchIlluminance : public Task {
public:
    GroundhogModel * model; //!< The model
    int skyMF; //!< The Reinhart sky subdivition scheme for the sky
    int sunMF; //!< The Reinhart sky subdivition scheme for the sun
    Workplane * workplane = nullptr; //!< The workplane to which the matrix will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix result; //!< The resulting matrix
    Weather * weather; //!< Weather
    
    //* Process a Workplane
    /*!
     @author German Molina
     */
    CalculateDDCDirectSunPatchIlluminance(GroundhogModel * theModel, Workplane * wp, int theMF)
    {
        std::string name = "Calculate DDCDirectSkyIlluminance";
        setName(&name);
        model = theModel;
        mf = theMF;
        workplane = wp;
        
        // Dependency 0: matrix task
        CalculateDDCDirectSkyMatrix * calcMatrixTask = new CalculateDDCDirectSkyMatrix(model,workplane,mf);
        addDependency(calcMatrixTask);                
    }
    
    //! Process a vector of rays
    /*!
     @author German Molina
     */
    CalculateDDCDirectSunPatchIlluminance(GroundhogModel * theModel, std::vector<RAY> * theRays, int theMF)
    {
        std::string name = "Calculate DDCDirectSkyIlluminance";
        setName(&name);
        model = theModel;
        mf = theMF;
        
        // Set the rays
        rays = theRays;
        
        // Dependency 0: matrix task
        CalculateDDCDirectSkyMatrix * calcMatrixTask = new CalculateDDCDirectSkyMatrix(model,rays,mf);
        addDependency(calcMatrixTask);
        
    }
    
    
    bool isEqual(Task * t)
    {
        bool sameModel = (model == static_cast<CalculateDDCDirectSunPatchIlluminance *>(t)->model);
        bool sameMF = (mf == static_cast<CalculateDDCDirectSunPatchIlluminance *>(t)->mf);
        
        if(workplane != nullptr){
            return (sameModel && sameMF && workplane == static_cast<CalculateDDCDirectSunPatchIlluminance *>(t)->workplane);
        }
        if(rays != nullptr){
            return (sameModel && sameMF && rays == static_cast<CalculateDDCDirectSunPatchIlluminance *>(t)->rays);
        }
        
        FATAL(e,"CalculateDDCDirectSunPatchIlluminance with null Workplane and Rays");
        return true;
    }
    
    bool solve()
    {
        if (!weather->hasData()){
            FATAL(m,"No Weather Data when CalculateDDCDirectSunPatchIlluminance");
            return false;
        }
        
        ColorMatrix * DC = static_cast<CalculateDDCDirectSkyMatrix *>(getDependency(0));
        size_t nBins = DC->ncols();
        ColorMatrix skyVector = ColorMatrix(nBins,1);
        
        // Make space for all results
        size_t nSensors = DC->nrows();
        size_t nSamples = weather->data.size();
        result.resize(nSensors,nSamples);
        
        Matrix * red = result.redChannel();
        Matrix * green = result.greenChannel();
        Matrix * blue = result.blueChannel();
        
        Location * location = model->getLocation();
        float albedo = location->getAlbedo();
        float latitude = location->getLatitude();
        float longitude = location-> getLongitude();
        float meridian = location->getTimeZone()*(-15.0);
        double rotation = model -> getNorthCorrection();
        
        for(size_t timestep = 0 ; timestep < nSamples; timestep++ ){
            HourlyData d = weather->data[i];
            
            if(d.diffuse_horizontal < 1e-3){
                // it is night... trivial solution
                for(size_t i = 0; i < nSensors; i++){
                    red[i].at(timestep) = 0.0;
                    green[i].at(timestep) = 0.0;
                    blue[i].at(timestep) = 0.0;
                }
            }else{
                
                // Is day... calculate
                genPerezSkyVector(d.month, d.day, d.hour, d.direct_normal, d.diffuse_horizontal, albedo, latitude, longitude, meridian, mf, true, false, rotation, &skyVector);
                
                // Multiply
                DC->multiplyToColumn(&skyVector,timestep,&result);
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
