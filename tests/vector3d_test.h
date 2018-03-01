/* vector3d_test.h */

#include <cmath>

#include "../include/emp_core.h"
//#include "../src/config_constants.h"
//#include "../src/common/geometry/vector.h"


TEST(Vector3DTest, ConstructorByComponents)
{
	double x, y, z;
	x = 1; y = 2; z = 3;
	Vector3D p = Vector3D(x, y, z);
	ASSERT_EQ(p.getX(), x);
	ASSERT_EQ(p.getY(), y);
	ASSERT_EQ(p.getZ(), z);
}

TEST(Vector3DTest, isZero)
{
	ASSERT_TRUE(Vector3D(0,0,0).isZero());
}


TEST(Vector3DTest, isEqual)
{
	ASSERT_TRUE(Vector3D(0, 0, 0).isEqual(Vector3D(0,0,0)));
	ASSERT_FALSE(Vector3D(0, 0, 0).isEqual(Vector3D(0, 0, 2)));
}

TEST(Vector3DTest, dotProduct)
{

	Vector3D zero = Vector3D(0, 0, 0);
	Vector3D e1 = Vector3D(1, 0, 0); 
	Vector3D e2 = Vector3D(0, 1, 0);
	Vector3D e3 = Vector3D(0, 0, 1);

	ASSERT_EQ(zero*e1,0);
	ASSERT_EQ(e1*e2, 0);
	ASSERT_EQ(e1*e3, 0);
	ASSERT_EQ(e2*e3, 0);
	ASSERT_EQ(e1*e1, 1);
	ASSERT_EQ(e2*e2, 1);
	ASSERT_EQ(e3*e3, 1);

	double l = 3;
	Vector3D v45 = Vector3D(l, l, 0);
	ASSERT_EQ(v45*e1, v45*e2);
	ASSERT_EQ(v45*e3, 0);
	ASSERT_EQ(v45*e1, l);
}

TEST(Vector3DTest, length) {
	ASSERT_EQ(5, Vector3D(3, 4, 0).getLength());
}


TEST(Vector3DTest, squaredLength) {
	ASSERT_EQ(25, Vector3D(3, 4, 0).getSquaredLength());
}


TEST(Vector3DTest, normalize)
{	
	Vector3D v = Vector3D(12, 32, 211);
	v.normalize();
	ASSERT_TRUE(std::abs(v.getLength()-1) < EMP_TINY);
}

TEST(Vector3DTest, scaling)
{
	double x = 10;
	double y = 31;
	double z = 131;
	double scale = 3.2132;
	Vector3D v = Vector3D(x,y,z);

	Vector3D v2 = v * scale;

	ASSERT_EQ(x*scale, v2.getX());
	ASSERT_EQ(y*scale, v2.getY());
	ASSERT_EQ(z*scale, v2.getZ());

}

TEST(Vector3DTest, crossProduct)
{
	Vector3D zero = Vector3D(0, 0, 0);
	Vector3D i = Vector3D(1, 0, 0);
	Vector3D j = Vector3D(0, 1, 0);
	Vector3D k = Vector3D(0, 0, 1);

	ASSERT_TRUE((zero%i).isZero());
	ASSERT_TRUE((zero%j).isZero());
	ASSERT_TRUE((zero%k).isZero());

	ASSERT_TRUE((i%i).isZero());
	ASSERT_TRUE((j%j).isZero());
	ASSERT_TRUE((k%k).isZero());


	Vector3D k2 = i%j;
	ASSERT_TRUE(k2.isEqual(k));

	Vector3D i2 = j%k;
	ASSERT_TRUE(i2.isEqual(i));

	Vector3D j2 = k%i;
	ASSERT_TRUE(j2.isEqual(j));

	Vector3D kNeg= j%i;
	ASSERT_EQ(kNeg.getX(), -k.getX());
	ASSERT_EQ(kNeg.getY(), -k.getY());
	ASSERT_EQ(kNeg.getZ(), -k.getZ());

	Vector3D iNeg = k%j;
	ASSERT_EQ(iNeg.getX(), -i.getX());
	ASSERT_EQ(iNeg.getY(), -i.getY());
	ASSERT_EQ(iNeg.getZ(), -i.getZ());

	Vector3D jNeg = i%k;
	ASSERT_EQ(jNeg.getX(), -j.getX());
	ASSERT_EQ(jNeg.getY(), -j.getY());
	ASSERT_EQ(jNeg.getZ(), -j.getZ());


	Vector3D v1 = Vector3D(3, -3, 1);
	Vector3D v2 = Vector3D(4,  9, 2);
	Vector3D v1v2 = v1%v2;
	ASSERT_EQ(v1v2.getX(), -15);
	ASSERT_EQ(v1v2.getY(), -2);
	ASSERT_EQ(v1v2.getZ(), 39);
}
