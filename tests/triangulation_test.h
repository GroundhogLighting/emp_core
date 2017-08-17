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

TEST(TriangulateTest, mergeTriangleCorrect)
{

	Polygon3D p = Polygon3D();
	Triangulation t = Triangulation(&p);

	Point3D a = Point3D(-1, 0, 0);
	Point3D b = Point3D(0, 0, 0);
	Point3D c = Point3D(0, 1, 0);
	Point3D op = Point3D(1, 0, 0);

	Triangle * t1 = new Triangle(&a, &b, &c);
	std::cout << "INDEX OF T1 " << t1->getIndex() << std::endl;
	Triangle * t2 = new Triangle(&b, &op, &c);
	t.addTriangle(t1);
	t.addTriangle(t2);

	t1->setNeighbor(t2, 1, true);

	ASSERT_TRUE(t.mergeIfPossible(t1, 1));

	ASSERT_EQ(3,t.getNumTriangles());
	ASSERT_TRUE(t.getTriangleRef(0) == NULL);
	ASSERT_TRUE(t.getTriangleRef(1) == NULL);
	ASSERT_FALSE(t.getTriangleRef(2) == NULL);

	ASSERT_TRUE(t.getTriangleRef(2)->getVertex(0)->isEqual(a));
	ASSERT_TRUE(t.getTriangleRef(2)->getVertex(1)->isEqual(op));
	ASSERT_TRUE(t.getTriangleRef(2)->getVertex(2)->isEqual(c));
}


TEST(TriangulateTest, mergeTriangleIncorrect)
{

	Polygon3D p = Polygon3D();
	Triangulation t = Triangulation(&p);

	Point3D a = Point3D(-1, 0, 0);
	Point3D b = Point3D(0, 0, 0);
	Point3D c = Point3D(0, 1, 0);
	Point3D op = Point3D(1, 0.5, 0);

	Triangle * t1 = new Triangle(&a, &b, &c);
	Triangle * t2 = new Triangle(&b, &op, &c);
	t.addTriangle(t1);
	t.addTriangle(t2);

	t1->setNeighbor(t2, 1, true);

	ASSERT_FALSE(t.mergeIfPossible(t1, 1));
}

