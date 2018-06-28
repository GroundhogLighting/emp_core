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

//#include "../radiance.h"
//#include "../oconv_options.h"
//#include "./CalculateDirectSunComponent.h"
#include "../config_constants.h"
#include "./taskmanager/compliance.h"

double defaultScoreCalculator(double v, double minLux, double maxLux)
{
    return (v >= minLux ? 1 : 0);
}

class CBDMTask : public Task {
    
public:
    
    int interp = EMP_TIME_INTERPOLATION; //!< The interpolation scheme
    Matrix result; //!< The vector with the ASE for each sensor
    float compliance = 0; //!< Percentage of space that is over daylit
    GroundhogModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the metric will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    ColorMatrix * depResults = nullptr; //!< The dependency results
    double minLux = 300; //!< The maximum illuminance allowed
    float minTime = EMP_TINY; //!< The minimum percentage of time expected for each sensor
    float maxTime = EMP_HUGE; //!< The maximum percentage of time expected for each sensor
    float early = 8; //!< Beggining of occupied hours
    float late = 18; //!< End of occupied hours
    int firstMonth = 1; //!< Beggining of occupied months
    int lastMonth = 12;  //!< End of occupied months
    std::function <double (double , double , double )> scoreCalculator = defaultScoreCalculator; //!< The score calculator
    
   
    
    bool isEqual(Task * t)
    {
        return false;
    }
    
    
    bool solve()
    {
        
                
        // Calculate the score (in percentage of timestep)
        calcCBDMScore(interp, model, firstMonth,lastMonth, early,late, minLux, EMP_HUGE, depResults, &result, scoreCalculator);
        
        if(workplane == nullptr){
            compliance = calcRaysCompliance(rays,minTime,maxTime,&result);
        }else{
            // Get the workplane's triangulation
            TriangulateWorkplane aux = TriangulateWorkplane(workplane);
            TaskManager * p = getParent();
            TriangulateWorkplane * triangulate = static_cast<TriangulateWorkplane *>(p->findTask(&aux));
            std::vector <Triangle *> * triangles = &(triangulate->triangles);
            
            compliance = calcWorkplaneCompliance(triangles, minTime,  maxTime, &result);
        }
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return true; // Mutex with all Daylight Factor calculations
    }
    
    bool submitResults(json * results)
    {
        return true;
    }
    
};

extern CBDMTask cbdmTask;


