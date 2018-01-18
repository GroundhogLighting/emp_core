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
#include "versions.h"

/*!
@file config_constants.h
*/

/*!
@defgroup configConstants Configuration constants

This module defines constants that are used in the rest of the program.
*/

/* @{ */

/// The emppath to store as environmental variable; where the lua scripts are stored
#define EMPATH "EMPATH"

/// The 'usage' message
#define USAGE "\n " << EMP_VERSION << " usage options: \n\n emp {--help|-h}\t\t: prints this message \n emp {--version|-v}\t\t: prints version \n emp {--about|-a}\t\t: prints about \n emp --checkpath\t\t: prints the GLAREPATH variable\n emp inputFile script.lua\t: applies a lua script over an input file "

/// The 'about' message
#define ABOUT "\n\n Copyright (C) 2017 German Molina (germolinal@gmail.com) \n\n This program is free software : you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. \n\n This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details. \n\n You should have received a copy of the GNU General Public License along with this program.If not, see http ://www.gnu.org/licenses/."

/// The separator used when writing files
#define EMP_TAB "\t" //!< This is the separator used when writing Radiance files

/// Maximum interior loops
#define EMP_TOO_MANY_LOOPS 40 //!< The number of interior loops that are considered too many in a face 

/// Huge number
#define EMP_HUGE 9e9 //!< This is a huge number that may be used by several sections of the program

/// Minus huge number
#define EMP_MINUS_HUGE -EMP_HUGE //!< This is a negative huge number that may be used by several sections of the program

/// Tiny number
#define EMP_TINY 1e-9 //!< This is a very small number that may be used by several sections of the program

/// Minus tiny number
#define EMP_MINUS_TINY -EMP_TINY //!< This is a very small number that may be used by several sections of the program

/// Maximum number of points in a triangulation before refining
#define EMP_MAX_POINTS_IN_WORKPLANE 1000 //!< This number is required on the poly2tri function

/// Name of the global variable pointing to the GroundhogModel in the Lua API
#define LUA_MODEL_VARIABLE "currentmodel"

/// Name of the global variable pointing to the TaskManager in the Lua API
#define LUA_TASKMANAGER_VARIABLE "currenttaskmanager"

/// Name of the global variable pointing to the TaskDictionary in the Lua API
#define LUA_TASKDICTIONARY_VARIABLE "currenttaskdictionary"

/// Name of the variable defining the TaskManager's autoSolve option
#define LUA_AUTOSOLVE_VARIABLE "auto_solve"

/// Name of the directory where we export the model before solving
//#define EMP_TMP_DIR "tmp"

/// The sky and ground hemispheres needed for completing a sky definition
#define RADIANCE_SKY_COMPLEMENT "skyfunc glow skyglow 0 0 4 1 1 1 0 skyglow source skyball 0 0 4 0 0 1 360\n\n"

/* @} */
