
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
#include "../../common/geometry/triangle.h"
#include "../../groundhogmodel/groundhogmodel.h"
#include "../../common/taskmanager/task.h"
#include "tbb/tbb.h"
#include "../../common/geometry/triangulation.h"
#include "../../grain_sizes.h"
#include "../radiance.h"

#include <fstream>



//! Triangulates a whole workplane
/*!
 Triangulates all the Polygon3D inside of a workplane
 */
class TriangulateWorkplane : public Task {
public:
    
    Workplane * workplane; //!< The workplane to triangulate
    std::vector <RAY> rays; //!< The generated rays
    std::vector <Triangle> triangles; //!< The generated triangles
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] aWorkplane The workplane to triangulate
     @param[in] area The maximum area allowed for each triangle in the final Triangulation
     @param[in] aspectRatio The maximum aspect ratio allowed for each triangle in the final Triangulation
     */
    TriangulateWorkplane(Workplane * aWorkplane)
    {
        workplane = aWorkplane;
        
        // Always report this
        reportResults = true;
        
        // It Does generate results
        generatesResults = true;
        
        std::string name = "Triangulate workplane " + *(aWorkplane->getName());
        setName(&name);
        
    }
    
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other task
     @return is equal?
     */
    bool isEqual(Task * t)
    {
        return workplane == static_cast<TriangulateWorkplane *>(t)->workplane;
    }
    
    //! Solves the task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        size_t nPols = workplane->getNumPolygons();
        
        // Initialize the triangulations
        std::vector<Triangulation * > triangulations = std::vector<Triangulation * >();
        
        // Fill the triangulations
        for(size_t i=0; i < nPols; i++){
            Polygon3D * p = workplane->getPolygonRef(i);
            Triangulation * t = new Triangulation(p);
            triangulations.push_back(t);
        }
        
        double maxArea = workplane->getMaxArea();
        double maxAspectRatio = workplane->getMaxAspectRatio();
        
        // Triangulate in parallel
        tbb::parallel_for(tbb::blocked_range<size_t>(0, nPols, EMP_TRIANGULATION_GRAIN_SIZE),
                          [=](const tbb::blocked_range<size_t>& r) {
                              for (size_t i = r.begin(); i != r.end(); ++i) {
                                  
                                  triangulations.at(i)->mesh(maxArea,maxAspectRatio);
                                  triangulations.at(i)->purge();
                              }
                          }
        );
        
        // Fill the results... in series
        size_t rayCount = 0;
        
        for(size_t i=0; i < nPols; i++){
            Triangulation * t = triangulations.at(i);
            size_t nTriangles = t->getNumTriangles();
            
            
            for(size_t j = 0; j < nTriangles; j++){
                
                // Add a Ray
                rays.push_back(RAY());
                
                // Get the Triangle
                Triangle * triangle = t->getTriangleRef(j);
                
                // Add the Triangle
                triangles.push_back(*triangle);
                
                // Add the center to the ray
                Point3D o = triangle->getCenter();
                Vector3D n = t->getPolygon()->getNormal();
                
                FVECT origin = {(float)o.getX(),(float)o.getY(),(float)o.getZ()};
                FVECT dir = {(float)n.getX(),(float)n.getY(),(float)n.getZ()};
                
                VCOPY(rays.at(rayCount).rorg, origin);
                VCOPY(rays.at(rayCount).rdir, dir);
                
                // Increase ray count
                rayCount++;
            }
            
            // Delete the Triangulation
            delete t;
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

extern TriangulateWorkplane triangulateWorkplane;
