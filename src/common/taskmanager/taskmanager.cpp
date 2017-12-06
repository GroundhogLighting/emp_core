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


#include <fstream>
#include "taskManager.h"
#include "common/utilities/io.h"
#include "tbb/tbb.h"

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
		t->addDependencyIndex(added);
		tasks[added]->addDependantIndex(currentIndex);

	}

	return currentIndex;
}

size_t TaskManager::countTasks()
{
	return tasks.size();
}


bool TaskManager::solve()
{


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
			return tasks[i]->solve();
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
				size_t dep = tasks[i]->getDependencyIndex(j);
				tbb::flow::make_edge(nodes[dep], nodes[i]);
			}
		}
	}

	start.try_put(tbb::flow::continue_msg());
	g.wait_for_all();

	return true;
}


void TaskManager::print(char * filename)
{

  std::ofstream file;
  if (filename != NULL) {
    file.open(filename);
  }

  
  if (filename == NULL) {
    std::cout << "digraph {\n";
  }
  else {
    file << "digraph {\n";
  }
	
  for (size_t i = 0; i < tasks.size(); i++) {
	  Task * task = tasks[i];
      size_t nDep = task->countDependencies();
      for (size_t j = 0; j < nDep; j++) {                   
        std::string ln = "\"" + *(task->getDependencyRef(j)->getName()) + "\" -> \"" + *(task->getName())+ "\"";
        if (filename == NULL) {
          std::cout << ln << ";\n";
        }
        else {
          file << ln << ";\n";
        }    
      }		
  }
  if (filename == NULL) {
    std::cout << "}\n";
  }
  else {
    file << "}\n";
  }
	
  
  if (filename != NULL) {
    file.close();
  }

}

bool TaskManager::compareTasks(Task * a, Task * b)
{
	// Check if same class.
	if (typeid(*a) != typeid(*b))
		return false;

	return a->isEqual(b);
}

void TaskManager::clean()
{
  size_t nTasks = countTasks();
  for (size_t i = 0; i < nTasks; i++) {
    delete tasks[i];    
  }
  tasks.erase(tasks.begin(), tasks.end());
}
