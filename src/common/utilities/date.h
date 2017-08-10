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

//! Represents a Date object

/*! This object contains a Month, Day, Hour and Minute
*/

class Date{

private:
    int month; //!< The month
    int day; //!< The day
    int hour; //!< The hour
    int minute; //!< The minute

public:

	//! Creates a new Date object.
	/*!
	Assigns month, day, hour and minute values to the month, day, hour and minute members respectively

	@author German Molina
	@param[in] month the month 
	@param[in] day the day 
	@param[in] hour the hour
	@param[in] min the minute
	*/
	Date(int month, int day, int hour, int min);

	//! Creates a new Date object from the epoch.
	/*!
	Creates a new time object, and extracts from it the apropriate data.

	@author German Molina
	@param[in] t the number of seconds from 1 January 1970 00:00:00
	*/
	Date(int64_t t);

	//! Destroys the Date object
	/*!
	@author German Molina
	*/
	~Date();

	//! Sets the month
	/*!
	@author German Molina
	@param[in] m the month
	*/
	void setMonth(int m);
	
	//! Sets the day
	/*!
	@author German Molina
	@param[in] d the day
	*/
	void setDay(int d);

	//! Sets the month
	/*!
	@author German Molina
	@param[in] h the hour
	*/
	void setHour(int h);

	//! Sets the minute
	/*!
	@author German Molina
	@param[in] min the minute
	*/
	void setMinute(int min);

	//! Retrieves the month
	/*!
	@author German Molina
	@return the month
	*/
	int getMonth();

	//! Retrieves the day
	/*!
	@author German Molina
	@return the day
	*/
	int getDay();

	//! Retrieves the hour
	/*!
	@author German Molina
	@return the hour
	*/
	int getHour();

	//! Retrieves the minute
	/*!
	@author German Molina
	@return the minute
	*/
	int getMinute();
};