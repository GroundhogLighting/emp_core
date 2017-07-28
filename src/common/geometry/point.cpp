#include<cmath>

#include "./point.h"

Point3D::Point3D(double new_x, double new_y, double new_z) 
{
	x = new_x;
	y = new_y;
	z = new_z;
}


Point3D::Point3D(Point3D * point) 
{ // clone implementation
	x = point->getX();
	y = point->getY();
	z = point->getZ();
}

double Point3D::getX() 
{
	return x;
}

double Point3D::getY() 
{
	return y;
}

double Point3D::getZ() {
	return z;
}


double Point3D::squaredDistanceTo(Point3D * point) 
{
	double dx = (x - point->x)*(x - point->x);
	double dy = (y - point->y)*(y - point->y);
	double dz = (z - point->z)*(z - point->z);
	return dx + dy + dz;
}



double Point3D::distanceTo(Point3D * point) 
{	
	return sqrt(squaredDistanceTo(point));
}