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

// GenCumulativeSky was conceived, developed and validated by Darren Robinson and
// Andrew Stone for efficient solar irradiation modelling using RADIANCE

// When using GenCumulativeSky they would be pleased if you would ackowledge their work
// by referring to the following article: "Robinson, D., Stone, A., Irradiation modeling
//made simple � the cumulative sky approach and its applications, Proc. PLEA 2004, Eindhoven 2004."

#pragma once

#include "../emp_model/emp_model.h"

#ifndef M_PI
#define M_PI 3.141592654
#endif

void genCumulativeSky(EmpModel * model, bool DoIlluminance, bool DoDiffuse, std::string filename);

