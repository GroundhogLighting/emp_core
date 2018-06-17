
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
#include "../oconv_options.h"
#include "./CalculateDirectSunIlluminance.h"

class CalculateAnnualSunlightExposure : public Task {
    
public:
    GroundhogModel * model; //!< The model
    RTraceOptions * options //!< The options to use in RCONTRIB
    Matrix result; //!< The resulting matrix
    int mf = 6; //!< The sky subdivition
    
    CalculateAnnualSunlightExposure(GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, int theMf)
    {
        generatesResults = true;
        
        model = theModel;
        rtraceOptions = theOptions;
        workplane = wp;
        options = theOptions;
        
        // Dependency 0
        CalculateDirectSunIlluminance * illuminanceTask = new CalculateDirectSunIlluminanceCalculateDirectSunIlluminance(model, wp, mf, options)
        addDependency(illuminanceTask);
        
        // Set the name
        std::string name = "ASE";
        setName(&name);
    }
    
    CalculateAnnualSunlightExposure(GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, int theMf)
    {
        model = theModel;
        rtraceOptions = theOptions;
        rays = theRays;
        options = *theOptions;
        
        // Dependency 0
        CalculateDirectSunIlluminance * illuminanceTask = new CalculateDirectSunIlluminanceCalculateDirectSunIlluminance(model, rays, mf, options)
        addDependency(illuminanceTask);
        
        // Set the name
        std::string name = "ASE";
        setName(&name);
    }
    
    ~CalculateAnnualSunlightExposure()
    {
        
    }
    
    bool isEqual(Task * t)
    {
        
        return (
                rtraceOptions->isEqual(static_cast<CalculateAnnualSunlightExposure *>(t)->rtraceOptions) &&
                mf == static_cast<CalculateAnnualSunlightExposure *>(t)->mf
                );
    }
    
    bool solve()
    {
        
        // Get dependency's result
        CalculateDirectSunIlluminance * dep = static_cast<CalculateDirectSunIlluminance *>(getDependency(0));
        ColorMatrix * depResults = dep->results;
        
        // Get size
        size_t nrows = depResults->nrows();
        size_t ncols = depResults->ncols();
        
        // Create an auxiliar matrix, and fill it with results
        Matrix aux = Matrix(nrow,ncols);
        depResults.calculateIlluminance(&aux);
        
        // Make space for the final results results
        results.resize(nrows,1);
        
        // Calculate ASE
        double threshold = 400;
        
        
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return true; // Mutex with all Daylight Factor calculations
    }
    
    bool submitResults(json * results)
    {
        size_t nrows = result.nrows();
        size_t ncols = result.ncols();
        Matrix df = Matrix(nrows,ncols);
        result.calcIrradiance(&df);
        
        std::string name = *getName();
        
        (*results)[name] = json::array();
        for(size_t row = 0; row < nrows; row++){
            //r[row] = df.getElement(row,0);
            (*results)[name].push_back(df.getElement(row,0));
        }
        
        
        return true;
    }
    
};

extern CalculateAnnualSunlightExposure calcASE;


