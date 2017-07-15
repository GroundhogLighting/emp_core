#include <string>

#include "./src/glare.h"
#include "./main.h"




int main(int argc, char* argv[]){	
	
	Glare glare;

	if (!glare.parseInputs(argc, argv)) {
		return 1;
	}

	if (!glare.solve()) {
		return 1;
	}

	return 0;
}



