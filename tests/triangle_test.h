/* triangle_test.h */

#include <cmath>

#include "../src/common/geometry/triangle.h"

TEST(TriangleTest, getArea)
{
	double l = 3;
	Point3D a = Point3D(-l, 0, 0);
	Point3D b = Point3D(l, 0, 0);
	Point3D c = Point3D(0, l, 0);

	Triangle t = Triangle(&a, &b, &c);

	ASSERT_TRUE(std::abs(t.getArea()-l*l) < GLARE_TINY);

}




TEST(TriangleTest, testPoint)
{
	int code;
	bool inTriangle;
	Polygon3D p = Polygon3D();

	Point3D a = Point3D(-1, 0, 0);
	Point3D b = Point3D(1, 0, 0);
	Point3D c = Point3D(0, 1, 0);

	Triangle triangle = Triangle(&a, &b, &c);

	// Vertex A.
	inTriangle = triangle.testPoint(&a, &code);
	ASSERT_TRUE(inTriangle);
	ASSERT_EQ(code, 0);

	// Vertex B.
	inTriangle = triangle.testPoint(&b, &code);
	ASSERT_TRUE(inTriangle);
	ASSERT_EQ(code, 1);

	// Vertex C.
	inTriangle = triangle.testPoint(&c, &code);
	ASSERT_TRUE(inTriangle);
	ASSERT_EQ(code, 2);

	// Segment AB.
	Point3D origin = Point3D(0, 0, 0);
	inTriangle = triangle.testPoint(&origin, &code);
	ASSERT_TRUE(inTriangle);
	ASSERT_EQ(code, 3);

	// Point outside
	Point3D point = Point3D(0, -1, 0);
	inTriangle = triangle.testPoint(&point, &code);
	ASSERT_FALSE(inTriangle);
	ASSERT_EQ(code, -1);
}

TEST(TriangleTest, circumCenter) {
	double l = 3;
	Point3D a = Point3D(-l,0,0);
	Point3D b = Point3D(l, 0, 0);
	Point3D c = Point3D(0, l, 0);

	Triangle t = Triangle(&a, &b, &c);

	Point3D center1 = t.getCircumCenter();	
	EXPECT_TRUE(center1.isEqual(Point3D(0, 0, 0)));

	l = 3;
	double cx = 23;
	double cy = 21;
	double cz = 3.12314;
	a = Point3D(-l + cx, 0 + cy, cz);
	b = Point3D(l + cx, 0 + cy, cz);
	c = Point3D(0 + cx, l + cy, cz);

	t = Triangle(&a, &b, &c);

	Point3D center2 = t.getCircumCenter();	
	ASSERT_TRUE(center2.isEqual(Point3D(cx, cy, cz)));
}


