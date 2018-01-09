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


#include <iostream>

//! Prints the rtrace options in a file or stdout
/*
@author German Molina
@param[in] L The lua state
@return the number of return values within the script
*/
int print_rtrace_options(lua_State * L);

//! Sets options for RTRACE routines of the model.
/*!
This function will retrieve the current GroundhogModel and
modify its RTRACE options

@author German Molina
@param L The lua_State * object
@return The number of variables in the lua stack
*/
int set_rtrace_options(lua_State *L);