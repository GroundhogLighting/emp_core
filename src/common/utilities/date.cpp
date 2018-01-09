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


#include "./io.h"
#include "./date.h"


Date::Date(int m, int d, int h, int min) 
{
	month = m;
	day = d;
	hour = h;
	minute = min;
}


Date::Date(int64_t t) 
{

	tm * time = gmtime((const time_t *)(&t));
	month = (time->tm_mon) + 1;
	day = time->tm_mday;
	hour = time->tm_hour;
	minute = time->tm_min;
	delete time;

}

Date::~Date() 
{
}

void Date::setMonth(int m) 
{
	month = m;
}
void Date::setDay(int d) 
{
	day = d;
}
void Date::setHour(int h) 
{
	hour = h;
}
void Date::setMinute(int min) 
{
	minute = min;
}

int Date::getMonth() 
{
	return month;
}
int Date::getDay() 
{
	return day;
}
int Date::getHour() 
{
	return hour;
}

int Date::getMinute() 
{
	return minute;
}
