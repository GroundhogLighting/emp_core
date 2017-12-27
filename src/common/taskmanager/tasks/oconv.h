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
#include "common/utilities/stringutils.h"
#include "calculations/oconv_options.h"
#include "calculations/radiance.h"

class OconvTask : public Task {


public:
  GroundhogModel * model;
  OconvOptions options;
  OconvTask(GroundhogModel * theModel, OconvOptions * theOptions)
  {
    options = *theOptions;
    std::string name = buildName();
    setName(&name);
    model = theModel;

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
    RadExporter exporter = RadExporter(model, "WILL NOT BE USED", false);
    std::string octName = *getName() + ".oct";

    if (!oconv(octName, &options, exporter)) {
      FATAL(errmsg, "Impossible to oconv");
      return false;
    }

    return true;
  }

  std::string buildName()
  {
    std::string ret = "Oconv";

    /*
    ret += "_"+std::string(OCONV_INCLUDE_WINDOWS) + "-" + (options.getOption<bool>(std::string(OCONV_INCLUDE_WINDOWS)) ? "true" : "false");
    ret += "_"+std::string(OCONV_USE_BLACK_GEOMETRY) + "-" + (options.getOption<bool>(std::string(OCONV_USE_BLACK_GEOMETRY)) ? "true" : "false");
    ret += "_" + std::string(OCONV_INCLUDE_SKY) + "-" + (options.getOption<bool>(std::string(OCONV_INCLUDE_SKY)) ? "true" : "false");
    ret += "_" + std::string(OCONV_LIGHTS_ON) + "-" + (options.getOption<bool>(std::string(OCONV_LIGHTS_ON)) ? "true" : "false");
    ret += "_" + std::string(OCONV_SKY) + "-" + options.getOption<std::string>(std::string(OCONV_SKY)) ;
    */
    ret += options.getOption<bool>(std::string(OCONV_INCLUDE_WINDOWS)) ? "1." : "0.";
    ret += options.getOption<bool>(std::string(OCONV_USE_BLACK_GEOMETRY)) ? "1." : "0.";
    ret += options.getOption<bool>(std::string(OCONV_INCLUDE_SKY)) ? "1." : "0.";
    ret += options.getOption<bool>(std::string(OCONV_LIGHTS_ON)) ? "1." : "0.";

    std::string sky = options.getOption<std::string>(std::string(OCONV_SKY));   
    fixString(&sky[0],sky.size());
    ret += sky;

    return ret;
  }
};