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

#include <iostream>
#include "./weather.h"

bool Weather::fillFromJSON(json * j)
{
	filled = true;

	json d = j->at("data").get<json>();
    size_t nData = d.size(); 
    
	for (size_t i = 0; i < nData; i++) {
        data.push_back( HourlyData() );
		data[i].month = d[i].at("month").get<int>();
		data[i].day = d[i].at("day").get<int>();
		data[i].hour = d[i].at("hour").get<double>();
		data[i].diffuse_horizontal = d[i].at("diffuse_horizontal").get<int>();
		data[i].direct_nomal = d[i].at("direct_normal").get<int>();
	}

	return true;
}

bool Weather::hasData()
{
	return filled;
}
