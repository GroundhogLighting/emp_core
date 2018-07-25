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


#ifdef WIN
#define	POPEN(x,y) _popen(x,y)
#define PCLOSE(x) _pclose(x)
#define FOPEN(varname,filename,mode) FILE * varname; fopen_s(&varname,filename,mode)
#define FSCANF fscanf_s
#define MKDIR(x) _mkdir(x)
#define ACCESS(x,y) _access(x,y)
#else
#include <unistd.h>
#include <sys/stat.h>
#define	POPEN(x,y) popen(x,y)
#define PCLOSE(x) pclose(x)
#define FOPEN(varname,filename,mode) FILE * varname = fopen(filename,mode)
#define FSCANF fscanf
#define MKDIR(x) mkdir(x,0777)
#define ACCESS(x,y) access(x,y)
#endif
