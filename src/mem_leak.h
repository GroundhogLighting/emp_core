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

#pragma once
/*
#ifdef DEBUG

#include <vector>
#include <iostream>
#include <string>




class MemAllocations {
public:  
  std::vector<void*> pointers = std::vector<void *>();
  std::vector<int> lineNumbers = std::vector<int>();
  std::vector<const char *> fileNames = std::vector<const char *>();

  void addAllocation(void * p, int line, const char * file)
  {
    pointers.push_back(p);
    lineNumbers.push_back(line);
    fileNames.push_back(file);
  }

  bool checkMemoryLeaks()
  {
    std::string l = "================================\n";
    
    // Print start
    std::cerr << l;
    std::cerr << "\tVERIFYING MEMORY LEAKAGE" << std::endl;

    size_t n = pointers.size();

    std::cerr << "\t..." << n << " registered allocations ..." << std::endl << std::endl;
    size_t leaks = 0;
    
    for (size_t i = 0; i < n; i++) {      
      if (pointers[i] != NULL) {        
        std::cerr << "\t... Memory leak detected: Created on line " << lineNumbers[i] << " of file " << fileNames[i] << std::endl;
        
        leaks++;
      }      
    }

    std::cerr << l;
    return leaks == 0;
  }
};


extern MemAllocations memtracker; 

#define DEBUG_NEW(ptr,x) new x; memtracker.addAllocation(ptr,__LINE__,__FILE__) 

#else
#define debug_new(var,x) new x
#endif 
*/
