#pragma once

#include <string>;

class Location {
private: 
	double latitude;
	double longitude;
	double timeZone;
	std::string city;
	std::string country;
public:
	Location();
	~Location();
	double getLatitude();
	void setLatitude(double l);
	double getLongitude();
	void setLongitude(double l);
	double getTimezone();
	void setTimezone(double t);
	std::string getCity();
	void setCity(std::string c);
	std::string getCountry();
	void setCountry(std::string c);
};