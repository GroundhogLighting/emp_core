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

#include "../../common/utilities/file.h"
#include "../../common/utilities/stringutils.h"
#include <fstream>


Location::Location() {
	city = "not specified";
	country = "not specified";
	latitude = 0;
	longitude = 0;
	timeZone = 0;
}

float Location::getLatitude() const
{
	return latitude;
}
void Location::setLatitude(float l)
{
	latitude = l;
}
float Location::getLongitude() const
{
	return longitude;
}
void Location::setLongitude(float l)
{
	longitude = l;
}
float Location::getTimeZone() const
{
	return timeZone;
}
void Location::setTimeZone(float t)
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

void Location::setAlbedo(float a)
{
	albedo = a;
}

float Location::getAlbedo() const
{
	return albedo;
}

bool Location::fillWeatherFromJSON(json * j)
{
	elevation = j->at("elevation").get<float>();
	return weather.fillFromJSON(j);
}

bool Location::fillWeatherFromEPWFile(std::string filename)
{
    if(!fexists(filename))
        FATAL(e,"File "+filename+" does not exist");
    
    std::ifstream infile(filename);
    
    std::string line;
    size_t lnCount = 0;
    while (std::getline(infile, line))
    {
        std::vector < std::string> tokens = std::vector < std::string>();
        tokenize(&line,",",&tokens);
        
        // Header information
        if(lnCount == 0){
            setCity(tokens.at(1));
            setCountry(tokens.at(3));
            setLatitude(atof(&tokens.at(6)[0]));
            setLongitude(-atof(&tokens.at(7)[0]));
            setTimeZone(atof(&tokens.at(8)[0]));
            setElevation(atof(&tokens.at(9)[0]));
        }else if(lnCount < 8){
            // skip
            
        }
        // weather data
        else{
            
            HourlyData h = HourlyData();
            
            h.month = atoi(&tokens.at(1)[0]);
            h.day = atoi(&tokens.at(2)[0]);
            
            h.hour = atof(&tokens.at(3)[0])-0.5;
            h.direct_normal = atof(&tokens.at(14)[0]);
            h.diffuse_horizontal = atof(&tokens.at(15)[0]);
            
            addHourlyData(h);
        }
        
        
        lnCount++;
    }
    
    markWeatherAsFilled();
    
    return true;
}


bool Location::fillWeatherFromWEAFile(std::string filename)
{
    if(!fexists(filename))
        FATAL(e,"File "+filename+" does not exist");
    
    std::ifstream infile(filename);
    
    std::string line;
    size_t lnCount = 0;
    while (std::getline(infile, line))
    {
        
        std::vector < std::string> tokens = std::vector < std::string>();
        tokenize(&line,&tokens);
        
        // Header information
        if(lnCount == 0){
            setCountry(tokens.at(1));
        }else if(lnCount == 1){
            setLatitude(atof(&tokens.at(1)[0]));
        }else if(lnCount == 2){
            setLongitude(atof(&tokens.at(1)[0]));
        }else if(lnCount == 3){
            setTimeZone(-atof(&tokens.at(1)[0])/15.0f);
        }else if(lnCount == 4){
            setElevation(atof(&tokens.at(1)[0]));
        }else if(lnCount == 5){
            // skip
        }
        // weather data
        else{
            HourlyData h = HourlyData();
            
            h.month = atoi(&tokens.at(0)[0]);
            h.day = atoi(&tokens.at(1)[0]);
            
            h.hour = atof(&tokens.at(2)[0]);
            h.direct_normal = atof(&tokens.at(3)[0]);
            h.diffuse_horizontal = atof(&tokens.at(4)[0]);
            
            addHourlyData(h);
        }
        
        
        lnCount++;
    }
    
    markWeatherAsFilled();
    
    return true;
}


float Location::getElevation() const
{
	return elevation;
}

void Location::setElevation(float a)
{
    elevation = a;
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
    
    const size_t weaSize = weather.data.size();
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

void Location::getDataByDate(int month, int day, float hour, HourlyData * data) const
{
    const int weaSize = (int)weather.data.size();
    
    for(int i=0; i<weaSize; i++){
        const HourlyData * now = &weather.data[i];
        
        // Skip if does not fit
        if(month != now->month || day != now->day)
            continue;
        
        // Lets get the hour now
        const HourlyData * next = &weather.data[i+1];
        if( now->hour <= hour && next->hour >= hour){
            // We are in the correct band... interpolate
            float x = (hour - now->hour)/(next->hour - now->hour);
            getInterpolatedData(i,x,data);
            return;
        }
    }
    // If we get to this point, we have a problem
    throw "Fatal: Your weather data does not have information for date "+std::to_string(day)+"/"+std::to_string(month);
}
