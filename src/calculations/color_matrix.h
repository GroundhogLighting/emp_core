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


#pragma once
#include "../common/geometry/matrix.h"

//! A matrix of double numbers


class ColorMatrix {
    
private:
    Matrix red; //!< The red component of the Matrix
    Matrix green; //!< The green component of the Matrix
    Matrix blue; //!< The blue component of the Matrix
    
public:
    
    //! Default constructor    
    ColorMatrix();
    
    //! Constructor by size
    /*!
     @author German Molina
     @param nrows The number of rows in the matrix
     @param ncols The number of columns in the matrix
     */
    ColorMatrix(size_t nrows, size_t ncols );
    
    //! Returns the number of columns in a matrix
    /*!
     @author German Molina
     @return the number of columns
     */
    size_t ncols();
    
    //! Returns the number of rows in a matrix
    /*!
     @author German Molina
     @return the number of rows
     */
    size_t nrows();
    
    //! Retrieves the red component
    /*!
     @author German Molina
     @return A pointer to the red matrix
     */
    Matrix * redChannel();
    
    //! Retrieves the green component
    /*!
     @author German Molina
     @return A pointer to the green matrix
     */
    Matrix * greenChannel();
    
    //! Retrieves the blue component
    /*!
     @author German Molina
     @return A pointer to the blue matrix
     */
    Matrix * blueChannel();
    
    //! Multiplies a matrix by another matrix
    /*!
     @author German Molina
     @param[in] m A pointer to the other matrix
     @param[out] res The resulting matrix
     @return success
     */
    bool multiply(ColorMatrix * m, ColorMatrix * res);
    
    //! Multiplies a vector (Nx1 sized matrix) by a matrix and puts the result in a column of another matrix
    /*!
     @author German Molina
     @param[in] vec A pointer to the Vector
     @param[in] col The column of res where to put the answer
     @param[out] res The resulting matrix
     @return success
     */
    bool multiplyToColumn(ColorMatrix * vec, size_t col, ColorMatrix * res);
    
    //! Resizes the matrix to new sizes
    /*!
     Fills the new values, if available, with 0.
     
     @author German Molina
     @param nrows The new number of rows
     @param ncols The new number of columns
     */
    void resize(size_t nrows, size_t ncols);
    
    //! Transforms a ColorMatrix into a Matrix with the irradiance values
    /*!
     @author German Molina
     @param result The matrix to alocate the results
     */
    void calcIrradiance(Matrix * result);
    
    //! Transforms a ColorMatrix into a Matrix with the illuminance values
    /*!
     @author German Molina
     @param result The matrix to alocate the results
     */
    void calcIlluminance(Matrix * result);
    
};
