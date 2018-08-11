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

#include "../../../taskmanager/mutexes.h"
#include "../../../os_definitions.h"
#include "../OconvTask.h"
#include "../../solar.h"

class CreateDirectSunOctree : public Task {
public:
    EmpModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    int mf = 1; //!< The reinhart subdivition sheme
    
    CreateDirectSunOctree(EmpModel * theModel, int theMf)
    {
        
        std::string name = "Direct Sun Octree";
        setName(&name);
        model = theModel;
        mf = theMf;
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);        
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);// --> Dependency 0
        
    }
    
    ~CreateDirectSunOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return model == static_cast<CreateDirectSunOctree *>(t)->model;
    }
    
    bool solve()
    {
        tbb::mutex::scoped_lock lock(oconvMutex);
        std::string octName = (static_cast<OconvTask *>(getDependencyRef(0))->octreeName);
        
        octreeName = "DIRECT_SUN_" + octName;
        fixString(&octreeName);
        
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "void light solar 0 0 3 1e6 1e6 1e6\n");
        size_t nbins = nReinhartBins(mf);
        Vector3D dir = Vector3D(0,0,0);
        
        const double latitude = model->getLocation()->getLatitude();
        
        for(size_t bin = 1; bin <= nbins; bin++){
            dir = reinhartCenterDir(bin,mf);
            
            if(!isInSolarTrajectory(dir,latitude, mf))
                continue;
            
            fprintf(octree, "solar source sun 0 0 4 %f %f %f 0.533\n", dir.getX(), dir.getY(), dir.getZ());
            
        }
        
        PCLOSE(octree);
        
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

extern CreateDirectSunOctree createDirectSunOctree;
