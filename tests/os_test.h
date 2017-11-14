/*os_test.h*/

#include "common/utilities/os.h"

TEST(OSTests, echo)
{

	// define keyword
	std::string c = "AUTO";

	// run command
	FILE * out;
	if (getStdout("echo " + c, &out)) {
		std::cout << "Worked!" << std::endl;
	}

	// get output
	char line[200];
	fgets(line, 200, out);	
	_pclose(out);

	ASSERT_TRUE(c.compare(line));
}
