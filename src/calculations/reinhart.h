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

#pragma once

#include "./common/geometry/vector.h"

// Returns the number of reinhart patches
/*!
@author German Molina
@param[in] MF the Reinhart subdivition scheme
@return The number of patches in such subdivition
*/
size_t nReinhartBins(int MF);

//! Returns the number of patches in a sky row
/*!
Auxiliar function present in reinhart.cal and reinsrc.cal files

@author German Molina
@param[in] r The row number
@param[in] MF The sky divition scheme
@return The number of patches
*/
size_t rnaz(size_t r, size_t MF);

//! Returns the accumulated number of patches in a sky row
/*!
Auxiliar function present in reinhart.cal and reinsrc.cal files

@author German Molina
@param[in] r The row number
@param[in] MF The sky divition scheme
@return The number of patches
*/
size_t raccum(size_t r, size_t MF);

//! Returns the Reinhart direction according to a certain bin number and MF scheme
/*!
Will also calculate the solid angle of the patch, if provided a non NULL pointer

@author German Molina
@param[in] nbin The bin number
@param[in] MF the sky subdivition scheme
@param[out] solidAngle The solid angle of the patch
@return The direction of the center of the patch
@note Patch 0 is ground!
*/
Vector3D reinhartDir(size_t nbin, size_t MF, double * solidAngle);

//! Returns the Reinhart direction according to a certain bin number and MF scheme
/*!
@author German Molina
@param[in] nbin The bin number
@param[in] MF the sky subdivition scheme
@return The direction of the center of the patch
@note Patch 0 is ground!
*/
Vector3D reinhartDir(size_t nbin, size_t MF);


//! Returns the Reinhart's patch solid angle
/*!
@author German Molina
@param[in] nbin The bin number
@param[in] MF the sky subdivition scheme
@return The solid angle
@note Patch 0 is ground!
*/
double reinhartSolidAngle(size_t nbin, size_t MF);

//! Returns the solid angle of a cone
/*!
@return the solid angle
@param[in] the angle
*/
double coneSolidAngle(double angle);