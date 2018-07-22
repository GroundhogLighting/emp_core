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



#include "./CheckASECompliance.h"


double aseScoreCalculator(double v, double minLux, double maxLux)
{
    return (v >= minLux ? 1 : 0);
}



CheckASECompliance::CheckASECompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, Workplane * wp, int theMf, double theMinLux, double theEarly, double theLate, int minMonth, int maxMonth, float theMinTime)
{
    model = theModel;
    minLux = theMinLux;
    early = theEarly;
    late = theLate;
    firstMonth = minMonth;
    lastMonth = maxMonth;
    workplane = wp;
    minTime = theMinTime;
    scoreCalculator = aseScoreCalculator;
    
    // Dependency 0
    CalculateDirectSolarIlluminance * illuminanceTask = new CalculateDirectSolarIlluminance(theModel, wp, theMf, theOptions, interp);
    addDependency(illuminanceTask);
    
    // Set the name    
    setName(&name);
}

CheckASECompliance::CheckASECompliance(std::string name, GroundhogModel * theModel, RTraceOptions * theOptions, std::vector<RAY> * theRays, int theMf,double theMinLux, double theEarly, double theLate, int minMonth, int maxMonth, double theMinTime)
{
    
    model = theModel;
    minLux = theMinLux;
    early = theEarly;
    late = theLate;
    firstMonth = minMonth;
    lastMonth = maxMonth;
    minTime = theMinTime;
    scoreCalculator = aseScoreCalculator;
    rays = theRays;
    
    // Dependency 0
    CalculateDirectSolarIlluminance * illuminanceTask = new CalculateDirectSolarIlluminance(theModel, theRays, theMf, theOptions, interp);
    addDependency(illuminanceTask);
    
    // Set the dependency results
    depResults = &(illuminanceTask->result);
    
    // Set the name
    setName(&name);
}








