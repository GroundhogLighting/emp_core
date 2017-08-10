/* segment_test.h */

#include <cmath>

#include "../src/common/geometry/segment.h"


TEST(SegmentTest, ConstructorByComponents) {
	
	Point3D a = Point3D(0, 0, 0);
	Point3D b = Point3D(3, 4, 5);
	Segment s = Segment(&a, &b);

	ASSERT_TRUE(s.start->isEqual(a));
	ASSERT_TRUE(s.end->isEqual(b));
}

TEST(SegmentTest, isEqual) {

	Point3D a = Point3D(0, 0, 0);
	Point3D b = Point3D(3, 4, 5);
	Segment s1 = Segment(&a, &b);
	Segment s2 = Segment(&a, &b);

	ASSERT_TRUE(s1.isEqual(&s2));
}

TEST(SegmentTest, getLength) {

	Point3D a = Point3D(0, 0, 0);
	Point3D b = Point3D(3, 4, 5);
	Segment s = Segment(&a, &b);
	ASSERT_EQ(s.getLength(),a.distanceTo(b));
}


TEST(SegmentTest, intersect) {

	Point3D origin = Point3D(0, 0, 0);

	Point3D ax = Point3D(-1, 0, 0);
	Point3D bx = Point3D(1, 0, 0);
	Point3D offsetAx = Point3D(-1, 0, 1);
	Point3D offsetBx = Point3D(1, 0, 1);

	Point3D ay = Point3D(0, -1, 0);
	Point3D by = Point3D(0, 1, 0);

	Point3D az = Point3D(0, 0, -1);
	Point3D bz = Point3D(0, 0, 1);

	Segment xAxis = Segment(&ax, &bx);
	Segment yAxis = Segment(&ay, &by);
	Segment zAxis = Segment(&az, &bz);
	Segment offsetX = Segment(&offsetAx, &offsetBx);

	Point3D intersection = Point3D(-1,-1,-1);
	bool doIntersect;

	// X Y
	doIntersect = xAxis.intersect(&yAxis, &intersection);
	ASSERT_TRUE(doIntersect);
	ASSERT_TRUE(intersection.isEqual(origin));

	// X Z
	doIntersect = xAxis.intersect(&zAxis, &intersection);
	ASSERT_TRUE(doIntersect);
	ASSERT_TRUE(intersection.isEqual(origin));

	// Y Z
	doIntersect = zAxis.intersect(&yAxis, &intersection);
	ASSERT_TRUE(doIntersect);
	ASSERT_TRUE(intersection.isEqual(origin));

	// X OFFSET-X
	doIntersect = offsetX.intersect(&xAxis, &intersection);
	ASSERT_FALSE(doIntersect);
	
	// Z OFFSET-X
	doIntersect = offsetX.intersect(&zAxis, &intersection);
	ASSERT_TRUE(doIntersect);
	ASSERT_TRUE(intersection.isEqual(Point3D(0,0,1)));

}