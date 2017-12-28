﻿#include <gtest/gtest.h>
#include <iostream>
#include "./src/glare.h"

#include "./tests/loop_test.h" 
#include "./tests/point3d_test.h"
#include "./tests/vector3d_test.h"
#include "./tests/segment_test.h"
#include "./tests/triangulation_test.h"
#include "./tests/polygon_test.h"
#include "./tests/triangle_test.h" 
#include "./tests/taskManager_test.h"
#include "./tests/optionset_test.h"
#include "./tests/matrix4x4_test.h"
#include "./tests/radiance_test.h"

int main(int argc, char* argv[]){	
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	
}


