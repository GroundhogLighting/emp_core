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

#include "./common.h"
#include <iostream>

GroundhogModel * getCurrentModel(lua_State * L)
{
	lua_getglobal(L, LUA_MODEL_VARIABLE);
	GroundhogModel * model = (GroundhogModel *)lua_touserdata(L, lua_gettop(L));
	lua_pop(L, 1);
	return model;
}

TaskManager * getCurrentTaskManager(lua_State * L)
{
	lua_getglobal(L, LUA_TASKMANAGER_VARIABLE);
	TaskManager * taskManager = (TaskManager *)lua_touserdata(L, lua_gettop(L));
	lua_pop(L, 1);
	return taskManager;
}


std::map<std::string, TaskFactory> * getCurrentTaskDictionary(lua_State * L)
{
  lua_getglobal(L, LUA_TASKDICTIONARY_VARIABLE);
  std::map<std::string,TaskFactory> * taskDictionary = (std::map<std::string, TaskFactory> *)lua_touserdata(L, lua_gettop(L));
  lua_pop(L, 1);
  return taskDictionary;
}

void checkNArguments(lua_State * L, int nRArgs)
{
	int nargs = lua_gettop(L);
	if (nargs != nRArgs) 
		nArgumentError(L, nRArgs);			
}


int checkNArguments(lua_State * L, int nArgs[],int length)
{
  int givenArgs = lua_gettop(L);
  for (int i = 0; i < length; i++) {
    if (givenArgs == nArgs[i])
      return givenArgs;
  }
  nArgumentError(L, nArgs, length);
  return -1;
}

void nArgumentError(lua_State * L, int nArgs[], int length)
{
  std::string allowable = "";
  for (int i = 0; i < length; i++) {
    std::string comma = i==(length-2) ? " or " : ", ";    
    if (i == length - 1)
      comma = "";

    allowable = allowable + std::to_string(nArgs[i]) + comma;
  }
  std::string errmsg = "Function allows receiving " + allowable + " arguments, but received " + std::to_string(lua_gettop(L));
  sendError(L, "Number of arguments", &errmsg[0]);
}


void nArgumentError(lua_State * L, int nRequiredArgs)
{
	std::string errmsg = "Function requires " + std::to_string(nRequiredArgs) + " but received " + std::to_string(lua_gettop(L));
	sendError(L, "Number of arguments", &errmsg[0]);
}

void checkArgType(lua_State * L, int expectedType, int argPoisition)
{
  if (lua_type(L, argPoisition) != expectedType)
    argTypeError(L, expectedType, argPoisition);
}


void argTypeError(lua_State * L, int expectedType, int argPoisition)
{
  std::string errmsg = "Argument "+std::to_string(argPoisition)+" should be a " + std::string(lua_typename(L,expectedType)) + " but a "+std::string(lua_typename(L,lua_type(L,argPoisition)))+" was received";
  sendError(L, "Argument type", &errmsg[0]);
}


void executionError(lua_State * L, const char * err)
{
  sendError(L, "Execition error", err);
}

void sendError(lua_State * L, const char * kind, const char * err)
{
	lua_Debug ar;
	lua_getstack(L, 1, &ar);
	lua_getinfo(L, "nSl", &ar);

	std::string errMsg = "'"+std::string(kind) + "' error in line " + std::to_string(ar.currentline) + ": " + std::string(err);
	lua_pushstring(L, &errMsg[0]);
	lua_error(L);
}


void badOptionError(lua_State * L, std::string optionName, const char * receivedType, const char * expectedType)
{
  std::string err = "Option '" + optionName + "' is not supposed to be a '"+ receivedType + "' but a '"+expectedType+"'";
  sendError(L, "Bad option type", &err[0]);
}

Workplane * getWorkplane(lua_State * L, std::string workplaneName)
{
  GroundhogModel * model = getCurrentModel(L);
  Workplane * wp = model->getWorkplaneByName(workplaneName);

  if (wp == nullptr) {
    std::string errmsg = "Workplane '" + workplaneName + "' does not exist";
    sendError(L, "No Workplane", &errmsg[0]);
  }

  return wp;
}


void missingOption(lua_State * L, std::string optionName, std::string optionType)
{
  std::string errmsg = "Required option '"+optionName+"' of type '"+optionType+"' was not given";
  sendError(L,"Missing option", &errmsg[0]);
}