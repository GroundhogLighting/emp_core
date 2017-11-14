#pragma once

#include <string>


#ifdef WIN
#include <direct.h>
#define	POPEN(x) _popen(x,"r")
#define PCLOSE(x) _pclose(x)
#define CHDIR(x) _chdir(x)

#else
#include <unistd.h>
#define	POPEN(x) popen(x)
#define PCLOSE(x) pclose(x)
#define CHDIR(x) chdir(x)

#endif
