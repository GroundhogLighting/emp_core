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


#include "./location.h"
#include "../../common/utilities/io.h"

Location::Location() {
	city = "not specified";
	country = "not specified";
	latitude = 0;
	longitude = 0;
	timeZone = 0;
}

double Location::getLatitude() const
{
	return latitude;
}
void Location::setLatitude(double l)
{
	latitude = l;
}
double Location::getLongitude() const
{
	return longitude;
}
void Location::setLongitude(double l)
{
	longitude = l;
}
double Location::getTimeZone() const
{
	return timeZone;
}
void Location::setTimeZone(double t)
{
	timeZone = t;
}

std::string Location::getCity() const
{
	return city;
}

void Location::setCity(std::string c)
{
	city = c;
}

std::string Location::getCountry() const
{
	return country;
}

void Location::setCountry(std::string c)
{
	country = c;
}

void Location::setAlbedo(double a)
{
	albedo = a;
}

double Location::getAlbedo() const
{
	return albedo;
}

bool Location::fillWeatherFromJSON(json * j)
{
	elevation = j->at("elevation").get<double>();
	return weather.fillFromJSON(j);
}


double Location::getElevation() const
{
	return elevation;
}

bool Location::hasWeather() const
{
	return weather.hasData();
}

HourlyData * Location::getHourlyData(size_t hour)
{
	return &(weather.data.at(hour));
}

size_t Location::getWeatherSize() const
{
    return weather.data.size();
}

void Location::addHourlyData(HourlyData h)
{
    weather.data.push_back(h);
}

void Location::markWeatherAsFilled()
{
    weather.filled = true;
}

void Location::getInterpolatedData(int beg,float i,HourlyData * data) const
{
    if(!weather.hasData())
        throw "Fatal: Cannot simulate because it has no data";
    
    size_t weaSize = weather.data.size();
    if(beg < 0 )
        throw "Impossible beggining data point in Weather File interpolation";
    
    
    const HourlyData * startData = &(weather.data[beg]);
    const HourlyData * endData = &(weather.data[(beg + 1) % weaSize]);
    
    data->month = startData->month;
    data->day = startData->day;
    data->hour = startData->hour + i*(endData->hour - startData->hour);
    
    // If it is night in both, return Zeroes, with the month of the previous
    if(startData->diffuse_horizontal < 1e-3 && endData->diffuse_horizontal < 1e-3){
        data->diffuse_horizontal = 0;
        data->direct_normal = 0;
        return;
    }
    
    // From now on, Month and Day should be the same in both startingData and endData
    // as nights (changes in day and month) are considered in the case before.
    
    // Linearly interpolate the diffuse_horizontal
    data->diffuse_horizontal = startData->diffuse_horizontal + i*(endData->diffuse_horizontal - startData->diffuse_horizontal);
    
    // Deal with direct normal
    data->direct_normal = startData->direct_normal + i*(endData->direct_normal - startData->direct_normal);
}
