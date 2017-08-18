/* vector3d_test.h */

#include <cmath>

#include "../src/common/geometry/triangulation.h"


TEST(TriangulateTest, isConvex)
{

	Polygon3D p = Polygon3D();
	Triangulation t = Triangulation(&p);

	Point3D a = Point3D(-1, -1, 0);
	Point3D b = Point3D(1, -1, 0);
	Point3D c = Point3D(1, 1, 0);
	Point3D d = Point3D(-1, 1, 0);

	ASSERT_TRUE(t.isConvex(&a, &b, &c, &d));
	ASSERT_FALSE(t.isConvex(&a, &b, &d, &c));
	ASSERT_FALSE(t.isConvex(&a, &a, &a, &a));
	
}
