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
