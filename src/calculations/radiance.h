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

/// Do not change this, please... it is required to compile Radiance in C++
#define FUN_ARGLIST ...

#define GETLUX(r,g,b) (r*47.45 + g*119.95 + b*11.60)

#include "./oconv_options.h"
#include "groundhogmodel/src/rtraceoptions.h"
#include "./Radiance/src/rt/ray.h"
#include "config_constants.h"

#include "Radiance/src/common/otypes.h"

#include "common/geometry/triangulation.h"

#include "writers/rad/radexporter.h"

//! This function emulates the use of Radiance's RTRACE program
/*!
@author German Molina
@param[in] t The Triangulation object from which rays will be traced
@param[in] options The RTRACE options
@param[in] baseDir The directory where the model and octree are located
@param[in] octname The name of the octree to read
@param[in] do_irrad The parameter that emulates the '-i' option in RTRACE
@param[in] imm_irrad The parameter that emulates the '-I' option in RTRACE
@param[in] amb The name of the ambient file to use
@param[out] rays The place where the resulting rays will be stored
*/
bool rtrace(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, bool do_irrad, bool imm_irrad, std::string amb, std::vector<RAY> * rays);


//! This function emulates the use of Radiance's RTRACE program with the -I option enabled
/*!
@author German Molina
@param[in] t The Triangulation object from which rays will be traced
@param[in] options The RTRACE options
@param[in] baseDir The directory where the model and octree are located
@param[in] octname The name of the octree to read
@param[in] amb The name of the ambient file to use
@param[out] rays The place where the resulting rays will be stored
*/
bool rtrace_I(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, std::string amb, std::vector<RAY> * rays);


//! This function emulates the use of Radiance's RTRACE program with the -i option enabled
/*!
@author German Molina
@param[in] t The Triangulation object from which rays will be traced
@param[in] options The RTRACE options
@param[in] baseDir The directory where the model and octree are located
@param[in] octname The name of the octree to read
@param[in] amb The name of the ambient file to use
@param[out] rays The place where the resulting rays will be stored
*/
bool rtrace_i(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, std::string amb, std::vector<RAY> * rays);


//! Creates an octree according to certain option
/*
@author German Molina
@param[in] octreeName The name of the octree to create
@param[in] options The OconvOptions set
@param[in] exporter The RadianceExporter that will write all the necessary geometry
@todo Lights on
*/
bool oconv(std::string octreeName, OconvOptions * options, RadExporter exporter);