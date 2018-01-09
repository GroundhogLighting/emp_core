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

#include "./api_io.h"

int warn(lua_State * L)
{
	lua_Debug ar;
	lua_getstack(L, 1, &ar);
	lua_getinfo(L, "nSl", &ar);

	const char * msg = lua_tostring(L, 1);
	std::cerr << "Warning from Line " << ar.currentline << ": " << msg << std::endl;
	
	return 0;
}

int raise(lua_State * L)
{	
	const char * msg = lua_tostring(L, 1);		
	sendError(L, "Usage", msg);	
	return 0;
}