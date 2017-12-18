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
#include "../task.h"
#include "common/geometry/triangulation.h"
#include "groundhogmodel/src/workplane.h"


//! Transforms a Polygon into a Triangulation
/*!
This task is used by Workplane triangulation in order to export 
the whole model
*/
class TriangulatePolygon : public Task {

public:
  double maxArea; //!< The maximum area allowed for each triangle in the triangulation
  double maxAspectRatio; //!< The maximum aspect ratio allowed for each triangle in the triangulation
  Triangulation triangulation; //!< The Triangulation object
  Polygon3D * polygon; //!< The Polygon3D object to triangulate

  //! Constructor
  /*!
  @author German Molina
  @param[in] aPolygon The polygon to triangulate
  @param[in] area The maximum area allowed for the triangles in the resulting triangulation
  @param[in] aspectRatio The maximum aspect ratio allowed for the triangles in the resulting triangulation
  */
  TriangulatePolygon(Polygon3D * aPolygon, double area, double aspectRatio)
  {
    std::string name = "Triangulate polygon";
    setName(&name);

    polygon = aPolygon;
    maxArea = area;
    maxAspectRatio = aspectRatio;

    triangulation = Triangulation(aPolygon);
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
      polygon == static_cast<TriangulatePolygon *>(t)->polygon &&
      maxArea == static_cast<TriangulatePolygon *>(t)->maxArea
      );
  }

  //! Solves the task
  /*!
  @author German Molina
  @return success
  */
  bool solve()
  { 
    return triangulation.mesh(maxArea, maxAspectRatio);            
  }


};


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
    std::string name = "Triangulate workplane " + *(aWorkplane->getName());
    setName(&name);
    workplane = aWorkplane;
    maxArea = area;
    maxAspectRatio = aspectRatio;

    size_t nWps = workplane->getNumPolygons();
    
    // This depends on the triangulation of all polygons... 
    for (size_t i = 0; i < nWps; i++) {
      Polygon3D * p = workplane->getPolygonRef(i);
      TriangulatePolygon * t = new TriangulatePolygon(p, maxArea, maxAspectRatio);
      addDependency(t);      
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
    //Gather results
    size_t n = countDependencies();
    for (size_t i = 0; i < n; i++) {
      TriangulatePolygon * d = dynamic_cast<TriangulatePolygon *>(getDependencyRef(i));
      triangulations.push_back(&(d->triangulation));
    }
    return true;
  }


};