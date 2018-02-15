#include <iostream>
//#include "./src/emp.h"

#include "gtest/gtest.h"

#include "./tests/loop_test.h" 
#include "./tests/point3d_test.h"
#include "./tests/vector3d_test.h"
#include "./tests/segment_test.h"
#include "./tests/triangulation_test.h"
#include "./tests/polygon_test.h"
#include "./tests/triangle_test.h" 
#include "./tests/taskManager_test.h"
#include "./tests/optionset_test.h"
#include "./tests/matrix_test.h"
#include "./tests/matrix4x4_test.h"
#include "./tests/reinhart_test.h"
#include "./tests/radiance_test.h"
#include "./tests/4cm_test.h"
#include "./tests/DDC_test.h"
#include "./tests/genperezskyvec_test.h"

#ifdef _DEBUG
#include <stdlib.h>
#include "debug_new.h"
#endif


int main(int argc, char* argv[]){	
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	
}



