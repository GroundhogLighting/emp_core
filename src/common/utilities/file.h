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
and return TRUE. Else, returns FALSE.

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

