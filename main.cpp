#include <string>

#include "config_constants.h"
#include "versions.h"

#include "./src/glare.h"
#include "./main.h"

#include <iostream>


// Include LUA headers
extern "C" {
#include <lua.h>
	//#include <lualib.h>
#include <lauxlib.h> 
}



int main(int argc, char* argv[]){	
	
	// Check if only some help is needed
	if (argc == 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1] , "-h")== 0) {
		std::cout << USAGE << std::endl;
		return 0;
	}
	else if (strcmp(argv[1],"--about") == 0 || strcmp(argv[1],"-a") == 0) {
		std::cout << ABOUT << std::endl;
		return 0;
	}
	else if (strcmp(argv[1],"--version") == 0 || strcmp(argv[1],"-v") == 0) {
		std::cout << GLARE_VERSION << std::endl;
		std::cout << RADIANCE_VERSION << std::endl;
		std::cout << LUA_VERSION << std::endl;
		return 0;
	}

	Glare glare;

	if (!glare.parseInputs(argc, argv)) {
		return 1;
	}

	if (!glare.solve(argc, argv)) {
		return 1;
	}

	return 0;
}



