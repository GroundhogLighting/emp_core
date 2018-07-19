#include "../include/emp_core.h"


TEST(SolarTest, A) {
	
	Point3D a = Point3D(0, 0, 0);
	Point3D b = Point3D(3, 4, 5);
	Segment s = Segment(&a, &b);

	ASSERT_TRUE(s.start->isEqual(a));
	ASSERT_TRUE(s.end->isEqual(b));
}
