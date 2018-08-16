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

#include "./taskmanager.h"
#include "../calculations/tasks/TriangulateWorkplane.h"
#include "../config_constants.h"
#include "./compliance.h"

#define GET_DEP_RESULTS(depClass) \
Matrix * getDependencyResults() \
{ \
    return static_cast< depClass *>(getDependencyRef(0))->getResult(); \
} \

class CBDMTask : public Task {
private:
    
    Matrix result; //!< The vector with the ASE for each sensor
    float compliance = 0; //!< Percentage of space that is over daylit

protected:
    int interp = EMP_TIME_INTERPOLATION; //!< The interpolation scheme
    EmpModel * model; //!< The model
    Workplane * workplane = nullptr; //!< The workplane to which the metric will be calculated
    std::vector<RAY> * rays = nullptr; //!< The rays to process
    Matrix * depResults = nullptr; //!< The dependency results
    double minLux = 0; //!< The minimum illuminance allowed
    double maxLux = EMP_HUGE; //!< The maximum illuminance allowed
    float minTime = (float)EMP_TINY; //!< The minimum percentage of time expected for each sensor
    float maxTime = (float)EMP_HUGE; //!< The maximum percentage of time expected for each sensor
    float early = 8; //!< Beggining of occupied hours
    float late = 18; //!< End of occupied hours
    int firstMonth = 1; //!< Beggining of occupied months
    int lastMonth = 12;  //!< End of occupied months
    std::function <float (double , double , double )> scoreCalculator = nullptr; //!< The score calculator
    
public:
    
    
    
    CBDMTask()
    {
        generatesResults = true;
    }
    
    bool isEqual(Task * t)
    {
        return false;
    }
    
    Matrix * getResult()
    {
        return &result;
    }
    
    double getCompliance()
    {
        return compliance;
    }
    
    bool solve()
    {
        
        depResults = getDependencyResults();
        
        if(depResults == nullptr)
            throw "Trying to solve a CBDM task with NULL dependency results";
        
        if(depResults->nrows() == 0 || depResults->ncols() == 0)
            throw "Trying to solve a CBDM task with Zero Size dependency results";
        
        // Calculate the score (in percentage of timestep)
        calcCBDMScore(interp, model, firstMonth,lastMonth, early,late, minLux, maxLux, depResults, &result, scoreCalculator);
        
        if(workplane == nullptr){
            compliance = calcRaysCompliance(rays,minTime,maxTime,&result);
        }else{
            // Get the workplane's triangulation
            TriangulateWorkplane aux = TriangulateWorkplane(workplane);
            TaskManager * p = getParent();
            TriangulateWorkplane * triangulate = static_cast<TriangulateWorkplane *>( p->findTask(&aux) );
            std::vector <Triangle *> triangles = triangulate->triangles;
            
            compliance = calcWorkplaneCompliance(&triangles, minTime,  maxTime, &result);
        }
        
        
        return true;
    }
    
    bool isMutex(Task * t)
    {
        return true; // Mutex with all Daylight Factor calculations
    }
    
    bool submitResults(json * j)
    {
        std::string wpName = workplane->getName();
        bulkResultsIntoJSON(getName(), wpName, &result, compliance, j);
        return true;
    }
    
    virtual Matrix * getDependencyResults() = 0;
    
};

//extern CBDMTask cbdmTask;


