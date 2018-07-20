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

#include <vector>
#include "tbb/tbb.h"

//! A matrix of double numbers

class Matrix {
    
private:
    std::vector< std::vector<double> > data; //!< The numerical data inside the matrix
    
public:
    
    //! Constructor
    /*!
     Builds a 1x1 matrix with a one in it.
     @author German Molina
     */
    Matrix();
    
    //! Constructor by size
    /*!
     @author German Molina
     @param nrows The number of rows in the matrix
     @param ncols The number of columns in the matrix
     */
    Matrix(size_t nrows, size_t ncols );
    
    //! Prints the matrix to the stdout
    /*!
     To be used in Debugging processes
     
     @author German Molina
     */
    void print() const;
    
    //! Returns the number of columns in a matrix
    /*!
     @author German Molina
     @return the number of columns
     */
    size_t ncols() const;
    
    //! Returns the number of rows in a matrix
    /*!
     @author German Molina
     @return the number of rows
     */
    size_t nrows() const;
    
    //! Retrieves a row
    /*!
     @author German Molina
     @param nrow The row number to retrieve
     */
    std::vector<double> * operator[](size_t nrow);
    
    //! Resizes the matrix to new sizes
    /*!
     Fills the new values, if available, with 0.
     
     @author German Molina
     @param nrows The new number of rows
     @param ncols The new number of columns
     */
    void resize(size_t nrows, size_t ncols);
    
    
    //! Multiplies a matrix by another matrix
    /*!
     @author German Molina
     @param[in] m A pointer to the other matrix
     @param[out] res The resulting matrix
     @return success
     */
    bool multiply(const Matrix * m, Matrix * res) const;

    //! Multiplies a vector (Nx1 sized matrix) by a matrix and puts the result in a column of another matrix
    /*!
     @author German Molina
     @param[in] vec A pointer to the Vector
     @param[in] col The column of res where to put the answer
     @param[out] res The resulting matrix
     @return success
     */
    bool multiplyToColumn( const Matrix * vec, size_t col, Matrix * res) const;
    
    //! Multiplies a single row of a vector vector (Nx1 sized matrix) by a matrix and puts the result in a column of another matrix
    /*!
     This method is used for when the vector Vec has only zeroes with the exception of
     one item.
     
     @author German Molina
     @param[in] vec A pointer to the Vector
     @param[in] row The element in the vector that is not Zero
     @param[in] col The column of res where to put the answer
     @param[out] res The resulting matrix
     @return success
     */
    bool multiplyRowToColumn( const Matrix * vec, size_t row, size_t col, Matrix * res) const;
    
    //! Sets a certain element to a certain value
    /*!
    @param nrow The row number of the element
    @param ncol The column number of the element
    @param value The value to set
    */
    void setElement(size_t nrow, size_t ncol, double value);
    
    //! Retrieves a certain element
    /*!
     @param nrow The row number of the element
     @param ncol The column number of the element
     @return the value of the element
     */
    double getElement(size_t nrow, size_t ncol) const;
};

extern Matrix matrix;





