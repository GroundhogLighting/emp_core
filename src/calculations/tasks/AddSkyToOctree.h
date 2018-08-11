
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

#include "../../os_definitions.h"
//#include "../radiance.h"
#include "../../config_constants.h"
#include "../../common/utilities/stringutils.h"
#include "./OconvTask.h"
#include "../../taskmanager/mutexes.h"

class AddSkyToOctree : public Task {

public:
    EmpModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    OconvOptions options;//!< The options passed to the octree
    std::string sky; //!< The sky to add to the octree


    AddSkyToOctree(EmpModel * theModel, OconvOptions * theOptions, std::string theSky)
    {
        
        model = theModel;        
        options = *theOptions;
        sky = theSky;
        std::string name = "Add sky "+theSky;
        setName(&name);
        
        // Dependency 0: Add the oconv task
        OconvTask * oconvTask = new OconvTask(model,&options);
        addDependency(oconvTask);
        
        // Set octree name
        fixString(&theSky[0],theSky.size());
        octreeName = theSky+"_" + oconvTask->octreeName;
        
    }
    
    ~AddSkyToOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<AddSkyToOctree *>(t)->model &&
                sky == static_cast<AddSkyToOctree *>(t)->sky
                );
    }
    
    bool solve()
    {
        
        tbb::mutex::scoped_lock lock(oconvMutex);
        std::string octName = (static_cast<OconvTask *>(getDependencyRef(0))->octreeName);
                
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "!%s\n",&sky[0]);
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

extern AddSkyToOctree addSkyToOctree;
