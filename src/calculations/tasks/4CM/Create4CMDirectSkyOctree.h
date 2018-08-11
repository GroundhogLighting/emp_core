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
#include "../OconvTask.h"
#include "../../../os_definitions.h"

class Create4CMDirectSkyOctree : public Task {
public:
    EmpModel * model; //!< The model to Oconv
    std::string octreeName; //!< The name of the final octree
    
    Create4CMDirectSkyOctree(EmpModel * theModel)
    {
        
        std::string name = "Create CreateDirectSkyOctree";
        setName(&name);
        model = theModel;
        
        // Add the BlackOctree dependency... black geometry, no sky, no lights
        OconvOptions oconvOptions = OconvOptions();
        oconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
        oconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);        
        oconvOptions.setOption(OCONV_LIGHTS_ON, false);
        
        OconvTask * oconvTask = new OconvTask(model,&oconvOptions);
        addDependency(oconvTask);// --> Dependency 0
        
    }
    
    ~Create4CMDirectSkyOctree()
    {
        remove(&octreeName[0]);
    }
    
    bool isEqual(Task * t)
    {
        return model == static_cast<Create4CMDirectSkyOctree *>(t)->model;
    }
    
    bool solve()
    {        
        const double pi = 3.141592654;
        const double desiredElementWidth = 0.183333; // Degrees
        const double dAngle = 0.4 * pi / 180.0;
        const double elementBrighness =  pi/0.51757717132568359;
        
        std::string octName = static_cast<OconvTask *>(getDependencyRef(0))->getName() + ".oct";
        octreeName = "DIRECT_SKY_" + octName;
        std::string command = "oconv -i " + std::string(octName) + " - > " + octreeName;
        
        //remove(&octreeName[0]);
        
        FILE *octree = POPEN(&command[0], "w");
        fprintf(octree, "void light element 0 0 3 %f %f %f\n",elementBrighness,elementBrighness,elementBrighness);
        
        
        
        // define altitide
        double altitude = dAngle/2.0;
        
        
        // The numbr of sources that fit a band
        size_t nSourcesInBand;
        
        // Element width
        double elementWidth;
        
        // Iterate going up
        int band = 0;
        while (altitude < 1.57079632679){
            nSourcesInBand = floor(pi * cos(altitude) / tan(dAngle/2.0));
            
            // Increase the solid angle
            elementWidth = desiredElementWidth;
            
            // Write
            double dAz = 2*pi/nSourcesInBand;
            double az = dAz/2;
            for (int i = 0; i < nSourcesInBand; i++){
                fprintf(octree,"element source %d_%d 0 0 4 %f %f %f %f\n", band, i, sin(az)*cos(altitude),cos(az)*cos(altitude),sin(altitude),elementWidth);
                
                az += dAz;
            }
            band ++;
            altitude += dAngle;
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

extern Create4CMDirectSkyOctree create4CMDirectSkyOctree;
