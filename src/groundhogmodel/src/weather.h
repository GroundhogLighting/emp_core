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
#include  <vector>
#include "../../3rdparty/json/json.hpp"

using nlohmann::json;

//! Represents a line in a weather tape
/*!
	Contains the necessary information for performing lighting simulation
	on such hour
*/

class HourlyData {
public:
	int month; //!< The month
	int day; //!< The day
	double hour; //!< The hour
	int diffuse_horizontal; //!< The diffuse horizontal radiation
	int direct_nomal; //!< The direct normal radiation
};

//! Represents a whole weather
/*!
	Contains all the information related to the weather
	of a Location. It is extracted from Weather data
*/

class Weather {

private:
	bool filled = false; //!< Indicates whether the Weather has been populated

public:
	std::vector<HourlyData> data = std::vector<HourlyData>(8760); //!< The actual data

	//! Grabs a JSON representing the Weather file and fills the Weather object
	/*!
	An example of the JSON:

	{
		"city":"Denver-Centennial--Golden---Nr",
		"state":"CO",
		"country":"USA",
		"latitude":39.74,
		"longitude":-105.18,
		"timezone":-7,
		"elevation":1829.0,
		"data": [
					{"month":1,"day":1,"hour":0.5,"direct_normal":"0","diffuse_horizontal":"0"},
					...
				] // This is a 8760 long array
	}

	@author German Molina
	@param j The pointer to the JSON object
	@return success
	*/
	bool fillFromJSON(json * j);

	//! Checks if the Weather file has been filled with data
	/*!
	@author German Molina
	@return filled
	*/
	bool hasData();
};