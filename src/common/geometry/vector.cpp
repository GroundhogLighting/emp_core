#include "./vector.h"

Vector3D::Vector3D(double new_x, double new_y, double new_z) {
	x = new_x;
	y = new_y;
	z = new_z;
}

Vector3D::~Vector3D() {

}

double Vector3D::getX() {
	return x;
}

double Vector3D::getY() {
	return y;
}

double Vector3D::getZ() {
	return z;
}