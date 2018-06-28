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

#include <string>
#include "./weather.h"

//! Represents a Location

/*!
This object contains the information required to place the sun somewhere
*/

class Location {
private: 
	double latitude; //!< The latitude in degrees North (use negative for South)
	double longitude; //!< The longitude in degrees West (use negative for East)
	double timeZone; //!< in GMT units
	std::string city; //!< The name of the city
	std::string country; //!< The name of the country
	double albedo = 0.2; //!< The albedo in the location
	Weather weather = Weather(); //!< The weather of the location obtained from a weather file
	double elevation = 0; //!< The elevation

public:
	
	//! Creates a new Location object
	/*!
	Both City Name and Country Name are set to 'not specified'. The Latitude, 
	Longitude and Time Zone are set to 0.

	@author German Molina	
	*/
	Location();
		

	//! Retrieves the latitude
	/*!
	@author German Molina
	@return the latitude
	*/
	double getLatitude();

	//! Sets the latitude
	/*!
	@author German Molina
	@param[in] l The latitude
	*/
	void setLatitude(double l);
	
	//! Retrieves the longitude
	/*!
	@author German Molina
	@return the longitude
	*/
	double getLongitude();

	//! Sets the Longitude
	/*!
	@author German Molina
	@param[in] l The longitude
	*/
	void setLongitude(double l);
	
	//! Retrieves the Time Zone
	/*!
	@author German Molina
	@return the time zone
	*/
	double getTimeZone();

	//! Sets the Time zone
	/*!
	@author German Molina
	@param[in] t The time zone
	*/
	void setTimeZone(double t);
	
	//! Retrieves the City name
	/*!
	@author German Molina
	@return the city name
	*/
	std::string getCity();

	//! Sets the City name
	/*!
	@author German Molina
	@param[in] c The city name
	*/
	void setCity(std::string c);
	
	//! Retrieves the Country name
	/*!
	@author German Molina
	@return the country name
	*/
	std::string getCountry();

	//! Sets the Country name
	/*!
	@author German Molina
	@param[in] c The country name
	*/
	void setCountry(std::string c);

	//! Sets the albedo
	/*!
	@author German Molina
	@param[in] a The albedo
	*/
	void setAlbedo(double a);

	//! Gets the albedo
	/*!
	@author German Molina
	@return The albedo
	*/
	double getAlbedo();

	//! Fill Weather member from JSON object
	/*!
	@author German Molina
	@param[in] j The JSON object
	@return success
	*/
	bool fillWeatherFromJSON(json * j);

	//! Gets the elevation
	/*!
	@author German Molina
	@return The elevation
	*/
	double getElevation();

	//! Gets the HourlyData for some hour
	/*!
	@author German Molina
	@param[in] hour The hour of the year
	@return The data
	*/
	HourlyData * getHourlyData(size_t hour);
    
    //! Add hourly data
    /*!
     @author German Molina
     @param h The hourly data to add
     */
    void addHourlyData(HourlyData h);
    
    //! Gets the number of samples in the weather data
    /*!
     @author German Molina
     @return The weather data length
     */
    size_t getWeatherSize();

	//! Checks if the weather has been filled with data
	/*!
	@author German Molina
	*/
	bool hasWeather();
    
    //! Marks weather as filled
    /*!
     @author German Molina
     */
    void markWeatherAsFilled();
    
    //! Interpolats the weather data
    /*!
     @author German Molina
     @param step The starting measured point     
     @param i The scheme to interpolate (0 to 1)
     @param data The HourlyData object to put the results in
     @todo Consider interpolation at sunrise and sunset!
     */
    void getInterpolatedData(int step,float i,HourlyData * data);
};
