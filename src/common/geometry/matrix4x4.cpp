/*****************************************************************************
	Emp

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

//! This class represents a 4x4 matrix
/*!
This class is focused on performing geometric transformations. 

A matrix contains an array 'data' of 16 doubles, that represent 
the 16 elements organized by row. That is, data[0] is the element
in the first row and first column; data[1] is the element in the
first row and second column.
*/

#include "./matrix4x4.h"
#include <stdexcept>
#include <iostream>

Matrix4x4::Matrix4x4()
{
}

Matrix4x4::Matrix4x4(Matrix4x4 * m)
{
  for (int i = 0; i < 16; i++) {
    data[i] = m->getElement(i);
  }
}

double Matrix4x4::getElement(int i)
{
  if (i >= 16 || i < 0)
    throw std::invalid_argument("index out of range when getting element from Matrix4x4");

  return data[i];
}

double Matrix4x4::getElement(int row, int col)
{
  if (row >= 4 || row < 0 || col >= 4 || col < 0)
    throw std::invalid_argument("Row or Column out of range when getting element from Matrix4x4");

  return data[4*row + col];
}

void Matrix4x4::setElement(int row, int col, double value)
{
  if (row >= 4 || row < 0 || col >= 4 || col < 0)
    throw std::invalid_argument("Row or Column out of range when setting element from Matrix4x4");

  data[4 * row + col] = value;
}


Matrix4x4 * Matrix4x4::operator*(Matrix4x4 * m)
{
  Matrix4x4 * ret = new Matrix4x4();

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {

      double v = 0;
      for (int i = 0; i < 4; i++) {
        v = v + getElement(row, i)*m->getElement(i, col);
      }

      ret->setElement(row, col, v);
    }
  }

  return ret;
}

void Matrix4x4::multiplyThis(Matrix4x4 * m)
{

  // Multiply
  Matrix4x4 * res = (*this)*m;

  // Copy data
  for (int i = 0; i < 16; i++) {
    data[i] = res->getElement(i);
  }

  // delete
  delete res;

}

void Matrix4x4::print()
{
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      std::cout << getElement(row, col) << "\t";
    }
    std::cout << "\n";
  }

  std::cout << "--------\n";

}
