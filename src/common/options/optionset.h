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

#include <vector>
#include <string>

//! A base class for sets of Options

/*!
This class allows creating derived classes that represent a set of predefined
options and their default values.

For now it is assumed that every option is a double value.

@todo Allow String options
*/

class OptionSet {
private:
	std::vector<std::string> names = std::vector<std::string>(); //!< The names of the options registered
	std::vector<double> values = std::vector<double>(); //!< The values of the options in the same order as the names
	
public:

	//! Registers a new option in the OptionSet
	/*!
	After registering, this option may be searched easily. The idea
	is that this method is called by constructors of derived classes.

	@author German Molina
	@param opt The name of the option
	@param defaultValue The default value for such option
	*/
	void addOption(std::string opt, double defaultValue);

	//! Sets an option value
	/*!
	Modifies the value of an option. Returns True if the 
	option was found and False if not.

	@author German Molina
	@param opt The name of the option
	@param value The new value for the option
	*/
	bool setOption(std::string opt, double value);

	//! Retrieves an option value
	/*!
	Retrieves value of an option. Returns the value if found, 
	or -1 (plus a Fatal error message) if option is not found.

	@author German Molina
	@param opt The name of the option
	@return The value of the option
	*/
	double getOption(std::string opt);

	//! Retrieves an option value by index
	/*!
	Retrieves value of an option in a certain index. 
	Returns the value if the index is correct, or NULL 
	(plus a Fatal error message) if the index excedes 
	the length of the vector.

	@author German Molina
	@param opt The name of the option
	@return The value of the option
	*/
	double getOption(size_t i);


	//! Retrieves the name of an option
	/*!
	Retrieves name of an option in a certain index. 
	Returns the name if the index is correct, or NULL 
	(plus a Fatal error message) if the index excedes 
	the length of the vector.

	@author German Molina
	@param size_t The index of the option
	@return The name of the option
	*/
	std::string getOptionName(size_t i);

	//! Returns the number of options registered
	/*!
	@author German Molina
	@return The number of options
	*/
	size_t countOptions();

	//! Prints the options
	/*!
	@author German Molina
	@param flnm The name of the file to print the options to
	@todo The function is currently printing to the STDOUT
	*/
	void print(std::string flnm);
};
