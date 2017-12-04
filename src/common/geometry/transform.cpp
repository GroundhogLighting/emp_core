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

#include "./transform.h"
#include <cmath>

#define DEGREES(x) x*3.141592654/180

Transform::Transform()
{

}



Matrix4x4 * Transform::getTranslationMatrix(double x, double y, double z)
{
  Matrix4x4 * res = new Matrix4x4();
  res->setElement(0, 3, x);
  res->setElement(1, 3, y);
  res->setElement(2, 3, z);

  return res;
}


Matrix4x4 * Transform::getRotationXMatrix(double rotation)
{
  Matrix4x4 * res = new Matrix4x4();
  rotation = DEGREES(rotation);
  res->setElement(1, 1, cos(rotation));
  res->setElement(1, 2, -sin(rotation));
  res->setElement(2, 1, sin(rotation));
  res->setElement(2, 2, cos(rotation));

  return res;
}


Matrix4x4 * Transform::getRotationYMatrix(double rotation)
{
  Matrix4x4 * res = new Matrix4x4();
  rotation = DEGREES(rotation);
  res->setElement(0, 0, cos(rotation));
  res->setElement(0, 2, sin(rotation));
  res->setElement(2, 0, -sin(rotation));
  res->setElement(2, 2, cos(rotation));

  return res;
}


Matrix4x4 * Transform::getRotationZMatrix(double rotation)
{
  Matrix4x4 * res = new Matrix4x4();
  rotation = DEGREES(rotation);
  res->setElement(0, 0, cos(rotation));
  res->setElement(0, 1, -sin(rotation));
  res->setElement(1, 0, sin(rotation));
  res->setElement(1, 1, cos(rotation));

  return res;
}

Matrix4x4 * Transform::getScaleMatrix(double scale)
{
  Matrix4x4 * res = new Matrix4x4();
  res->setElement(0, 0, scale);
  res->setElement(1, 1, scale);
  res->setElement(2, 2, scale);

  return res;
}

Matrix4x4 * Transform::getMatrix()
{
  return &m;
}

void Transform::preMultiply(Transform * t)
{
  // Clone the given matrix
  Matrix4x4 aux = Matrix4x4(t->getMatrix());

  // Multiply by this transform
  aux.multiplyThis(&m);

  // Clone
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      m.setElement(row,col, aux.getElement(row,col));
    }
  }
}