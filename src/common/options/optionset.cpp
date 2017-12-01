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

#include <typeinfo>

#include "api/src/common.h" //  for filling optionsets with LUA tables
#include "./optionset.h"
#include <fstream>

size_t OptionSet::size()
{
  return data.size();
}

bool OptionSet::hasOption(std::string opt)
{
  return (data.find(opt) != data.end());
}

bool OptionSet::isEqual(OptionSet * other)
{
  
  // return false if both do not have the same size
  if (data.size() != other->size())
    return false;

  for (json::iterator it = data.begin(); it != data.end(); ++it) {   

    std::string k = it.key();
    auto v = it.value();
    // If the option does not exists || it is different, return false
    if (!(other->hasOption(k)) || v != other->getOption<decltype(v)>(k)) {
      return false;      
    }
  }
  return true;
}


bool OptionSet::fillFromLuaTable(lua_State * L, int tablePosition)
{
  for (json::iterator it = data.begin(); it != data.end(); ++it) {

    std::string optionName = it.key();
    auto value = it.value();
    
    // Now the value we are looking for is in the position 2 of the stack
    int field = lua_getfield(L, 1, &optionName[0]);
    
    // If the value is there (i.e. it is not nil)
    if (field != LUA_TNIL) {
      // Retrieve it and use it      
      if (field == LUA_TNUMBER) {
        // Verify that the original value was a number as well
        if (value.is_number_integer()) {          
          setOption(optionName, (int)lua_tonumber(L,2));
        }
        else if (value.is_number() ){
          setOption(optionName, lua_tonumber(L, 2));
        }
        else {                   
          badOptionError(L, optionName, lua_typename(L, field));
        }
      }
      else if (field == LUA_TSTRING) {
        if (!value.is_string()) {
          badOptionError(L, optionName, lua_typename(L, field));
        }        
        setOption(optionName, lua_tostring(L, 2));
      }
      else if (field == LUA_TBOOLEAN ) {
        if (!value.is_boolean()) {
          badOptionError(L, optionName, lua_typename(L, field));
        }
        setOption(optionName, lua_toboolean(L, 2));
      }
      else {
        fatal("Unrecognized value type "+ std::string(lua_typename(L, field)) +" of value for option " + optionName,__LINE__,__FILE__);
        return false;
      }
    }
    // remove the value (or nil, if it was not there)
    lua_pop(L, 1);
  }
  return true;
}


void OptionSet::print(char * filename)
{
  std::ofstream file;
  if (filename != NULL) {
    file.open(filename);
  }

  for (json::iterator it = data.begin(); it != data.end(); ++it) {
    if (filename == NULL) {
     std::cout << "-" << it.key() << " " << it.value() << std::endl;
    }
    else {
      file << "-" << it.key() << " " << it.value() << std::endl;
    }
  }

  if (filename != NULL) {
    file.close();
  }
}