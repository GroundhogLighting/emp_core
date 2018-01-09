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

//! @file file.h

/*!
@defgroup file File utilities

This module defines functions that are handy for managing files and directories
*/

/* @{ */




//! Crates a directory
/*!
Checks if the directory already exists. If not, it will create it
and return TRUE.

@author German Molina (obtained from somewhere in the internet)
@param[in] dirname the name of the directory to create
@return success
*/
bool createdir(std::string dirname);

//! Checks if a file exists
/*!
@author German Molina (obtained from somewhere in the internet)
@param[in] filename the name of the file to check
@return exists
*/
bool fexists(std::string filename);

//! Checks if a directory exists
/*!
@author German Molina (obtained from somewhere in the internet)
@param[in] dirname the name of the directory to check
@return exists
*/
bool dexist(std::string dirname);

//! Checks if a directory is, in fact, a directory
/*!
@author German Molina (obtained from somewhere in the internet)
@param[in] dirname the name of the directory to check
@return it is a directory
*/
bool isDir(std::string dirname);



/* @} */

