#include <iostream>
#include <gtest/gtest.h>
#include "./src/glare.h"

#include "./tests/loop_test.h"
#include "./tests/point3d_test.h"
#include "./tests/vector3d_test.h"
#include "./tests/segment_test.h"
#include "./tests/triangulate_test.h"

int main(int argc, char* argv[]){	
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	
}



