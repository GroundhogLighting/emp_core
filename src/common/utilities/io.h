/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

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

//! A debug message function. Only reports in _DEBUG releases
/*!
@author German Molina (based on information on the internet)
*/
#ifdef _DEBUG
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
extern void warn(const char * message);

#define WARN(var,x) std::string var=x; warn(&var[0])

//! Report fatal error
/*!
Sends a message to the Standard Error, appending 'Fatal: ' before it.

Should be used when something fatal happens. Usually, something will go wrong.

@author German Molina
@param[in] message the message
@param[in] ln The line number where this function was called
@param[in] file The file name where this function was called
*/
extern void fatal(const char * message, int ln, const char * file);
#define FATAL(var,x) {std::string var=std::string(x); fatal(&var[0],__LINE__,__FILE__);};

//! Informs something
/*!
Sends a message to the Standard Output, appending '     ...' before it.

Should be used for informing progress and things that are certainly not errors.

@author German Molina
@param[in] message the message
@param[in] verbose Should we inform?
*/
extern void inform(const char * message, bool verbose);

#define INFORM(var,x,y) std::string var=x; inform(&var[0],y)

//! Warns that a certain window, or Otype object has no material
/*!
@author German Molina
@param[in] type The type of surface (e.g. window or other)
@param[in] name The name of the object
*/
extern void warnNoMaterial(const char * type , char * name);

/* @} */
