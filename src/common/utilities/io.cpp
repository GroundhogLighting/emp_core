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


#include <string>
#include <iostream>

#include "./io.h"



void warn(char * message) 
{	
	std::cerr << "Warning: " << message << std::endl;
}

void inform(char * message, bool verbose) 
{
	if (!verbose) {
		return;
	}
	std::cerr << "     ... " << message << std::endl;
}

void fatal(char * message, int ln, char * file) 
{	
	std::cerr << "Fatal: " << message << " -- Line " << ln << " of File " << file<< std::endl;
}



void warnNoMaterial(char * type, char * name)
{
  std::string errmsg = std::string(type)+ " object called " + std::string(name) + " has no material. It will be ignored";
  warn(&errmsg[0]);
}
