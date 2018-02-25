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

#ifndef VECTOR_3D_H
#define VECTOR_3D_H

//! A 3D vector representation


class Vector3D {

private:
	double x; //!< The X component
	double y; //!< The Y component
	double z; //!< The Z component

public:

	//! Creates a new Vector3D object.
	/*!
	Assigns x, y and z values to the x, y and z members respectively

	@author German Molina
	@param[in] x the x component of the vector
	@param[in] y the y component of the vector
	@param[in] z the z component of the vector
	*/
	Vector3D(double x, double y, double z);
	
	//~Vector3D();


	//! Retrieves the X component of the vector
	/*!
	@author German Molina
	@return the X component of the vector
	*/
	double getX();

	//! Retrieves the Y component of the vector
	/*!
	@author German Molina
	@return the Y component of the vector
	*/
	double getY();

	//! Retrieves the Z component of the vector
	/*!
	@author German Molina
	@return the Z component of the vector
	*/
	double getZ();

	//! Dot product
	/*!
	@author German Molina
	@param[in] v The other vector
	@return The dot product
	*/
	double operator*(Vector3D v);

	//! Scaling (i.e. product by a constant)
	/*!
	@author German Molina
	@param[in] s The scaling factor
	@return The result
	*/
	Vector3D operator*(double s);

	//! Inverse scaling (i.e. division by a constant)
	/*!
	@author German Molina
	@param[in] s The scaling factor
	@return The result
	*/
	Vector3D operator/(double s);

	//! Vector addition
	/*!
	@author German Molina
	@param[in] v The other vector
	@return The addition 
	*/
	Vector3D operator+(Vector3D v);

	//! Vector sustraction
	/*!
	@author German Molina
	@param[in] v The other vector
	@return The result
	*/
	Vector3D operator-(Vector3D v);

	//! Cross product
	/*!
	@author German Molina
	@param[in] v The other vector
	@return The cross product
	@note Remember to remove!
	*/
	Vector3D operator%(Vector3D v);

	//! Checks if two vectors are parallel
	/*!
	@author German Molina
	@param[in] v The other vector
	@return True if they are parallel
	@note One can be oposite to the other.
	*/
	bool  isParallel(Vector3D v);

	//! Checks if two vectors go in the same direction
	/*!
	@author German Molina
	@param[in] v The other vector
	@return True if they are the same direction
	*/
	bool  sameDirection(Vector3D v);

	//! Checks if a vector can be considered Zero
	/*!
	@author German Molina
	@return True or False
	*/
	bool isZero();

	//! Checks if a vector can be considered equal to another
	/*!
	@author German Molina
	@param[in] v The other vector
	@return True or False
	*/
	bool isEqual(Vector3D v);

	//! Calculates the length of a vector
	/*!
	@author German Molina
	@return length
	*/
	double getLength();

	//! Calculates the squared length of a vector
	/*!
	@author German Molina
	@return length
	*/
	double getSquaredLength();

	//! Normalizes a Vector3D
	/*!
	@author German Molina	
	*/
	void normalize();

	//! Prints the Vector to the stdout
	/*!
	@author German Molina
	*/
	void print();
};

extern Vector3D vector3D;

#endif