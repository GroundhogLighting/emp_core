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


#include "./color_matrix.h"
#include "../common/utilities/io.h"


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

size_t ColorMatrix::ncols() const
{
    return red.ncols();
}

size_t ColorMatrix::nrows() const
{
    return red.nrows();
}

const Matrix * ColorMatrix::redChannel() const
{
    return &red;
}

const Matrix * ColorMatrix::greenChannel() const
{
    return &green;
}

const Matrix * ColorMatrix::blueChannel() const
{
    return &blue;
}


Matrix * ColorMatrix::r()
{
    return &red;
}

Matrix * ColorMatrix::g()
{
    return &green;
}

Matrix * ColorMatrix::b()
{
    return &blue;
}


bool ColorMatrix::multiply(const ColorMatrix * m, ColorMatrix * res) const
{
    
    red.multiply(m->redChannel(), res->r());
    green.multiply(m->greenChannel(), res->g());
    blue.multiply(m->blueChannel(), res->b());
    
    return true;
}

bool ColorMatrix::multiplyToColumn(const ColorMatrix * vec, size_t col, ColorMatrix * res) const
{
    red.multiplyToColumn(vec->redChannel(),col, res->r());
    green.multiplyToColumn(vec->greenChannel(),col, res->g());
    blue.multiplyToColumn(vec->blueChannel(),col, res->b());
    
    return true;
}

bool ColorMatrix::multiplyRowToColumn( const ColorMatrix * vec, size_t row, size_t col, ColorMatrix * res) const
{
    red.multiplyRowToColumn(vec->redChannel(), row, col, res->r());
    green.multiplyRowToColumn(vec->greenChannel(), row, col, res->g());
    blue.multiplyRowToColumn(vec->blueChannel(), row, col, res->b());
    
    return true;
}

void ColorMatrix::resize(size_t nrows, size_t ncols)
{
    red.resize(nrows,ncols);
    green.resize(nrows,ncols);
    blue.resize(nrows,ncols);
}

void ColorMatrix::calcIrradiance(Matrix * result) const
{
    const size_t cols = ncols();
    const size_t rows = nrows();
    
    // Check size match
    if(cols != result->ncols() || rows != result->nrows()){
        WARN(msg,"Inconsistent size of result matrix when calcIrradiance... resizing");
        result->resize(rows,cols);
    }
    
    tbb::parallel_for(tbb::blocked_range<size_t>(0, cols),
                      [=](const tbb::blocked_range<size_t>& r1) {
                          for (size_t col = r1.begin(); col != r1.end(); ++col) {
                              
                              tbb::parallel_for(tbb::blocked_range<size_t>(0, rows),
                                                [=](const tbb::blocked_range<size_t>& r2) {
                                                    for (size_t row = r2.begin(); row != r2.end(); ++row) {
                                                        
                                                        auto r = red.getElement(row,col);
														auto g = green.getElement(row,col);
														auto b = blue.getElement(row,col);
                                                        result->setElement(row,col,0.265f*r + 0.67f*g + 0.065f*b);
                                                        
                                }
                            },
                            tbb::auto_partitioner()
                            );// end of loop in rows
                              
                          }
                      },
      tbb::auto_partitioner()
      );// end of loop in cols

    
}

void ColorMatrix::calcIlluminance(Matrix * result) const
{
    const size_t cols = ncols();
    const size_t rows = nrows();
    
    // Check size match
    if(cols != result->ncols() || rows != result->nrows()){
        WARN(msg,"Inconsistent size of result matrix when calcIlluminance... resizing");
        result->resize(rows,cols);
    }
    
    tbb::parallel_for(tbb::blocked_range<size_t>(0, cols),
                      [=](const tbb::blocked_range<size_t>& r1) {
                          for (size_t col = r1.begin(); col != r1.end(); ++col) {
                              
                              tbb::parallel_for(tbb::blocked_range<size_t>(0, rows),
                                                [=](const tbb::blocked_range<size_t>& r2) {
                                                    for (size_t row = r2.begin(); row != r2.end(); ++row) {
                                                        
														auto r = red.getElement(row,col);
														auto g = green.getElement(row,col);
														auto b = blue.getElement(row,col);
                                                        result->setElement(row,col,47.5f*r + 119.95f*g + 11.60f*b);
                                                        
                                                    }
                                                },
                                                tbb::auto_partitioner()
                                                );// end of loop in rows
                              
                          }
                      },
                      tbb::auto_partitioner()
                      );// end of loop in cols

    
    
}



float ColorMatrix::calcIrradiance(size_t row, size_t col) const
{
    const auto r = red.getElement(row,col);
    const auto g = green.getElement(row,col);
    const auto b = blue.getElement(row,col);
    return 0.265f*r + 0.67f*g + 0.065f*b;
}

float ColorMatrix::calcIlluminance(size_t row, size_t col) const
{
    const auto r = red.getElement(row,col);
    const auto g = green.getElement(row,col);
    const auto b = blue.getElement(row,col);
    return 47.5f*r + 119.95f*g + 11.60f*b;
    
}




