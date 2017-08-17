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


TEST(LoopTests, testPointConvexLoopInterior)
{
	Vector3D normal = Vector3D(0, 0, 1);
	Loop loop = Loop();
	double l = 1 / sqrt(2);
	loop.addVertex(new Point3D(-l, -l, 0));
	loop.addVertex(new Point3D(l, -l, 0));
	loop.addVertex(new Point3D(l, l, 0));
	loop.addVertex(new Point3D(-l, l, 0));

	ASSERT_TRUE(loop.testPoint(Point3D(0,0,0), &normal));	
}

TEST(LoopTests, testPointConvexLoopExterior)
{
	Vector3D normal = Vector3D(0, 0, 1);
	Loop loop = Loop();
	double l = 1 / sqrt(2);
	loop.addVertex(new Point3D(-l, -l, 0));
	loop.addVertex(new Point3D(l, -l, 0));
	loop.addVertex(new Point3D(l, l, 0));
	loop.addVertex(new Point3D(-l, l, 0));

	ASSERT_FALSE(loop.testPoint(Point3D(-10, 0, 0), &normal));
}

TEST(LoopTests, testPointConcaveLoopExterior1)
{
	Vector3D normal = Vector3D(0, 0, 1);
	Loop loop = Loop();

	double l = 1 / sqrt(2);
	double bigl = 2 / sqrt(2);

	loop.addVertex(new Point3D(-bigl, -bigl, 0));
	loop.addVertex(new Point3D(0, -bigl, 0)); // colinear point
	loop.addVertex(new Point3D(bigl, -bigl, 0));
	loop.addVertex(new Point3D(bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, -bigl, 0));

	loop.addVertex(new Point3D(-l, -l, 0));
	loop.addVertex(new Point3D(-l, l, 0));
	loop.addVertex(new Point3D(l, l, 0));
	loop.addVertex(new Point3D(l, -l, 0));
	loop.addVertex(new Point3D(-l, -l, 0));

	loop.clean();

	ASSERT_FALSE(loop.testPoint(Point3D(0, 0, 0), &normal));	
}


TEST(LoopTests, testPointConcaveLoopExterior2)
{
	Vector3D normal = Vector3D(0, 0, 1);
	Loop loop = Loop();
	
	double l = 1 / sqrt(2);
	double bigl = 2 / sqrt(2);

	loop.addVertex(new Point3D(-bigl, -bigl, 0));
	loop.addVertex(new Point3D(0, -bigl, 0)); // colinear point
	loop.addVertex(new Point3D(bigl, -bigl, 0));
	loop.addVertex(new Point3D(bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, -bigl, 0));

	loop.addVertex(new Point3D(-l, -l, 0));
	loop.addVertex(new Point3D(-l, l, 0));
	loop.addVertex(new Point3D(l, l, 0));
	loop.addVertex(new Point3D(l, -l, 0));
	loop.addVertex(new Point3D(-l, -l, 0));


	loop.clean();

	ASSERT_FALSE(loop.testPoint(Point3D(-10, 0, 0), &normal));	
}


TEST(LoopTests, testPointConcaveLoopInterior)
{
	Vector3D normal = Vector3D(0, 0, 1);
	Loop loop = Loop();
	double l = 1 / sqrt(2);
	double bigl = 2 / sqrt(2);

	loop.addVertex(new Point3D(-bigl, -bigl, 0));
	loop.addVertex(new Point3D(bigl, -bigl, 0));
	loop.addVertex(new Point3D(bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, -bigl, 0));

	loop.addVertex(new Point3D(-l, -l, 0));
	loop.addVertex(new Point3D(-l, l, 0));
	loop.addVertex(new Point3D(l, l, 0));
	loop.addVertex(new Point3D(l, -l, 0));
	loop.addVertex(new Point3D(-l, -l, 0));


	ASSERT_TRUE(loop.testPoint(Point3D(-1.5/sqrt(2), -1.5 / sqrt(2), 0), &normal));
}


TEST(LoopTests, testPointConcaveLoopInteriorWithClean)
{
	Vector3D normal = Vector3D(0, 0, 1);
	Loop loop = Loop();
	double l = 1 / sqrt(2);
	double bigl = 2 / sqrt(2);

	loop.addVertex(new Point3D(-bigl, -bigl, 0));
	loop.addVertex(new Point3D(0, -bigl, 0)); // colinear point
	loop.addVertex(new Point3D(bigl, -bigl, 0));
	loop.addVertex(new Point3D(bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, bigl, 0));
	loop.addVertex(new Point3D(-bigl, -bigl, 0));

	loop.addVertex(new Point3D(-l, -l, 0));
	loop.addVertex(new Point3D(-l, l, 0));
	loop.addVertex(new Point3D(l, l, 0));
	loop.addVertex(new Point3D(l, -l, 0));
	loop.addVertex(new Point3D(-l, -l, 0));


	loop.clean();

	ASSERT_TRUE(loop.testPoint(Point3D(-1.5 / sqrt(2), -1.5 / sqrt(2), 0), &normal));
}


TEST(LoopTests, clean)
{
	Loop loop = Loop();
	loop.addVertex(new Point3D(-2, -2, 0)); // 0
	loop.addVertex(new Point3D(0, -2, 0));  // 1 -- colinear point
	loop.addVertex(new Point3D(2, -2, 0));  // 2
	loop.addVertex(new Point3D(2, 2, 0));   // 3
	loop.addVertex(new Point3D(-2, 2, 0));  // 4

	loop.clean();

	for (int i = 0; i < loop.size(); i++) {
		if (i == 1) {
			// Test colinear point
			ASSERT_TRUE(loop.getVertexRef(i) == NULL);
		}
		else {
			ASSERT_FALSE(loop.getVertexRef(i) == NULL);
		}
	}
}


TEST(LoopTests, clean2)
{
	Loop loop = Loop();
	loop.addVertex(new Point3D(-2, -2, 0)); // 0
	loop.addVertex(new Point3D(2, -2, 0)); // 1
	loop.addVertex(new Point3D(2, 2, 0)); // 2
	loop.addVertex(new Point3D(-2, 2, 0)); // 3
	loop.addVertex(new Point3D(-2, 1, 0)); // 4. colinear point
	loop.addVertex(new Point3D(-2, 0, 0)); // 5. colinear point
	loop.addVertex(new Point3D(-2, -1, 0)); // 6. colinear point

	loop.clean();

	for (int i = 0; i < loop.size(); i++) {
		if (i > 3) {
			// Test colinear point
			ASSERT_TRUE(loop.getVertexRef(i) == NULL);
		}
		else {
			ASSERT_FALSE(loop.getVertexRef(i) == NULL);
		}
	}
}

