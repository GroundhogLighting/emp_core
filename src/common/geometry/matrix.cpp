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

#include "./matrix.h"
#include <iostream>
#include <stdexcept>
#include "common/utilities/io.h"

#define NROWS data.size()
#define NCOLS data[0].size()

Matrix::Matrix()
{
    data = std::vector< std::vector <double> >(1);
}

Matrix::Matrix(size_t nrows, size_t ncols)
{
    data = std::vector< std::vector <double> >(nrows,std::vector <double>(ncols,0.0));
}


void Matrix::print()
{
    size_t nrows = NROWS;
    size_t ncols = NCOLS;
    
    for (size_t row = 0; row < nrows; row++) {
        for (int col = 0; col < ncols; col++) {
            std::cout << data[row][col] << "\t";
        }
        std::cout << "\n";
    }    
    std::cout << "--------\n";
    
}

size_t Matrix::ncols()
{
    return NCOLS;
}

size_t Matrix::nrows()
{
    return NROWS;
}

std::vector<double> * Matrix::operator[](size_t nrow)
{
    return &data[nrow];
}

void Matrix::resize(size_t nrows, size_t ncols)
{
    data.resize(nrows);
    for(size_t nrow = 0; nrow < nrows; nrow++ ){
        data[nrow].resize(ncols,0.0);
    }
}

bool Matrix::multiply(Matrix * m, Matrix * res)
{
    // Check size consistency with m
    if (NCOLS != m->nrows())
        throw std::invalid_argument("Size mismatch between matrices when trying to multiply()");
    
    const size_t ncols = m->ncols();
    const size_t aux = m->nrows();
    
    // Check size consistency with res
    if(res->ncols() != m->ncols() || res->nrows() != NROWS){
        WARN(msg, "Size mismatch between resulting matrix and factors... resizing results");
        res->resize(NROWS,m->ncols());
    }
    
    // Multiply
    for (int row = 0; row < NROWS; row++) {
        for (int col = 0; col < ncols ; col++) {
            double v = 0;
            for (int i = 0; i < aux; i++) {
                v += (data[row][i] * (*m)[i]->at(col));
            }
            (*res)[row]->at(col) = v;
        }
    }
    
    return true;
}

bool Matrix::multiplyToColumn(Matrix * vec, size_t col, Matrix * res)
{
    if ( 1 != vec->ncols())
        throw std::invalid_argument("vector needs to have only one column multiplyToLocation()");
    
    // Check size consistency with m
    if (NCOLS != vec->nrows())
        throw std::invalid_argument("Size mismatch between matrices when trying to multiply()");
    
    // Check size consistency with res
    if(res->ncols() <= col || res->nrows() != NROWS){
        WARN(msg, "Size mismatch between resulting matrix and factors... resizing results");
        res->resize(NROWS,col);
    }
    
    // Multiply
    const size_t ncols = vec->nrows();
    for (int row = 0; row < NROWS; row++) {
        double v = 0;
        for (int i = 0; i < ncols; i++) {
            v += (data[row][i] * (*vec)[i]->at(0));
        }
        (*res)[row]->at(col) = v;
    }
    
    return true;
}

void Matrix::setElement(size_t row, size_t col, double value)
{
    data[row][col] = value;
}

