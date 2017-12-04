#pragma once


#ifdef WIN
#define	POPEN(x,y) _popen(x,y)
#define PCLOSE(x) _pclose(x)
#define FOPEN(varname,filename,mode) FILE * varname; fopen_s(&varname,filename,mode)

#else
#include <unistd.h>
#define	POPEN(x,y) popen(x,y)
#define PCLOSE(x) pclose(x)
#define FOPEN(varname,filename,mode) FILE * varname = fopen(filename,mode)

#endif