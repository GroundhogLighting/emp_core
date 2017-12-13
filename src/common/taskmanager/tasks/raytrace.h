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

#include "./oconv.h"
#include "writers/rad/radexporter.h"
#include "./triangulate.h"

class RTraceTask : public Task {

public:
  GroundhogModel * model;
  OconvOptions options;
  std::vector<Triangulation * > * triangulations;


  RTraceTask(GroundhogModel * theModel, OconvOptions * theOptions, Workplane * wp)
  {
    model = theModel;
    options = *theOptions;
    
    // Create and add the Oconv dependency
    OconvTask * oconvTask = new OconvTask(model, &options);
    addDependency(oconvTask);
    
    // Create and add the TriangulateWorkplane dependency
    double maxArea = options.getOption<double>("max_area");
    double maxAspectRatio = options.getOption<double>("max_aspect_ratio");
    TriangulateWorkplane * triangulateWorkplaneTask = new TriangulateWorkplane(wp, maxArea, maxAspectRatio);
    triangulations = &(triangulateWorkplaneTask->triangulations);
    addDependency(triangulateWorkplaneTask);

    // Set the name
    std::string aux = oconvTask->buildName();
    std::string name = "Rtrace "+(*wp->getName())+ "_"+aux+"_"+std::to_string(maxArea) + "_" + std::to_string(maxAspectRatio);
    setName(&name);


  }

  
  bool isEqual(Task * t)
  {
    return (
      model == static_cast<RTraceTask *>(t)->model &&
      options.isEqual(& static_cast<RTraceTask *>(t)->options)
      );
  }
  

  bool solve()
  {
    return true;
  }

};