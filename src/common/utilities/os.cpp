#include "./os.h"
#include <stdio.h>

#include <iostream>

bool getStdout(std::string c, FILE ** out)
{

#ifdef WIN			
	return((*out = _popen(&c[0], "r")) != NULL);
#endif

	return false;
}