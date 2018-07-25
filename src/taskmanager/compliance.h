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
#include "../calculations/radiance.h"
#include "../common/geometry/triangle.h"

double calcRaysCompliance(const std::vector<RAY> * rays, double minTime, double maxTime, const Matrix * result);


double calcWorkplaneCompliance(const std::vector <Triangle *> * triangles, double minTime, double maxTime, const Matrix * result);


void bulkResultsIntoJSON(std::string taskName, std::string wpName, const Matrix * results, double compliance, json * j);
