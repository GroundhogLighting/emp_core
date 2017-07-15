#pragma once

class Point3D {

private:
	double x;
	double y;
	double z;

public:
	Point3D(double x, double y, double z);
	Point3D(Point3D * point); // clone implementation
	//~Point3D();
	double getX();
	double getY();
	double getZ();
	double squaredDistanceTo(Point3D * point);
	double distanceTo(Point3D * point);
};