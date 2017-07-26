#pragma once

#include "./componentinstance.h"
#include "./componentdefinition.h"


class ComponentDefinition;

class ComponentInstance {


private:
	ComponentDefinition * definition;

	double rotationX;
	double rotationY;
	double rotationZ;

	double scale;

	double x;
	double y;
	double z;

public:
	ComponentInstance(ComponentDefinition * definitionRef);
	~ComponentInstance();

	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void setRotationX(double rx);
	void setRotationY(double ry);
	void setRotationZ(double rz);

	double getX();
	double getY();
	double getZ();
	double getRotationX();
	double getRotationY();
	double getRotationZ();

	void setScale(double s);
	double getScale();

	ComponentDefinition * getDefinitionRef();

};