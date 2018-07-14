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


#include "./DDC/CalculateDDCGlobalIlluminance.h"
#include "../../taskmanager/cbdm_task.h"

double daScoreCalculator(double v, double minLux, double maxLux);


class CheckDACompliance : public CBDMTask {
    
public:
    
    CheckDACompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, int sunMf, int skyMf, double theMinLux, double theEarly, double theLate, int minMonth, int maxMonth, float theMinTime);
    
    CheckDACompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays, int sunMf, int skyMf,double theMinLux, double theEarly, double theLate, int minMonth, int maxMonth, double theMinTime);

    GET_DEP_RESULTS(CalculateDDCGlobalIlluminance);
    
};

extern CheckDACompliance checkDA;




