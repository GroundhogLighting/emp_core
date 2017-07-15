#include <string>
#include <iostream>

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cerr << "     >>> DEBUGG MSG: "  << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

void warn(std::string message);
void fatal(std::string message);
std::string size_tToString(size_t sz);
void inform(std::string message, bool verbose);