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


#include "./task.h"

Task::~Task()
{
    
}

void Task::setName(std::string * n)
{
	name = *n;
}


std::string * Task::getName()
{
	return &name;
}

void Task::addDependency(Task * t)
{  
  dependencies.push_back(t);
  t->addDependant(this);
}

void Task::addDependant(Task * t)
{
  dependants.push_back(t);
}


size_t Task::countDependencies()
{
	return dependencies.size();
}

Task * Task::getDependencyRef(size_t i)
{
	return dependencies[i];
}


Task * Task::getDependantRef(size_t i)
{
  return dependants[i];
}


size_t Task::countDependants()
{
  return dependants.size();
}

void Task::replaceDependency(Task * a, Task * b)
{
  size_t n = dependencies.size();
  for (size_t i = 0; i < n; i++) {
    if (dependencies[i] == a) {
      dependencies[i] = b;
      return;
    }
  }
}

