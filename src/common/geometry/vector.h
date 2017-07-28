#pragma once


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

};