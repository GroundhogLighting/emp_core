#pragma once

#include <string>


#ifdef WIN
#include <direct.h>
#define	POPEN(x,y) _popen(x,y)
#define PCLOSE(x) _pclose(x)
#define CHDIR(x) _chdir(x)

#else
#include <unistd.h>
#define	POPEN(x,y) popen(x,y)
#define PCLOSE(x) pclose(x)
#define CHDIR(x) chdir(x)

#endif
