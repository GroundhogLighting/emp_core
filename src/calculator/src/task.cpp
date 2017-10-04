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


#include "./task.h"

Task::Task()
{

}

Task::~Task()
{
	
}

void Task::setName(std::string n)
{
	name = n;
}

void Task::setID(int i)
{
	id = i;
}

int Task::getID()
{
	return id;
}

std::string Task::getName()
{
	return name;
}

void Task::addDependency(Task * t)
{
	dependencies.push_back(t);
}


void Task::addDependencyIndex(size_t i)
{
	dependenciesIndexes.push_back(i);
}

void Task::addDependantIndex(size_t i)
{
	dependantsIndexes.push_back(i);
}

size_t Task::countDependencies()
{
	return dependencies.size();
}

Task * Task::getDependencyRef(size_t i)
{
	return dependencies[i];
}


size_t Task::getDependencyIndex(size_t i)
{
	return dependenciesIndexes[i];
}

size_t Task::getDependantIndex(size_t i)
{
	return dependantsIndexes[i];
}


size_t Task::countDependants()
{
	return dependantsIndexes.size();
}


bool Task::solve()
{
	return true;
}

bool Task::isEqual(Task * t)
{
	return true;
}