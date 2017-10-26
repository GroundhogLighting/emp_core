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

#include "groundhogmodel/groundhogmodel.h"
#include "common/taskmanager/taskmanager.h"

//! This is the main class within Glare. It manages the Read, Write, Calculate process.


class Glare {

private:

	const std::string usage =	"usage v1: Glare inputFile outFile --> exports file\n"
								"usage v2: Glare inputFile --> performs standard calculation \n"
								"usage v3: Glare inputFile.lua --> performs custom calculations\n"; /*!< This is the string representing the usage intructions.*/
	
	std::string inputFile; //!< The file that is being read

	std::string secondArgument; //!< The second argument given to the program. It may be a Lua scrip, a name of a new model or a specific task
	
	bool verbose = true; //!< An option that controls what sort of things will be "informed" or not
	
	GroundhogModel model = GroundhogModel(); //!< The Groundhog Model being handled

	TaskManager taskManager = TaskManager(); //!< The TaskManager that will handle and coordinate what needs to be done.

public:
	//! Creates a Glare object.
	/*! 
	Defaults the 'verbose' option to TRUE, and model to an empty GroundhogModel
	*/
	Glare();
	
	//! Destroys the Glare object
	/*! 
	Deletes all its members as well. Each member should delete its own members (i.e. the model deletes
	layers, views, etc.)

	@author German Molina
	*/
	~Glare();
	
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
	*/
	bool solve();
	

	//! Loads the input file
	/*!
	Checks the file extension, creates the corresponding Reader and loads
	it as a GroundhogModel.

	@author German Molina
	@param[in] inputFile the file to be loaded
	@return success
	*/
	bool loadFile(std::string inputFile);

}; // END OF GLARE CLASS