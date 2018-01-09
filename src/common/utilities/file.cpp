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


#include <string>
#include <fstream>
#include <io.h>
#include <direct.h>

#include <sys/stat.h>
#include <sys/types.h>


#include "./file.h"
#include "./io.h"

bool createdir(std::string dirname) 
{
	if (dexist(dirname) && isDir(dirname)) {
		//FATAL(errorMessage,"Impossible to create '"+dirname+"'. It already exists");
		return true;
	}
	else {
		_mkdir(dirname.c_str());
		return true;
	}
}

bool fexists(std::string filename)
{
	std::ifstream ifile(filename);
	return (bool)ifile;
}

bool dexist(std::string dirname)
{
	return _access(dirname.c_str(), 0) == 0;
}

bool isDir(std::string dirname) 
{
	if (_access(dirname.c_str(), 0) == 0) {
		struct stat status;
		stat(dirname.c_str(), &status);
		if (status.st_mode & S_IFDIR) {
			return true;
		}
		else {			
			return false;
		}
	}
	else {
		FATAL(errorMessage,"Directory does not exist");
		return false;
	}
}