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


#include "config_constants.h"
#include "groundhogmodel/groundhogmodel.h"

#include "./api/src/common.h"
#include "./options.h"

#include <iostream>

int print_rtrace_options(lua_State * L)
{
	GroundhogModel * model = getCurrentModel(L);
	RTraceOptions * options = model->getRTraceOptions();
	options->print("aa");
	return 0;	
}

int set_rtrace_options(lua_State *L)
{	
	GroundhogModel * model = getCurrentModel(L);
	RTraceOptions * options = model->getRTraceOptions();
	
	// Check nuber of arguments
	if (!checkNArguments(L, 1)) {
		return 0;
	}

	// Check type
	if (lua_type(L, 1) != LUA_TTABLE){
		std::cerr << "Not a table " << std::endl;
		return 0;
	}
	
	size_t j = options->countOptions();
	

	for (size_t i = 0; i < j; i++) {
		std::string optionName = options->getOptionName(i);
		if (lua_getfield(L, 1,&optionName[0]) != LUA_TNIL) {			
			double value = luaL_checknumber(L, 2);
			options->setOption(optionName, value);
		}
		lua_pop(L, 1);
	}

	return 0;
	
}