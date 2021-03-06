#include <iostream>


#include "gtest/gtest.h"


#include "./tests/string_test.h"
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
#include "./tests/materials_test.h"
#include "./tests/skp_reader_test.h"
#include "./tests/tasks_tests.h"
#include "./tests/location_tests.h"
#include "./tests/gencumulativesky_test.h"




int main(int argc, char* argv[]){	
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	
}



