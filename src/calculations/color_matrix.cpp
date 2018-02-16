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


#include "./color_matrix.h"
#include "common/utilities/io.h"

ColorMatrix::ColorMatrix()
{
    red = Matrix();
    green = Matrix();
    blue = Matrix();
}

ColorMatrix::ColorMatrix(size_t nrows, size_t ncols )
{
    red = Matrix(nrows,ncols);
    green = Matrix(nrows,ncols);
    blue = Matrix(nrows,ncols);
}

size_t ColorMatrix::ncols()
{
    return red.ncols();
}

size_t ColorMatrix::nrows()
{
    return red.nrows();
}

Matrix * ColorMatrix::redChannel()
{
    return &red;
}

Matrix * ColorMatrix::greenChannel()
{
    return &green;
}

Matrix * ColorMatrix::blueChannel()
{
    return &blue;
}

bool ColorMatrix::multiply(ColorMatrix * m, ColorMatrix * res)
{
    red.multiply(m->redChannel(), res->redChannel());
    green.multiply(m->greenChannel(), res->greenChannel());
    blue.multiply(m->blueChannel(), res->blueChannel());
    
    return true;
}

bool ColorMatrix::multiplyToColumn(ColorMatrix * vec, size_t col, ColorMatrix * res)
{
    red.multiplyToColumn(vec->redChannel(),col, res->redChannel());
    green.multiplyToColumn(vec->greenChannel(),col, res->greenChannel());
    blue.multiplyToColumn(vec->blueChannel(),col, res->blueChannel());
    
    return true;
}

void ColorMatrix::resize(size_t nrows, size_t ncols)
{
    red.resize(nrows,ncols);
    green.resize(nrows,ncols);
    blue.resize(nrows,ncols);
}

void ColorMatrix::calcIrradiance(Matrix * result)
{
    size_t cols = ncols();
    size_t rows = nrows();
    
    // Check size match
    if(cols != result->ncols() || rows != result->nrows()){
        WARN(msg,"Inconsistent size of result matrix when calcIrradiance... resizing");
        result->resize(rows,cols);
    }
    
    double r,g,b;
    for(size_t col = 0; col < cols; col++){
        for(size_t row=0;row<rows;row++){
            r = red.getElement(row,col);
            g = green.getElement(row,col);
            b = blue.getElement(row,col);
            result->setElement(row,col,0.265*r + 0.67*g + 0.065*b);
        }
    }
    
}

void ColorMatrix::calcIlluminance(Matrix * result)
{
    size_t cols = ncols();
    size_t rows = nrows();
    
    // Check size match
    if(cols != result->ncols() || rows != result->nrows()){
        WARN(msg,"Inconsistent size of result matrix when calcIrradiance... resizing");
        result->resize(rows,cols);
    }
    
    double r,g,b;
    for(size_t col = 0; col < cols; col++){
        for(size_t row=0;row<rows;row++){
            r = red.getElement(row,col);
            g = green.getElement(row,col);
            b = blue.getElement(row,col);
            result->setElement(row,col,47.5*r + 119.95*g + 11.60*b);
        }
    }
    
}


