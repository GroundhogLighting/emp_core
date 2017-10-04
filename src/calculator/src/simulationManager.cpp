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

#include "simulationManager.h"
#include "common/utilities/io.h"

SimulationManager::SimulationManager()
{

}


SimulationManager::~SimulationManager()
{
	// Delete all tasks
	for (size_t i = 0; i < tasks.size(); i++) {
		delete tasks[i];
	}
}


size_t SimulationManager::addTask(Task * t)
{
	size_t currentIndex = tasks.size();
	
	// Check if the Task exists
	size_t n = tasks.size();
	for (size_t i = 0; i < n; i++) {	
		if (compareTasks(tasks[i], t)) {			
			// Task is redundant... delete, add dependand and return
			delete t;		
			return i;
		}
	}

	// If not exist, add and return true
	tasks.push_back(t);
	
	// Add the dependencies
	n = t->countDependencies();
	for (size_t i = 0; i < n; i++) {
		Task * dep = t->getDependencyRef(i);		
		size_t added = addTask(dep);
		t->addDependencyIndex( added );
		tasks[added]->addDependantIndex(currentIndex);
		
	}

	return currentIndex;
}

size_t SimulationManager::countTasks()
{
	return tasks.size();
}


bool SimulationManager::solve()
{
	return true;
}


void SimulationManager::print()
{
	std::cout << "==================== " << std::endl;
	std::cout << "[ ";
	for (size_t i = 0; i < tasks.size(); i++) {
		Task * task = tasks[i];
		std::cout << task->getName() << "  ";
	}
	std::cout << " ]" << std::endl;

	for (size_t i = 0; i < tasks.size(); i++) {
		Task * task = tasks[i];

		std::cout << task->getName() << "  " << i  <<  std::endl;

		std::cout << " ... dependencies: ";
		for (size_t j = 0; j < task->countDependencies(); j++) {
			std::cout << task->getDependencyIndex(j) << " , ";
		}
		std::cout << std::endl;

		std::cout << " ... dependants: ";
		for (size_t j = 0; j < task->countDependants(); j++) {
			std::cout << task->getDependantIndex(j) << " , ";
		}
		std::cout << std::endl << std::endl;
	}

	std::cout << "==================== " << std::endl;
}

bool SimulationManager::compareTasks(Task * a, Task * b)
{
	// Check if same class.
	if (a->getID() != b->getID())
		return false;

	return a->isEqual(b);	
}