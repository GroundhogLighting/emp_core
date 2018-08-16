
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


#include "./Calculate2PhaseGlobalIlluminance.h"
#include "../../taskmanager/cbdm_task.h"



class CheckUDICompliance : public CBDMTask {
    
public:
    
    CheckUDICompliance(std::string name, EmpModel * theModel, RTraceOptions * theOptions, Workplane * wp, int sunMf, int skyMf, double theMinLux, double theMaxLux, float theEarly, float theLate, int minMonth, int maxMonth, float theMinTime);
    
    
    CheckUDICompliance(std::string name, EmpModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays, int sunMf, int skyMf,double theMinLux, double theMaxLux, float theEarly, float theLate, int minMonth, int maxMonth, float theMinTime);
                    
    GET_DEP_RESULTS(Calculate2PhaseGlobalIlluminance);
};

extern CheckUDICompliance checkUDI;




