/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "./componentinstance.h"
#include "common/utilities/io.h"


ComponentInstance::ComponentInstance(ComponentDefinition * definitionRef) {
	definition = definitionRef;	
	rotationX = 0; 
	rotationY = 0; 
	rotationZ = 0; 

	scale = 1; 

	x = 0; 
	y = 0; 
	z = 0; 
}


ComponentInstance::~ComponentInstance() {
}

void ComponentInstance::setX(double newX) {
	x = newX;
}

void ComponentInstance::setY(double newY) {
	y = newY;
}

void ComponentInstance::setZ(double newZ) {
	z = newZ;
}

double ComponentInstance::getX() {
	return x;
}

double ComponentInstance::getY() {
	return y;
}

double ComponentInstance::getZ() {
	return z;
}


void ComponentInstance::setRotationX(double newRotationX) {
	rotationX = newRotationX;
}

void ComponentInstance::setRotationY(double newRotationY) {
	rotationY = newRotationY;
}

void ComponentInstance::setRotationZ(double newRotationZ) {
	rotationZ = newRotationZ;
}

double ComponentInstance::getRotationX() {
	return rotationX;
}

double ComponentInstance::getRotationY() {
	return rotationY;
}

double ComponentInstance::getRotationZ() {
	return rotationZ;
}

void ComponentInstance::setScale(double s) {
	scale = s;
}

double ComponentInstance::getScale() {
	return scale;
}

ComponentDefinition * ComponentInstance::getDefinitionRef() {
	return definition;
}

Transform * ComponentInstance::getTransform()
{

  Transform * res = new Transform();
  Matrix4x4 * m = res->getMatrix();

  // Scale
  /*
  double s = getScale();
  Matrix4x4 * scale = res->getScaleMatrix(s);
  m->multiplyThis(scale);
  delete scale;
  */
  // Translation
  double x = getX();
  double y = getY();
  double z = getZ();
  Matrix4x4 * translation = res->getTranslationMatrix(x, y, z);
  m->multiplyThis(translation);
  delete translation;

  
  // Rotation X
  double rx = getRotationX();
  Matrix4x4 * rotationX = res->getRotationXMatrix(rx);
  m->multiplyThis(rotationX);
  delete rotationX;


  // Rotation Y
  double ry = getRotationY();
  Matrix4x4 * rotationY = res->getRotationYMatrix(ry);
  m->multiplyThis(rotationY);
  delete rotationY;

  // Rotation Z
  double rz = getRotationZ();
  Matrix4x4 * rotationZ = res->getRotationZMatrix(rz);
  m->multiplyThis(rotationZ);
  delete rotationZ;

  

  return res;
}