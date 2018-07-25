/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

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

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "./matrix4x4.h"

class Transform {
private:
  Matrix4x4 m = Matrix4x4();  

public:
  //! Basic constructor.
  /*!
  Builds an identity transform.

  @author German Molina  
  */
  Transform();

  
  //! Creates a translation matrix
  /*!
  @author German Molina
  @param[in] x The translation on the X axis
  @param[in] y The translation on the Y axis
  @param[in] x The translation on the Z axis
  */
  Matrix4x4 * getTranslationMatrix(double x, double y, double z);

  //! Creates a Rotation matrix on axis X
  /*!
  @author German Molina
  @param[in] rotation The rotation
  */
  Matrix4x4 * getRotationXMatrix(double rotation);

  //! Creates a Rotation matrix on axis Y
  /*!
  @author German Molina
  @param[in] rotation The rotation
  */
  Matrix4x4 * getRotationYMatrix(double rotation);

  //! Creates a Rotation matrix on axis Z
  /*!
  @author German Molina
  @param[in] rotation The rotation
  */
  Matrix4x4 * getRotationZMatrix(double rotation);


  //! Creates a Scale matrix 
  /*!
  @author German Molina
  @param[in] scale The scale
  */
  Matrix4x4 * getScaleMatrix(double scale);

  //! Retrieves a pointer to the matrix
  /*!
  @author German Molina
  @return The Matrix4x4
  */
  Matrix4x4 * getMatrix();

  //! Adds the transformation from a base transformation
  /*!
  Basically does (this) = t*(this)

  @author German Molina
  @param[in] t The base transform
  */
  void preMultiply(Transform * t);

};

extern Transform transform;

#endif
