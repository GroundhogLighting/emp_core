#pragma once

#include "../../common/geometry/point.h"
#include "../../common/geometry/vector.h"

class View {
private:
	Point3D viewPoint;
	Vector3D viewDirection;
	Vector3D viewUp;

public:
	Point3D viewPoint();
	Vector3D viewDirection();
	Vector3D viewUp();

};