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

class TriangulatePolygon : public Task {
private:
  Triangulation triangulation;
  double maxArea;
  double maxAspectRatio;

public:
  Polygon3D * polygon;

  TriangulatePolygon(Polygon3D * aPolygon, double area, double aspectRatio)
  {
    std::string name = "triangulate polygon";
    setName(&name);

    polygon = aPolygon;
    maxArea = area;
    maxAspectRatio = aspectRatio;

    triangulation = Triangulation(aPolygon);
  }

  bool isEqual(Task * t)
  {
    return (
      polygon == static_cast<TriangulatePolygon *>(t)->polygon &&
      maxArea == static_cast<TriangulatePolygon *>(t)->maxArea
      );
  }

  bool solve()
  {
    return triangulation.mesh(maxArea, maxAspectRatio);
  }

  Triangulation * getTriangulation()
  {
    return &triangulation;
  }


};

class TriangulateWorkplane : public Task {
private:

public:

  Workplane * workplane;
  double maxArea;
  double maxAspectRatio;
  std::vector<Triangulation * > triangulations;

  TriangulateWorkplane(Workplane * aWorkplane, double area, double aspectRatio)
  {
    std::string name = "Triangulate workplane " + *(aWorkplane->getName());
    setName(&name);
    workplane = aWorkplane;
    maxArea = area;
    maxAspectRatio = aspectRatio;

    size_t nWps = workplane->getNumPolygons();
    
    // This depends on the triangulation of all polygons
    for (size_t i = 0; i < nWps; i++) {
      Polygon3D * p = workplane->getPolygonRef(i);
      TriangulatePolygon * t = new TriangulatePolygon(p, maxArea, maxAspectRatio);
      addDependency(t);
      triangulations.push_back(t->getTriangulation());
    }

  }

  bool isEqual(Task * t)
  {
    return (
      workplane == static_cast<TriangulateWorkplane *>(t)->workplane &&
      maxArea == static_cast<TriangulateWorkplane *>(t)->maxArea &&
      maxAspectRatio == static_cast<TriangulateWorkplane *>(t)->maxAspectRatio
      );
  }

  bool solve()
  {
    // Does not really do anything... just joins all the other dependencies
    return true;
  }


};