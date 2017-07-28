#include <string>
#include <fstream>
#include <io.h>
#include <direct.h>

#include <sys/stat.h>
#include <sys/types.h>


#include "./file.h"
#include "./io.h"

bool createdir(std::string dirname) 
{
	if (dexist(dirname) && isDir(dirname)) {
		fatal("Impossible to create '"+dirname+"'. It already exists");
		return false;
	}
	else {
		_mkdir(dirname.c_str());
		return true;
	}
}

bool fexists(std::string filename)
{
	std::ifstream ifile(filename);
	return (bool)ifile;
}

bool dexist(std::string dirname)
{
	return _access(dirname.c_str(), 0) == 0;
}

bool isDir(std::string dirname) 
{
	if (_access(dirname.c_str(), 0) == 0) {
		struct stat status;
		stat(dirname.c_str(), &status);
		if (status.st_mode & S_IFDIR) {
			return true;
		}
		else {			
			return false;
		}
	}
	else {
		fatal("Directory does not exist");
		return false;
	}
}