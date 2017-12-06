/*****************************************************************************
Glare

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

class ExportRadianceDir : public Task {
private:
	std::string dir;
	GroundhogModel * model;
	bool verbose;

public:

	ExportRadianceDir(std::string theDir, GroundhogModel * ghmodel, bool verb)
	{
      std::string name = "Export model";
	  setName(&name);

	  dir = theDir;
	  model = ghmodel;
	  verbose = verb;
	}

	bool isEqual(Task * t)
	{
		return (
					model == static_cast<ExportRadianceDir *>(t)->getModel() &&
					dir == static_cast<ExportRadianceDir *>(t)->getDir()  
				);
	}

	bool solve()
	{
		RadExporter r = RadExporter(model, dir, verbose);
		return r.exportModel();
	}

	std::string getDir()
	{
		return dir;
	}

	GroundhogModel * getModel()
	{
		return model;
	}
};



class ExportRadianceDirWithWorkplanes : public Task {
private:
	std::string dir; //!< The directory where to export
	GroundhogModel * model; //!< The model to export
	bool verbose; //!< inform progress?
    std::vector <std::vector <Triangulation *>> triangulatedWorkplanes = std::vector <std::vector <Triangulation *>>(); //!< Reference to the triangulations that will be exported
    std::vector <std::string * > names; //!< Reference to the names of the workplanes

public:

	ExportRadianceDirWithWorkplanes(std::string theDir, GroundhogModel * ghmodel, bool verb)
	{
      std::string name = "Export model with WPs";
	  setName(&name);

	  dir = theDir;
	  model = ghmodel;
	  verbose = verb;

      // Add to the export dependency
      addDependency(new ExportRadianceDir(dir, model, verbose));

      // Create main export
      TriangulateWorkplane * triangulateTask;

      // Add Triangulate task for each workplane
      size_t nWps = model->getNumWorkplanes();
      for (size_t i = 0; i < nWps; i++)
      {
        // Get the workplane
        Workplane * wp = model->getWorkplaneRef(i);
        
        // create the Triangulate Task
        triangulateTask = new TriangulateWorkplane(wp, 0.25,1.3);
                
        // Add the triangulate as a dependency to this task
        addDependency(triangulateTask);

        // get note of the triangulation
        triangulatedWorkplanes.push_back(triangulateTask->triangulations);
        names.push_back(triangulateTask->workplane->getName());

      }

	}

	bool isEqual(Task * t)
	{
		return (
			model == static_cast<ExportRadianceDir *>(t)->getModel() &&
			dir == static_cast<ExportRadianceDir *>(t)->getDir()
			);
	}

	bool solve()
	{
      // Check if there are any workplanes
      size_t nWPs = triangulatedWorkplanes.size();
      INFORM(iMsg, "Exporting " + std::to_string(nWPs) + " workplanes", verbose);
      if (nWPs == 0)
        return true;

      // create directory
      std::string baseDir = dir + "/" + GLARE_WORKPLANES_SUBFOLDER;
      createdir(baseDir);

      // Iterate workplanes
      for (size_t i = 0; i < nWPs; i++) {
        
        // Retrieve the triangulated workplane
        std::vector < Triangulation * > thisWorkplane = triangulatedWorkplanes[i];

        // Retrieve the name
        std::string ptsFileName = baseDir + "/" + *names[i] + ".pts";
        std::string pxlFileName = baseDir + "/" + *names[i] + ".pxl";
        

        // Create the files
        std::ofstream ptsFile;
        ptsFile.open(ptsFileName);
        std::ofstream pxlFile;
        pxlFile.open(pxlFileName);

        // Write down all polygons in triangulation
        size_t nP = thisWorkplane.size();
        for (size_t j = 0; j < nP; j++) {

          // Get the triangulated polygon
          Triangulation * triangulatedPolygon = thisWorkplane[j];

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
              pxlFile << px << GLARE_TAB << py << GLARE_TAB << pz << GLARE_TAB;
            }
            pxlFile << "\n";
            ptsFile << x / 3 << GLARE_TAB << y / 3 << GLARE_TAB << z / 3 << GLARE_TAB << nx << GLARE_TAB << ny << GLARE_TAB << nz << "\n";
            
          }

        }


        // Close the files
        ptsFile.close();
        pxlFile.close();
      }

      return true;
	}

	std::string getDir()
	{
		return dir;
	}

	GroundhogModel * getModel()
	{
		return model;
	}
};