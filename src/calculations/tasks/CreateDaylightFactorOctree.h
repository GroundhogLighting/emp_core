
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

#include "./CreateDirectSunOctree.h"
#include "../radiance.h"
#include "../../config_constants.h"
#include "../../common/taskmanager/mutexes.h"

class CreateDaylightFactorOctree : public Task {
public:
    GroundhogModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    
    CreateDaylightFactorOctree(GroundhogModel * theModel)
    {
        
        std::string name = "Direct Sun Octree";
        setName(&name);
        model = theModel;      
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, false);
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);// --> Dependency 0
        
    }
    
    ~CreateDaylightFactorOctree()
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
        
        octreeName = "DAYLIGHT_FACTOR_" + octName;
        
        double albedo = model->getLocation()->getAlbedo();
        
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "!gensky -ang 45 40 -c -B %f -g %f\n",100.0,albedo);
        fprintf(octree, RADIANCE_SKY_COMPLEMENT);
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

extern CreateDaylightFactorOctree createDaylightFactorOctree;
