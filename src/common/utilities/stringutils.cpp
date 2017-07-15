#include <string>

bool stringInclude(std::string word, std::string substring) {
	return word.find(substring) != -1;
}

bool stringInclude(char * word, std::string substring) {
	return std::string(word).find(substring) != -1;
}

bool stringInclude(char * word, char * substring) {
	return std::string(word).find(substring) != -1;
}


bool stringIncludeAny(std::string word, char ** substringArray, int nItems) {
	for (int i = 0; i < nItems; i++) {
		if (stringInclude(word, substringArray[i])) {
			return true;
		}
	}
	return false;
}


bool stringIncludeAny(char * word, char ** substringArray, int nItems) {
	return stringIncludeAny(std::string(word), substringArray, nItems);
}