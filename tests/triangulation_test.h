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

TEST(TriangulateTest, ResetNeighborhood)
{
	Polygon3D p = Polygon3D();
	Triangulation t = Triangulation(&p);

	// Add triangle 0
	Point3D * a0 = new Point3D(0, 0, 0);
	Point3D * b0 = new Point3D(0, 1, 0);
	Point3D * c0 = new Point3D(1, 0, 0);
	Triangle * t0 = new Triangle(a0, b0, c0);
	t.addTriangle(t0);

	// Add triangle 1
	Point3D * a1 = new Point3D(0, 0, 0);
	Point3D * b1 = new Point3D(0, 1, 0);
	Point3D * c1 = new Point3D(-1, 0, 0);
	Triangle * t1 = new Triangle(a1, b1, c1);
	t.addTriangle(t1);

	// Add triangle 2
	Point3D * a2 = new Point3D(0, 0, 0);
	Point3D * b2 = new Point3D(0, -1, 0);
	Point3D * c2 = new Point3D(-1, 0, 0);
	Triangle * t2 = new Triangle(a2, b2, c2);
	t.addTriangle(t2);

	// Add triangle 3
	Point3D * a3 = new Point3D(0, 0, 0);
	Point3D * b3 = new Point3D(0, -1, 0);
	Point3D * c3 = new Point3D(1, 0, 0);
	Triangle * t3 = new Triangle(a3, b3, c3);
	t.addTriangle(t3);

	// Reset neighborhoods
	t.resetNeighborhoods();
	
	// Check results.
	ASSERT_TRUE(t0->getNeighbor(0) == t1);
	ASSERT_TRUE(t0->getNeighbor(1) == NULL);
	ASSERT_TRUE(t0->getNeighbor(2) == t3);


	ASSERT_TRUE(t1->getNeighbor(0) == t0);
	ASSERT_TRUE(t1->getNeighbor(1) == NULL);
	ASSERT_TRUE(t1->getNeighbor(2) == t2);


	ASSERT_TRUE(t2->getNeighbor(0) == t3);
	ASSERT_TRUE(t2->getNeighbor(1) == NULL);
	ASSERT_TRUE(t2->getNeighbor(2) == t1);


	ASSERT_TRUE(t3->getNeighbor(0) == t2);
	ASSERT_TRUE(t3->getNeighbor(1) == NULL);
	ASSERT_TRUE(t3->getNeighbor(2) == t0);
}

TEST(TriangulateTest, cleanTest)
{
  Polygon3D * p = new Polygon3D();
  
  Point3D * a = new Point3D(1.666323, -22.30068,0);
  Point3D * b = new Point3D(1.666323, -18.399,4);
  Point3D * c = new Point3D(2.701599,-22.164,0);

  Loop * loop = p->getOuterLoopRef();
  loop->addVertex(a);
  loop->addVertex(b);
  loop->addVertex(c);

  Vector3D normal = Vector3D(0.09269, 0.70406, -0.70406);
  p->setNormal(normal);

  Triangulation tri = Triangulation(p);
  tri.mesh(0.25, 1.3);

  Triangulation triClean = Triangulation(p);
  triClean.mesh(0.25, 1.3);
  triClean.purge();

  ASSERT_EQ(triClean.getNumTriangles(), tri.realSize());

  size_t size = triClean.getNumTriangles();
  size_t count = 0;
  for (size_t i = 0; i < size; i++) {
    if (tri.getTriangleRef(i) == nullptr)
      continue;    

    ASSERT_TRUE(tri.getTriangleRef(i)->isEqual(triClean.getTriangleRef(count++)));
  }

}
