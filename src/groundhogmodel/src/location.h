#pragma once

#include <string>

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

public:
	
	//! Creates a new Location object
	/*!
	Both City Name and Country Name are set to 'not specified'. The Latitude, 
	Longitude and Time Zone are set to 0.

	@author German Molina	
	*/
	Location();
	
	//! Destroys a Location object
	/*!
	@author German Molina
	*/
	~Location();

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
	double getTimezone();

	//! Sets the Time zone
	/*!
	@author German Molina
	@param[in] t The time zone
	*/
	void setTimezone(double t);
	
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
};