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
#include "writers/rad/radexporter.h"
#include "./triangulate.h"
#include "common/utilities/file.h"
#include <fstream>

//! Exports all the geometry in a GroundhogModel into a Radiance directory
/*!

*/
class ExportRadianceDir : public Task {
private:
	bool verbose; //!< Inform progress?

public:
	std::string dir; //!< The directory to export the model to
	GroundhogModel * model; //! The model to export
    
  //! Constructor
  /*!
  This Task only exports the geometry... it does not consider the workplanes.

  @author German Molina
  @param[in] theDir A directory to write the files in
  @param[in] ghmodel A pointer to the GroundhogModel to export
  @param[in] verb Verbose?
  */
  ExportRadianceDir(std::string theDir, GroundhogModel * ghmodel, bool verb)
  {
    std::string name = "Export model";
	setName(&name);

	dir = theDir;
	model = ghmodel;
	verbose = verb;
  }
    
  //! Compares to of these tasks
  /*!
  @author German Molina
  @param[in] t The other ExportRadianceDir object
  @return are equal?
  */
  bool isEqual(Task * t)
  {
	  return (
				  model == static_cast<ExportRadianceDir *>(t)->model &&
				  dir == static_cast<ExportRadianceDir *>(t)->dir  
			  );
  }

  //! Solves this task
  /*!
  @author German Molina
  @return success
  */
  bool solve()
  {
	  RadExporter r = RadExporter(model, dir, verbose);      
	  bool s = r.exportModel();      
      return s;
  }

};

//! Exports a workplane in Radiance format
/*!
This task depends on TriangulateWorkplane task.
*/
class ExportWorkplane : public Task {

public:
  Workplane * workplane; //!< The workplane to export
  double maxAspectRatio = 1.3; //!< The maximum aspect ratio allowed for this exportation
  double maxArea = 0.25; //!< The maximum area of the pixels (triangles) in the triangulation
  std::string exportName; //!< The name of the file to export

  //! Constructor
  /*!
  @author German Molina
  @param[in] workplane The workplane to export
  @param[in] maxArea The maximum area allowed for the triangulation
  @param[in] maxAspectRatio the maximum aspect ratio allowed in the triangulation
  @param[in] name The name of the file to write
  */
  ExportWorkplane(Workplane * workplane, double maxArea, double maxAspectRatio, std::string name)
  {
    std::string n = "Export workplane " + *(workplane->getName());
    setName(&n);

    // Add the dependency 0
    TriangulateWorkplane * triangulateDependency = new TriangulateWorkplane(workplane, maxArea, maxAspectRatio);
    addDependency(triangulateDependency);    
    exportName = name;

  }

  //! Compares to of these tasks
  /*!
  @author German Molina
  @param[in] t The other ExportRadianceDir object
  @return are equal?
  */
  bool isEqual(Task * t)
  {
    return (
      workplane == static_cast<ExportWorkplane *>(t)->workplane &&
      maxAspectRatio == static_cast<ExportWorkplane *>(t)->maxAspectRatio &&
      maxArea == static_cast<ExportWorkplane *>(t)->maxArea
      );
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
    std::vector<Triangulation * > * triangulations = &(dependency->triangulations);
    size_t nP = triangulations->size(); 

    // Iterate
    for (size_t j = 0; j < nP; j++) {
            
      // Get the triangulated polygon
      Triangulation * triangulatedPolygon = triangulations->at(j);

      // Retrieve the normal
      Vector3D normal = triangulatedPolygon->getPolygon()->getNormal();
      double nx = normal.getX();
      double ny = normal.getY();
      double nz = normal.getZ();

      // Count triangles
      size_t nTriangles = triangulatedPolygon->getNumTriangles();

      for (size_t k = 0; k < nTriangles; k++) {
        Triangle * triangle = triangulatedPolygon->getTriangleRef(k);

        if (triangle == NULL)
          continue;

        double x = 0;
        double y = 0;
        double z = 0;
        for (int l = 0; l < 3; l++) {
          Point3D * p = triangle->getVertex(l);
          double px = p->getX();
          double py = p->getY();
          double pz = p->getZ();
          x += px;
          y += py;
          z += pz;
          pxlFile << px << EMP_TAB << py << EMP_TAB << pz << EMP_TAB;
        }
        pxlFile << "\n";
        ptsFile << x / 3 << EMP_TAB << y / 3 << EMP_TAB << z / 3 << EMP_TAB << nx << EMP_TAB << ny << EMP_TAB << nz << "\n";

      }

    }


    // Close the files
    ptsFile.close();
    pxlFile.close();
  

    return true;
  }
};


//! Exports not only the geometry, but the workplanes as well
/*!
Implementing the exporting prcess in this way allows multicore exporting... One thread is used
for exporting geometry, and many others to triangulate and write workplanes.

The triangulation and writing of the Workplanes depend on the exportation of the geometry

*/
class ExportRadianceDirWithWorkplanes : public Task {
private:
	std::string dir; //!< The directory where to export
	GroundhogModel * model; //!< The model to export
	bool verbose; //!< inform progress?
    std::vector <std::vector <Triangulation *>> triangulatedWorkplanes = std::vector <std::vector <Triangulation *>>(); //!< Reference to the triangulations that will be exported
    std::vector <std::string * > names; //!< Reference to the names of the workplanes

public:

  //! Constructor
  /*!
  @author German Molina
  @param[in] theDir The directory to export the model
  @param[in] ghmodel The model to export
  @param[in] verbose Inform progress?
  */
	ExportRadianceDirWithWorkplanes(std::string theDir, GroundhogModel * ghmodel, bool verb)
	{
      std::string name = "Export model with WPs";
	  setName(&name);

	  dir = theDir;
	  model = ghmodel;
	  verbose = verb;

      // Create main export
      ExportWorkplane * exportWorkplaneTask;

      // Add Triangulate task for each workplane
      size_t nWps = model->getNumWorkplanes();
      for (size_t i = 0; i < nWps; i++)
      {

        // Get the workplane
        Workplane * wp = model->getWorkplaneRef(i);
        double maxArea = wp->getMaxArea();
       
        // create the Triangulate Task ... dependency i
        exportWorkplaneTask = new ExportWorkplane(wp, maxArea, 1.3, dir + "/" + EMP_WORKPLANES_SUBFOLDER+"/"+*(wp->getName()));
        exportWorkplaneTask->addDependency(new ExportRadianceDir(dir, model, verbose));
        
        // Add the triangulate as a dependency to this task
        addDependency(exportWorkplaneTask);        

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
			model == static_cast<ExportRadianceDirWithWorkplanes *>(t)->model &&
			dir == static_cast<ExportRadianceDirWithWorkplanes *>(t)->dir
			);
	}

    //! Solves this task
    /*!
    This task does not really do anything... just coordinate all the dependencies.

    @author German Molina
    @return success
    */
	bool solve()
	{            
      return true;
	}

};
