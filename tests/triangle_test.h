/* triangle_test.h */

#include <cmath>

#include "../include/emp_core.h"
//#include "../src/common/geometry/triangle.h"

TEST(TriangleTest, getArea)
{
	double l = 3;
	Point3D * a = new Point3D(-l, 0, 0);
	Point3D * b = new Point3D(l, 0, 0);
	Point3D * c = new Point3D(0, l, 0);

	Triangle t = Triangle(a, b, c);

	ASSERT_TRUE(std::abs(t.getArea()-l*l) < EMP_TINY);

}




TEST(TriangleTest, testPoint)
{
	int code;
	bool inTriangle;
	Polygon3D p = Polygon3D();

	Point3D * a = new Point3D(-1, 0, 0);
	Point3D * b = new Point3D(1, 0, 0);
	Point3D * c = new Point3D(0, 1, 0);

	Triangle triangle = Triangle(a, b, c);

	// Vertex A.
	inTriangle = triangle.testPoint(a, &code);
	ASSERT_TRUE(inTriangle);
	ASSERT_EQ(code, 0);

	// Vertex B.
	inTriangle = triangle.testPoint(b, &code);
	ASSERT_TRUE(inTriangle);
	ASSERT_EQ(code, 1);

	// Vertex C.
	inTriangle = triangle.testPoint(c, &code);
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
  
  // TEST 1... centerd in origin
	double l = 3;
	Point3D * a = new Point3D(-l,0,0);
	Point3D * b = new Point3D(l, 0, 0);
	Point3D * c = new Point3D(0, l, 0);

	Triangle t = Triangle(a, b, c);

	Point3D center1 = t.getCircumCenter();	
	ASSERT_TRUE(center1.isEqual(Point3D(0, 0, 0)));
    
    // Test 2: Centered somewhere else
	l = 3;
	double cx = 23;
	double cy = 21;
	double cz = 3.12314;
	Point3D * a2 = new Point3D(-l + cx, 0 + cy, cz);
    Point3D * b2 = new Point3D(l + cx, 0 + cy, cz);
    Point3D * c2 = new Point3D(0 + cx, l + cy, cz);

	Triangle t2 = Triangle(a2, b2, c2);

	Point3D center2 = t2.getCircumCenter();	
    
	ASSERT_TRUE(center2.isEqual(Point3D(cx, cy, cz)));
  
    // Test 3: From internet solved problem
    Point3D * a3 = new Point3D(3, 2, 0);
    Point3D * b3 = new Point3D(1, 4, 0);
    Point3D * c3 = new Point3D(5, 4, 0);

    Triangle t3 = Triangle(a3, b3, c3);
    Point3D center3 = t3.getCircumCenter();
    ASSERT_TRUE(center3.isEqual(Point3D(3, 4, 0)));
}


