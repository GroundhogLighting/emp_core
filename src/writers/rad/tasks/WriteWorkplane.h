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

//! Exports a workplane in Radiance format
/*!
 This task depends on TriangulateWorkplane task.
 */
class WriteWorkplane : public Task {
    
public:
    Workplane * workplane; //!< The workplane to export
    std::string exportName; //!< The name of the file to export
    
    
    //! Constructor
    /*!
     @author German Molina
     @param[in] workplane The workplane to export
     @param[in] maxArea The maximum area allowed for the triangulation
     @param[in] maxAspectRatio the maximum aspect ratio allowed in the triangulation
     @param[in] name The name of the file to write
     */
    WriteWorkplane(Workplane * workplane, std::string name)
    {
        std::string n = "Export workplane " + workplane->getName();
        setName(&n);
        generatesResults = true;
        
        // Add the dependency 0
        TriangulateWorkplane * triangulateDependency = new TriangulateWorkplane(workplane);
        addDependency(triangulateDependency);
        exportName = name;
        
    }
    
    //! Compares two of these tasks
    /*!
     @author German Molina
     @param[in] t The other ExportRadianceDir object
     @return are equal?
     */
    bool isEqual(Task * t)
    {
        return (workplane == static_cast<WriteWorkplane *>(t)->workplane);
    }
    
    //! Solves this task
    /*!
     @author German Molina
     @return success
     */
    bool solve()
    {
        
        // Retrieve the name
        std::string ptsFileName = "./"+exportName + ".pts";
        std::string pxlFileName = "./" + exportName + ".pxl";
        
        // Create the files
        std::ofstream ptsFile;
        ptsFile.open(ptsFileName);
        if (!ptsFile.is_open())
            throw 20;
        
        std::ofstream pxlFile;
        pxlFile.open(pxlFileName);
        if (!pxlFile.is_open())
            throw 20;
        
        // Get the dependency... has only one.
        TriangulateWorkplane * dependency = static_cast<TriangulateWorkplane *>(getDependencyRef(0));
        
        // Write down all polygons in triangulation
        size_t nRays = dependency->rays.size();
        
        // Iterate
        for (size_t i = 0; i < nRays; i++) {
            
            // Get the Triangle and ray
            Triangle * triangle = dependency->triangles.at(i);
            RAY * ray = &(dependency->rays.at(i));
            
            if (triangle == NULL)
                continue;
            
            // Write in Pixel file
            for (int l = 0; l < 3; l++) {
                Point3D * p = triangle->getVertex(l);
                double px = p->getX();
                double py = p->getY();
                double pz = p->getZ();
                pxlFile << px << EMP_TAB << py << EMP_TAB << pz << EMP_TAB;
            }
            pxlFile << "\n";
            
            ptsFile << ray->rorg[0] << EMP_TAB << ray->rorg[1] << EMP_TAB << ray->rorg[2] << EMP_TAB;
            ptsFile << ray->rdir[0] << EMP_TAB << ray->rdir[1] << EMP_TAB << ray->rdir[2] << "\n";
            
            
        }
            
        
        
        
        // Close the files
        ptsFile.close();
        pxlFile.close();
        
        
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

extern WriteWorkplane writeWorkplane;
