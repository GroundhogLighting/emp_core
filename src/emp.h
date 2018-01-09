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

#include "groundhogmodel/groundhogmodel.h"
#include "common/taskmanager/taskmanager.h"


//! This is the main class within Emp. It manages the Read, Write, Calculate process.


class Emp {

private:
	
	std::string inputFile; //!< The file that is being read

	std::string script; //!< The lua script to solve
	
	bool verbose = true; //!< An option that controls what sort of things will be "informed" or not
	
	GroundhogModel model = GroundhogModel(); //!< The Groundhog Model being handled

	TaskManager taskManager = TaskManager(); //!< The TaskManager that will handle and coordinate what needs to be done.

public:
	
	//! Parses the inputs provided through the command line
	/*!
	The first function to be called. Will return TRUE if all the inputs were
	consistent and valid. Will return FALSE in any other case.

	The inputs are basically the same as in the main() function.

	@author German Molina 
	@param[in] argc the number of arguments
	@param[in] argv arguments
	@return success 
	*/
	bool parseInputs(int argc, char* argv[]);
	

	//! Decides what needs to be done and call everything else to do it.
	/*! 
	This is the second and last function to be called by the main() function.
	From this function, all other functions are called.

	Will return TRUE if everything went well, and false in any other case.

	@author German Molina
	@return success
	@param[in] argc Number of arguments
	@param[in] argv The arguments given to the command line
	*/
	bool solve(int argc, char* argv[]);
	

	//! Loads the input file
	/*!
	Checks the file extension, creates the corresponding Reader and loads
	it as a GroundhogModel.

	@author German Molina
	@param[in] inputFile the file to be loaded
	@return success
	*/
	bool loadFile(std::string inputFile);

}; // END OF Emp CLASS