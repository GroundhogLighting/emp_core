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

#include "./task.h"

//! The SimulationManager is an object that performs the calculation in an efficient way

/*!

The SimulationManager intends to perform all the calculations needed by the GroundhogModel
or user in a safer, efficient and easier way. In order to do that, certain characteristics
have been added to this object:

- It allows developers and users to define only the Task they want to solve (i.e. Calculate 
Daylight Autonomy) and it will define what needs to be done in between (which is called Expand)
- During expansion, the SimulationManager will only add the tasks that are not redundant (i.e.
calculating twice the Daylight Autonomy of a workplane)

*/


class SimulationManager {
private:
	std::vector <Task * > tasks = std::vector<Task *>(); //!< The Task objects to solve.

public:
	
	//! Constructor
	/*!
	Initializes a SimulationManager with zero Task objects

	@author German Molina	
	*/
	SimulationManager();

	//! Destructor
	/*!
	Deletes all added Task objects

	@author German Molina
	*/
	~SimulationManager();
	
	//! Adds a Tasks to the list 
	/*!
	Tries to add a task to the SimulationManager. If the Task is redundant,
	it will return the index of the equivalent Task. If not, it will return
	the index where the Task has been added (the last)

	@author German Molina
	@param[in] t The pointer to the Task to add.
	@return The index of the added Task
	*/
	size_t addTask(Task * t);

	//! Count Tasks
	/*!
	@author German Molina
	@return The number of Task objects to solve (includes NULL ones)
	*/
	size_t countTasks();

	//! Solve all the tasks
	/*!
	@param German molina
	*/
	bool solve();

	//! A Debug function...
	/*!
	@author German Molina
	*/
	void print();

	//! Compares two Task object
	/*!
	@author German Molina
	@param[in] a The pointer to Task number 1
	@param[in] b The pointer to Task number 2
	*/
	bool compareTasks(Task * a, Task * b);
};