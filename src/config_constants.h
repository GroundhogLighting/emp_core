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

/*!
@file config_constants.h
*/

/*!
@defgroup configConstants Configuration constants

This module defines constants that are used in the rest of the program.
*/

/* @{ */

/// Maximum interior loops
#define TOO_MANY_LOOPS 40 //!< The number of interior loops that are considered too many in a face
 
/// Maximum string length
#define MAX_STRING_LENGTH 100 //!< The number of characters that the char * are allowed to have by default

/// Huge number
#define HUGE 9e9 //!< This is a huge number that may be used by several sections of the program

/// Minus huge number
#define MINUS_HUGE -HUGE //!< This is a negative huge number that may be used by several sections of the program

/// Tiny number
#define TINY 1e-9 //!< This is a very small number that may be used by several sections of the program

/// Minus tiny number
#define MINUS_TINY -TINY //!< This is a very small number that may be used by several sections of the program

/// Maximum number of points in a triangulation before refining
#define MAX_POINTS_IN_WORKPLANE 10000 //!< This number is required on the poly2tri function


/* @} */