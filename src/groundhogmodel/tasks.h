
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
#include "groundhogmodel/groundhogmodel.h"
#include "groundhogmodel/tasks.h"
#include "./triangulate.h"
#include "common/taskmanager/task.h"
#include "tbb/tbb.h"
#include "common/geometry/triangulation.h"

#define EMP_TRIANGULATION_GRAIN_SIZE 24

#include <fstream>


//! Triangulates a whole workplane
/*!
 Triangulates all the Polygon3D inside of a workplane
 */
class TriangulateWorkplane : public Task {
public:
    
    Workplane * workplane; //!< The workplane to triangulate
    double maxArea; //!< The maximum area allowed for each triangle in the resulting Triangulation
    double maxAspectRatio; //!< The maximum aspect ratio allowed for each triangle in the resulting Triangulation
    std::vector<Triangulation * > triangulations; //!< The final triangulations
    
    
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] aWorkplane The workplane to triangulate
     @param[in] area The maximum area allowed for each triangle in the final Triangulation
     @param[in] aspectRatio The maximum aspect ratio allowed for each triangle in the final Triangulation
     */
    TriangulateWorkplane(Workplane * aWorkplane, double area, double aspectRatio)
    {
        workplane = aWorkplane;
        maxArea = area;
        maxAspectRatio = aspectRatio;
        
        // Always report this
        reportResults = true;
        // It Does generate results
        generatesResults = true;
        
        std::string name = "Triangulate workplane " + *(aWorkplane->getName()) + "-" +std::to_string(maxArea)+"_"+std::to_string(maxAspectRatio);
        setName(&name);
        
        size_t nPols = workplane->getNumPolygons();
        for (size_t i = 0; i < nPols; i++) {
            Polygon3D * p = workplane->getPolygonRef(i);
            triangulations.push_back(new Triangulation(p));
        }
        
    }
    
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other task
     @return is equal?
     */
    bool isEqual(Task * t)
    {
        return (
                workplane == static_cast<TriangulateWorkplane *>(t)->workplane &&
                maxArea == static_cast<TriangulateWorkplane *>(t)->maxArea &&
                maxAspectRatio == static_cast<TriangulateWorkplane *>(t)->maxAspectRatio
                );
    }
    
    //! Solves the task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        size_t nPols = workplane->getNumPolygons();
        
        tbb::parallel_for(tbb::blocked_range<size_t>(0, nPols, EMP_TRIANGULATION_GRAIN_SIZE),
                          [=](const tbb::blocked_range<size_t>& r) {
                              for (size_t i = r.begin(); i != r.end(); ++i) {
                                  triangulations.at(i)->mesh(maxArea, maxAspectRatio);
                                  triangulations.at(i)->purge();
                              }
                          }
                          );
        
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

