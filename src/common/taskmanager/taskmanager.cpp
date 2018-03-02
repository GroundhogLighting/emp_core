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


#include <fstream>
#include "./taskmanager.h"
#include "../utilities/io.h"
#include "tbb/tbb.h"

#ifdef _DEBUG
// Define mutex for informing progress
tbb::mutex verboseMutex;
#endif

TaskManager::TaskManager()
{

}

TaskManager::~TaskManager()
{
	// Delete all tasks
	for (size_t i = 0; i < tasks.size(); i++) {
		delete tasks[i];
	}
}

size_t TaskManager::addTask(Task * t)
{
	size_t currentIndex = tasks.size();

	// Check if the Task exists
	size_t n = currentIndex;
	for (size_t i = 0; i < n; i++) {
		if (compareTasks(tasks[i], t)) {
			// Task is redundant... 
                        
            // update dependants.            
            size_t nDependants = t->countDependants();
            for (size_t j = 0; j < nDependants; j++) {
              t->getDependantRef(j)->replaceDependency(t, tasks[i]);              
            }

            // update dependencies
            size_t nDependencies = t->countDependencies();
            for (size_t j = 0; j < nDependencies; j++) {
              t->getDependencyRef(j)->replaceDependency(t, tasks[i]);
            }
            
            //delete, 
            delete t;

            // return
			return i;
		}
	}

	// If not exist, add and return true
	tasks.push_back(t);

    // Check for mutex    
    for (size_t i = 0; i < currentIndex; i++) {
      if ( checkMutex(tasks[i],t) ) {
        tasks[i]->addDependency(t);          
      }
    }
    

	// Add the dependencies
	n = t->countDependencies();
	for (size_t i = 0; i < n; i++) {
		Task * dep = t->getDependencyRef(i);
		addTask(dep);
	}

	return currentIndex;
}

size_t TaskManager::countTasks()
{
	return tasks.size();
}


bool TaskManager::solve(json * results)
{

    if(tasks.size() == 0){
#ifndef AVOID_EMP_CORE_WARNINGS
        WARN(v,"No task to solve in TaskManager");
#endif
        return true;
    }
	// Create a vector to store all the nodes
	std::vector< tbb::flow::continue_node<tbb::flow::continue_msg> > nodes;
	nodes.reserve(tasks.size());
    
	// Create the graph
	tbb::flow::graph g;

	// Create the Start node
	tbb::flow::continue_node< tbb::flow::continue_msg> start(g,
		[](const tbb::flow::continue_msg &) {		
	});

	// Add all nodes
	for (size_t i = 0; i < tasks.size(); i++)
	{
		nodes.push_back(tbb::flow::continue_node<tbb::flow::continue_msg>(g, [=](const tbb::flow::continue_msg &) {
            bool success;
            try {
#ifdef _DEBUG
                verboseMutex.lock();
                std::cout << "    ... Starting Task '" << *(tasks[i]->getName()) << "'" << std::endl;
                verboseMutex.unlock();
#endif
                success= tasks[i]->solve();
#ifdef _DEBUG
                verboseMutex.lock();
                std::cout << "    ... Ended Task '" << *(tasks[i]->getName()) <<  "'" << std::endl;
                verboseMutex.unlock();
#endif
            }catch(std::out_of_range& ex) {
                std::cout << "Exception: " << ex.what() << std::endl;
            }
            
            return success;
		}));
	}

	// Connect nodes	
	for (size_t i = 0; i < tasks.size(); i++)
	{
		size_t nDependencies = tasks[i]->countDependencies();

		if (nDependencies == 0) {
			// Connect to source node
			tbb::flow::make_edge(start, nodes[i]);
		}
		else {
			for (size_t j = 0; j < nDependencies; j++) {
				size_t dep = findTask(tasks[i]->getDependencyRef(j));
				tbb::flow::make_edge(nodes[dep], nodes[i]);
			}
		}
	}

    // Solve!
    try {
        start.try_put(tbb::flow::continue_msg());
        g.wait_for_all();
    } catch(std::out_of_range& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
    
    if (results == nullptr)
      return true;

    // submit results
    for (size_t i = 0; i < tasks.size(); i++) {
      if (tasks.at(i)->generatesResults && tasks.at(i)->reportResults) {
        tasks.at(i)->submitResults(results);
      }
    }

	return true;
}


void TaskManager::print(char * filename)
{

  std::ofstream file;
  if (filename != nullptr) {
    file.open(filename);
  }

  
  if (filename == nullptr) {
    std::cout << "digraph {\n";
  }
  else {
    file << "digraph {\n";
  }
	
  for (size_t i = 0; i < tasks.size(); i++) {
	  Task * task = tasks[i];
      size_t nDep = task->countDependencies();
      if(nDep == 0){
          std::string ln = "\"" + *(task->getName()) + "\"";
          if (filename == nullptr) {
              std::cout << ln << ";\n";
          }
          else {
              file << ln << ";\n";
          }
          continue;
      }
      
      for (size_t j = 0; j < nDep; j++) {                   
        std::string ln = "\"" + *(task->getDependencyRef(j)->getName()) + "\" -> \"" + *(task->getName())+ "\"";
        if (filename == nullptr) {
          std::cout << ln << ";\n";
        }
        else {
          file << ln << ";\n";
        }    
      }		
  }
  if (filename == nullptr) {
    std::cout << "}\n";
  }
  else {
    file << "}\n";
  }
	
  
  if (filename != NULL) {
      file.close();
  }

}

bool TaskManager::solve()
{
    return solve(nullptr);
}

bool TaskManager::compareTasks(Task * a, Task * b)
{
	// Check if same class.
	if (typeid(*a) != typeid(*b))
		return false;

    // Check deeper
	return a->isEqual(b);
}


bool TaskManager::checkMutex(Task * a, Task * b)
{
    // Check if same class.
    if (typeid(*a) != typeid(*b))
        return false;
    
    // Check deeper
    return a->isMutex(b);
}


void TaskManager::clean()
{
  size_t nTasks = countTasks();
  for (size_t i = 0; i < nTasks; i++) {
    delete tasks[i];    
  }
  tasks.erase(tasks.begin(), tasks.end());
}


size_t TaskManager::findTask(Task * t)
{
  size_t n = tasks.size();
  for (size_t i = 0; i < n; i++) {
    if (tasks[i] == t)
      return i;
  }
  FATAL(err, "Task not found on TaskManager!");
  return -1;
}
