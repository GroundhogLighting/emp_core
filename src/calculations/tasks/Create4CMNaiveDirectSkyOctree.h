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

#include "../oconv_options.h"
#include "../../common/taskmanager/mutexes.h"

class Create4CMNaiveDirectSkyOctree : public Task {
public:
    GroundhogModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    int mf = 1; //!< The reinhart subdivition sheme
    
    Create4CMNaiveDirectSkyOctree(GroundhogModel * theModel, const int theMf)
    {
        
        std::string name = "Create CreateNaiveDirectSkyOctree";
        setName(&name);
        model = theModel;
        mf = theMf;
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);        
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        // --> Dependency 0
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);
        
    }
    
    ~Create4CMNaiveDirectSkyOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return model == static_cast<Create4CMNaiveDirectSkyOctree *>(t)->model;
    }
    
    bool solve()
    {
        tbb::mutex::scoped_lock lock(oconvMutex);
        std::string octName = *(static_cast<OconvTask *>(getDependencyRef(0))->getName()) + ".oct";
        octreeName = "NAIVE_DIRECT_SKY_" + octName;
        //remove(&octreeName[0]);
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "void light solar 0 0 3 1 1 1\n");
        size_t nbins = nReinhartBins(mf);
        Vector3D dir = Vector3D(0,0,0);
        for(size_t bin = 1; bin <= nbins; bin++){
            dir = reinhartCenterDir(bin,mf);
            fprintf(octree, "solar source sun 0 0 4 %f %f %f 11.000\n", dir.getX(), dir.getY(), dir.getZ());
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

extern Create4CMNaiveDirectSkyOctree create4CMNaiveDirectSkyOctree;
