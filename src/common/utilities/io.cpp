#include <string>
#include <iostream>
#include <sstream>

#include "./io.h"


std::string size_tToString(size_t sz) {
	std::stringstream ss;
	ss << sz;
	return ss.str();
}

void warn(std::string message) {	
	std::cerr << "Warning: " << message << std::endl;
}

void inform(std::string message, bool verbose) {
	if (!verbose) {
		return;
	}
	std::cout << "     ... " << message << std::endl;
}

void fatal(std::string message) {	
	std::cerr << "Fatal: " << message << std::endl;
}

