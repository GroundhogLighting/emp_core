/* point3d_test.h */

#include <cmath>

#include "../src/common/geometry/point3d.h"


TEST(Point3DTest, ConstructorByComponents) 
{
	double x, y, z;
	x = 1; y = 2; z = 3;
	Point3D p = Point3D(x,y,z);
    ASSERT_EQ(p.getX(),x);
    ASSERT_EQ(p.getY(),y);
    ASSERT_EQ(p.getZ(),z);
}

TEST(Point3DTest, ConstructorByOtherPoint)
{
	Point3D p = Point3D(1, 2, 3);
	Point3D p2 = Point3D(p);

	ASSERT_EQ(p2.getX(), 1);
	ASSERT_EQ(p2.getY(), 2);
	ASSERT_EQ(p2.getZ(), 3);
}


TEST(Point3DTest, isEqual)
{
	ASSERT_TRUE(Point3D(0, 0, 0).isEqual(Point3D(0, 0, 0)));
	ASSERT_FALSE(Point3D(0, 0, 0).isEqual(Point3D(0, 0, 2)));
}


TEST(Point3DTest, squaredDistanceToZero)
{
	double x, y, z;
	Point3D p = Point3D(0, 0, 0);

	// X distance
	x = 1;
	Point3D linear = Point3D(x, 0, 0);
	ASSERT_EQ(p.squaredDistanceTo(linear), x*x);

	// Pitagoras
	x = 3; y = 4; z = 0;
	Point3D pitagoras = Point3D(x, y, z);
	ASSERT_EQ(p.squaredDistanceTo(pitagoras), 5*5);

	// cube
	x = 2;
	Point3D cube = Point3D(x,x,x);
	ASSERT_EQ(p.squaredDistanceTo(cube), 2*2*3);

}


TEST(Point3DTest, distanceToZero)
{
	double x, y, z;
	Point3D p = Point3D(0, 0, 0);

	// X distance
	x = 1;
	Point3D linear = Point3D(x, 0, 0);
	ASSERT_EQ(p.distanceTo(linear), x);

	// Pitagoras
	x = 3; y = 4; z = 0;
	Point3D pitagoras = Point3D(x, y, z);
	ASSERT_EQ(p.distanceTo(pitagoras), 5);

	// cube
	x = 2;
	Point3D cube = Point3D(x, x, x);
	ASSERT_EQ(p.distanceTo(cube), 2 * sqrt(3));
}


TEST(Point3DTest, substractionAgainstZero)
{
	Point3D p2 = Point3D(0, 0, 0);
	double x, y, z;
	x = 2; y = 10; z = 32;
	Point3D p1 = Point3D(x, y, z);

	Vector3D v = p1 - p2;
	ASSERT_TRUE(v.isEqual(Vector3D(x, y, z)));

}


TEST(Point3DTest, genericSubstraction)
{
	double x1, y1, z1;
	x1 = 23; y1 = 10; z1 = 32;

	Point3D p1 = Point3D(x1, y1, z1);
	double x2, y2, z2;
	
	x2 = 2; y2 = 10; z2 = 32;
	Point3D p2 = Point3D(x2, y2, z2);

	Vector3D v = p1 - p2;
	ASSERT_EQ(v.getX(), x1-x2);
	ASSERT_EQ(v.getY(), y1-y2);
	ASSERT_EQ(v.getZ(), z1-z2);

}


TEST(Point3DTest, genericAddition)
{
	double x1, y1, z1;
	x1 = 23; y1 = 10; z1 = 32;

	Point3D p1 = Point3D(x1, y1, z1);
	double x2, y2, z2;

	x2 = 2; y2 = 10; z2 = 32;
	Vector3D v = Vector3D(x2, y2, z2);

	Point3D p2 = p1 + v;
	ASSERT_EQ(p2.getX(), x1 + x2);
	ASSERT_EQ(p2.getY(), y1 + y2);
	ASSERT_EQ(p2.getZ(), z1 + z2);

}

TEST(Point3DTest, position)
{
	Point3D p = Point3D(4, 6, 1);
	Vector3D v = p.position();

	ASSERT_EQ(p.getX(), v.getX());
	ASSERT_EQ(p.getY(), v.getY());
	ASSERT_EQ(p.getZ(), v.getZ());
}

TEST(Point3DTest, transform)
{
	// Identity transform
	double x, y, z;
	x = 0; y = 0; z = 0;
	Point3D p = Point3D(x, y, z);
	Vector3D i = Vector3D(1, 0, 0);
	Vector3D j = Vector3D(0, 1, 0);
	Vector3D k = Vector3D(0, 0, 1);

	ASSERT_EQ(p.transform(i, j, k).getX(), x);
	ASSERT_EQ(p.transform(i, j, k).getY(), y);
	ASSERT_EQ(p.transform(i, j, k).getZ(), z);

	x = 123; y = -21; z = 1;
	p = Point3D(x, y, z); 
	ASSERT_EQ(p.transform(i, j, k).getX(), x);
	ASSERT_EQ(p.transform(i, j, k).getY(), y);
	ASSERT_EQ(p.transform(i, j, k).getZ(), z);

	// Non identity
	i = Vector3D(1, 2, -1);
	j = Vector3D(2, 4, 6);
	k = Vector3D(-11, -2, -43);

	p = Point3D(1, 1, 1);
	ASSERT_EQ(p.transform(i, j, k).getX(), -8);
	ASSERT_EQ(p.transform(i, j, k).getY(), 4);
	ASSERT_EQ(p.transform(i, j, k).getZ(), -38);

}