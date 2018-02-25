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

#ifndef MATRIX_4X4_H
#define MATRIX_4X4_H

class Matrix4x4 {
private:
  double data[16] = {1,0,0,0,   0,1,0,0,    0,0,1,0,    0,0,0,1};
public:
  //! Constructor
  /*!
  @author German Molina
  */
  Matrix4x4();

  //! Clone constructor
  /*!
  @author German Molina
  */
  Matrix4x4(Matrix4x4 * m);

  //! Retrieves an element from the matrix
  /*!
  @author German Molina
  @return the element
  @param[in] i The index of the element
  */
  double getElement(int i);

  //! Retrieves a specific element from the matrix
  /*!
  @author German Molina
  @return the element
  @param[in] row The row of the element (starts from 0)
  @param[in] col The column of the element (starts from 0)
  */
  double getElement(int row, int col);

  //! Retrieves a specific element from the matrix
  /*!
  @author German Molina
  @return the element
  @param[in] row The row of the element (starts from 0)
  @param[in] col The column of the element (starts from 0)
  @param[in] value The value of the element
  */
  void setElement(int row, int col, double value);

  //! Multiplies a matrix by another matrix
  /*!
  @author German Molina
  @param[in] m A pointer to the other matrix
  @return A pointer to a new matrix
  @note This method creates a new matrix that should be deleted
  */
  Matrix4x4 * operator*(Matrix4x4 * m);  

  //! Prints the matrix in a readable format
  /*!
  @author German Molina  
  */
  void print();

  //! Multiplies the Matrix4x4 by another Matrix4x4 object
  /*!
  Instead of returning a new object, it will be modified
  @author German Molina
  @param[in] m The pointer to the other matrix
  */
  void multiplyThis(Matrix4x4 * m);

};

extern Matrix4x4 matrix4x4;

#endif
