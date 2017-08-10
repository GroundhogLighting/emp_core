/* loop_test.h */
#include "../src/common/geometry/loop.h"


TEST(LoopTests, emptyVectorOnConstructor)
{
	Loop loop = Loop();
	ASSERT_EQ(0, loop.size());
}

TEST(LoopTests, addVertex)
{
	Loop loop = Loop();
	loop.addVertex(new Point3D(0, 0, 0));
	ASSERT_EQ(1, loop.size());
}