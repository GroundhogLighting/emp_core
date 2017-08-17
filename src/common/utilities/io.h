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


#pragma once

#include <string>
#include <iostream>

/*!
@defgroup ioUtils I/O utilities

This module defines functions that are used in the rest of the program to inform, report or warn about things
*/

/* @{ */

//! A debug message function. Only reports in DEBUG releases
/*!
@author German Molina (based on information on the internet)
*/
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cerr << "     >>> DEBUGG MSG: "  << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

//! Warn something
/*!
Sends a message to the Standard Error, appending 'Warning: ' before it.

Should be used when something weird or worth informing has happened

@author German Molina
@param[in] message the message
*/
void warn(std::string message);

//! Report fatal error
/*!
Sends a message to the Standard Error, appending 'Fatal: ' before it.

Should be used when something fatal happens. Usually, something will go wrong.

@author German Molina
@param[in] message the message
*/
void fatal(std::string message, int ln, char * file);

//! Informs something
/*!
Sends a message to the Standard Output, appending '     ...' before it.

Should be used for informing progress and things that are certainly not errors.

@author German Molina
@param[in] message the message
@param[in] verbose Should we inform?
*/
void inform(std::string message, bool verbose);

/* @} */