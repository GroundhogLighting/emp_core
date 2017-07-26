#pragma once

#include <string>

#include "../../common/geometry/point.h"
#include "../../common/geometry/vector.h"

#define PERSPECTIVE_VIEW 0
#define PARALLEL_VIEW 1


class View {
private:
	std::string name;
	Point3D * viewPoint;
	Vector3D * viewDirection;
	Vector3D * viewUp;
	double viewHorizontal;
	double viewVertical;
	int viewType;

public:
	View();
	~View();

	void setName(std::string name);
	std::string getName();

	void setViewPoint(Point3D * point);
	Point3D * getViewPoint();
	void setViewUp(Vector3D * vector);
	Vector3D * getViewUp();
	void setViewDirection(Vector3D * vector);
	Vector3D * getViewDirection();
	void setViewHorizontal(double vH);
	double getViewHorizontal();
	void setViewVertical(double vV);
	double getViewVertical();
	void setViewType(int vT);
	int getViewType();

};