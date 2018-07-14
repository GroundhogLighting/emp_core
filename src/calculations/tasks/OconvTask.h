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


#include "../../common/utilities/stringutils.h"


#include "../../taskmanager/task.h"
#include "../oconv_options.h"

#include "../../writers/rad/radexporter.h"
#include "../reinhart.h"
#include "./TriangulateWorkplane.h"
#include "../../taskmanager/mutexes.h"

class OconvTask : public Task {
    
public:
    
    GroundhogModel * model; //!< The model to Oconv
    OconvOptions options; //!< The Options passed to Oconv
    std::string octreeName; //!< The name of the created octree
    
    
    
    OconvTask(GroundhogModel * theModel, OconvOptions * theOptions)
    {
        options = *theOptions;
        std::string name = buildName();
        setName(&name);
        model = theModel;
        octreeName = getName() + ".oct";
        fixString(&octreeName);
    }
    
    ~OconvTask()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return (
                model == static_cast<OconvTask *>(t)->model &&
                options.isEqual(&static_cast<OconvTask *>(t)->options)
                );
    }
    
    bool solve()
    {
        tbb::mutex::scoped_lock lock(oconvMutex);
        RadExporter exporter = RadExporter(model);
        
        if (!oconv(octreeName, &options, exporter)) {
            FATAL(errmsg, "Impossible to oconv");
            return false;
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
        return true;
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
    
    std::string buildName()
    {
        std::string ret = "Oconv";
        
        ret += options.getOption<bool>(std::string(OCONV_INCLUDE_WINDOWS)) ? ".1." : "0.";
        ret += options.getOption<bool>(std::string(OCONV_USE_BLACK_GEOMETRY)) ? "1." : "0.";
        ret += options.getOption<bool>(std::string(OCONV_LIGHTS_ON)) ? "1" : "0";
        
        return ret;
    }
};

extern OconvTask oconvTask;
