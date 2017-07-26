#pragma once 

#include <string>

bool stringInclude(std::string word, std::string substring);
bool stringInclude(char * word, std::string substring);
bool stringInclude(char * word, char * substring);
bool stringIncludeAny(std::string word, char ** substringArray, int nItems);
bool stringIncludeAny(char * word, char ** substringArray, int nItems);
void utf8toASCII(char * input, char * output, size_t * stringLength);
void fixString(char * s, size_t stringLength);