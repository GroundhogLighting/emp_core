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

#include <vector>
#include <string>
#include "common/options/optionset.h"


//! A Task is an instruction or set of instructions that is required to perform a simulation

/*!
The Task class is a base class for several specific Task objects (i.e. every specific Task will 
be derived from this class).

Every task is dependent on one or more Task objects, which will need to be solved before 
this one. Similarly, each Task is aware of which other Task objects depend upon it...
these, however, will be filled by the SimulationManager when expanded.

It should be noticed that each class derived from Task will need to define (if needed)
a set of results (i.e. data generated while solving it) and a set of targets (i.e.
a CalculateAnnualDaylight Task is generic, but targeted to different workplanes).

*/

class Task {
private:
	
	std::string name = "ThisTaskHasNoName"; //!< The name of the task
	std::vector<Task *> dependencies = std::vector<Task * >(); //!< The vector of Task objects that this Task depend upon
    std::vector<Task *> dependants = std::vector<Task * >(); //!< The vector of Task objects that depend on this Task

public:
    bool reportResults = false; //!< True if the TaskManager should report the results or not
    bool generatesResults = false; //!< Indicates whether or not this task is worth reporting results
	
	//! Assigns a name to the Task
	/*!
	@author German Molina
	@param[in] name The name to assign
	*/
	void setName(std::string * name);

	
	//! Retrieves the name of the Task
	/*!
	@author German Molina
	@return The name
	*/
	std::string * getName();

	//! Adds a dependency to the Task
	/*!
	@author German Molina
	@param[in] t The pointer to the Task to add	
	*/
	void addDependency(Task * t);

    //! Adds a dependant Task
    /*!
    @author German Molina
    @param[in] t The pointer to the Task to add
    */
    void addDependant(Task * t);


	//! Retrieves a dependency pointer
	/*!
	@author German Molina
	@param[in] i The index of the dependency to get
	@return The pointer to the dependency
	*/
	Task * getDependencyRef(size_t i);
    
    //! Retrieves a dependant pointer
    /*!
    @author German Molina
    @param[in] i The index of the dependency to get
    @return The pointer to the dependency
    */
    Task * getDependantRef(size_t i);

	//! Counts the number of dependants in a Task
	/*!
	@author German Molina
	@return The number
	*/
	size_t countDependants();

	//! Gets the number of dependencies
	/*!
	@author German Molina
	@return The number of dependencies
	*/
	size_t countDependencies();

    //! Replaces one dependency by another one.
    /*!
    @author German Molina
    @param[in] a The dependency to be replaced
    @param[in] b The dependency to replace 'a' with
    */
    void replaceDependency(Task * a, Task * b);

	//! Function that performs the instructions required
	/*!
	Each derived Task will solve some instructions and generate some data,
	which will be called using this function.

	@author German Molina
	@return success
	*/
	virtual bool solve() = 0;

	//! Function that compares two Task objects
	/*!
	This function compares whether a Task is equal to another one.
	That is, solving it would end in redundant calculations or 
	actions (i.e. calculating the Daylight Autonomy twice for a
	certain workplane). 
	
	This is a virtual function because the targets will depend on
	each derived Task.
	
	@author German Molina
	@param[in] t The pointer to the other Task
	@return is equal?
	*/
	virtual bool isEqual(Task * t) = 0;

    //! Checks if two tasks are compatible to run in parallel
    /*!
    If two Tasks are defined as Mutex, they cannot run in parallel; thus
    the task manager will put a dependency to one another
    
    @author German Molina
	@param[in] t The pointer to the other Task
	@return is mutex?
    */
    virtual bool isMutex(Task * t) = 0;

    //! Adds the Task reuslts to a result JSON
    /*!
    @author German Molina
    @param[in] results The results JSON to fill
    */
    virtual bool submitResults(json * results) = 0;
    
};


using TaskFactory = Task* (*)(lua_State *L);


