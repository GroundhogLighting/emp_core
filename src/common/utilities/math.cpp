#include <SketchUpAPI/model/face.h>

#include <math.h>

double distanceBetween(SUPoint3D a, SUPoint3D b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

double squaredDistanceBetween(SUPoint3D a, SUPoint3D b) {
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);
}