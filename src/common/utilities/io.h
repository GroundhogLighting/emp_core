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
void fatal(std::string message);

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