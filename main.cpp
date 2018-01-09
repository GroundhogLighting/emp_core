#include <string>
#include "config_constants.h"
#include "versions.h"
#include "./src/emp.h"
#include "./main.h"
#include <iostream>
#include "Lua/src/lua.hpp"

#include "mem_leak.h"

#ifdef DEBUG
  //MemAllocations memtracker = MemAllocations();
#endif

int main(int argc, char* argv[]){	
	
	// Check if only some help is needed
	if (argc == 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1] , "-h")== 0) {
		std::cout << USAGE << std::endl;		
	}
	else if (strcmp(argv[1],"--about") == 0 || strcmp(argv[1],"-a") == 0) {
		std::cout << ABOUT << std::endl;		
	}
	else if (strcmp(argv[1],"--version") == 0 || strcmp(argv[1],"-v") == 0) {
		std::cout << EMP_VERSION << std::endl;
		std::cout << RADIANCE_VERSION << std::endl;
		std::cout << LUA_VERSION << std::endl;		
    }
    else if (strcmp(argv[1], "--checkpath") == 0 ) {
      if (const char * glarepath = std::getenv(EMPATH)) {
        std::cout << glarepath << std::endl;
      }
      else {
        std::cout << "There is no "<< EMPATH << " variable stored" << std::endl;
      }
    }
    else if (argc < 3) {
      FATAL(errmsg, "Incorrect glare usage\n");
      std::cout << USAGE << std::endl;
    }
    else {


	  Emp emp;

	  if (!emp.parseInputs(argc, argv)) {
		  return 1;
	  }

	  if (!emp.solve(argc, argv)) {
		  return 1;
	  }

  #ifdef DEBUG
      //memtracker.checkMemoryLeaks();
  #endif

    }
	return 0;
}



