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


#include "./location.h"
#include "common/utilities/io.h"

Location::Location() {
	city = "not specified";
	country = "not specified";
	latitude = 0;
	longitude = 0;
	timeZone = 0;
}

Location::~Location() {
}

double Location::getLatitude() {
	return latitude;
}
void Location::setLatitude(double l) {
	latitude = l;
}
double Location::getLongitude() {
	return longitude;
}
void Location::setLongitude(double l) {
	longitude = l;
}
double Location::getTimeZone() {
	return timeZone;
}
void Location::setTimeZone(double t) {
	timeZone = t;
}

std::string Location::getCity() {
	return city;
}

void Location::setCity(std::string c) {
	city = c;
}

std::string Location::getCountry() {
	return country;
}

void Location::setCountry(std::string c) {
	country = c;
}

void Location::setAlbedo(double a)
{
	albedo = a;
}

double Location::getAlbedo()
{
	return albedo;
}

bool Location::fillWeatherFromJSON(json * j)
{
	elevation = j->at("elevation").get<double>();
	return weather.fillFromJSON(j);
}


double Location::getElevation()
{
	return elevation;
}

bool Location::hasWeather()
{
	return weather.hasData();
}

HourlyData * Location::getHourlyData(size_t hour)
{
	return &(weather.data[hour]);
}
