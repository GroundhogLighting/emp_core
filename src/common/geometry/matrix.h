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

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

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





/*
 Almost copied and pasted from Reinders, James.
 Intel Threading Building Blocks: Outfitting C++ for Multi-core Processor
 Parallelism. O'Reilly Media. Kindle Edition.
 */


// Example 11-35. The StrassenMultiply task

/*
 * Perform A x B => C for square submatrices of
 * A, B, and C assuming the submatrix
 * dimension is divisible by two.
 *
 * First, decompose matrices as follows:
 *      n_2 = n/2 = order of partitioned matrices
 *
 *          ------------- ------------- -------------
 *      n/2 ! a11 ! a12 ! ! b11 ! b12 ! ! c11 ! c12 ! n/2
 *          !-----!-----!*!-----!-----! = !-----!-----!
 *      n/2 ! a21 ! a22 ! ! b21 ! b22 ! ! c21 ! c22 ! n/2
 *          ------------- ------------- -------------
 *            n/2   n/2     n/2   n/2     n/2   n/2
 *
 *      algorithm:
 * Then, compute temp. Matrices as follows:
 *      p1 = (a11+a22)*(b11+b22)
 *      p2 = (a21+a22)*b11
 *      p3 = a11*(b12-b22)
 *      p4 = a22*(b21-b11)
 *      p5 = (a11+a12)*b22
 *      p6 = (a21-a11)*(b11+b12)
 *      p7 = (a12-a22)*(b21+b22)
 *
 * In the end, when all temp. matrices are ready,
 * compute the result matrix C:
 *      c11 = p1+p4-p5+p7
 *      c12 = p3+p5
 *      c21 = p2+p4
 *      c22 = p1+p3-p2+p6
 *
 * Each matrix multiplication is implemented as a
 * recursive call to strassen_mult.
 */
/*
class StrassenMultiply : public tbb::task {
    int n, ax, ay, as, bx, by, bs, cx, cy, cs, d, s;
    Matrix *A, *B, *C;
public:
    // dimensions of A, B, and C submatrices
    StrassenMultiply ( int _n,
                      // (ax,ay) = origin of A submatrix for multiplicand
                      Matrix * A, int _ax, int _ay, int _as,
                      // (bx,by) = origin of B submatrix for multiplicand
                      Matrix * B, int _bx, int _by, int _bs,
                      // (cx,cy) = origin of C submatrix for result
                      Matrix * C, int _cx, int _cy, int _cs,
                      // current depth of Strassen's recursion
                      int _d,
                      // Strassen's recursion limit for array dimensions
                      int _s
                      )//: n(_n), A(_A), B(_B), C(_C),
    //ax(_ax), ay(_ay), as(_as), bx(_bx), by(_by), bs(_bs),
    //cx(_cx), cy(_cy), cs(_cs), d(_d), s(_s)
    {}
    
    tbb::task* execute ( ) {
        if (n < s) {
            // just do the simple algorithm for small matrices
            matrix_mult(n, n, n, A, ax, ay, as, B, bx, by, bs, C, cx, cy, cs, d);
        } else {
            
            // we are going to create a list of seven tasks -
            // those tasks may create seven more each
            tbb::task_list list;
            int count = 1;
            int n_2 = n >> 1;
            double *work;
            double *p1, *p2, *p3, *p4, *p5, *p6, *p7;
            
            work = (double *) alloca (sizeof(double) * n_2 * n_2 * 9);
            p1 = work;
            p2 = p1 + n_2 * n_2;
            p3 = p2 + n_2 * n_2;
            p4 = p3 + n_2 * n_2;
            p5 = p4 + n_2 * n_2;
            p6 = p5 + n_2 * n_2;
            p7 = p6 + n_2 * n_2;
            
            // p1 = (a11 + a22) x (b11 + b22)
            double* a_cum1 = (double *) alloca (sizeof(double) * n_2 * n_2);
            double* b_cum1 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_add(n_2, n_2, A, ax, ay, as, A, ax+n_2, ay+n_2, as, a_cum1, 0, 0, n_2);
            
            matrix_add(n_2, n_2, B, bx, by, bs, B, bx+n_2, by+n_2, bs, b_cum1, 0, 0, n_2);
            ++count;
            
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, a_cum1, 0, 0, n_2, b_cum1, 0, 0, n_2, p1, 0, 0, n_2, d+1, s));
            
            // p2 = (a21 + a22) x b11
            double* a_cum2 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_add(n_2, n_2, A, ax+n_2, ay, as, A, ax+n_2, ay+n_2, as, a_cum2, 0, 0, n_2);
            ++count;
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, a_cum2, 0, 0, n_2, B, bx, by, bs, p2, 0, 0, n_2, d+1, s));
            
            // p3 = a11 x (b12 - b22)
            double* b_cum3 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_sub(n_2, n_2, B, bx, by+n_2, bs, B, bx+n_2, by+n_2, bs, b_cum3, 0, 0, n_2);
            ++count;
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, A, ax, ay, as, b_cum3, 0, 0, n_2, p3, 0, 0, n_2, d+1, s));
            
            // p4 = a22 x (b21 - b11)
            double* b_cum4 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_sub(n_2, n_2, B, bx+n_2, by, bs, B, bx, by, bs, b_cum4, 0, 0, n_2);
            ++count;
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, A, ax+n_2, ay+n_2, as, b_cum4, 0, 0, n_2, p4, 0, 0, n_2, d+1, s));
            
            // p5 = (a11 + a12) x b22
            double* a_cum5 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_add(n_2, n_2, A, ax, ay, as, A, ax, ay+n_2, as, a_cum5, 0, 0, n_2);
            ++count;
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, a_cum5, 0, 0, n_2, B, bx+n_2, by+n_2, bs, p5, 0, 0, n_2, d+1, s));
            
            // p6 = (a21 - a11) x (b11 + b12)
            double* a_cum6 = (double *) alloca (sizeof(double) * n_2 * n_2);
            double* b_cum6 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_sub(n_2, n_2, A, ax+n_2, ay, as, A, ax, ay, as, a_cum6, 0, 0, n_2);
            matrix_add(n_2, n_2, B, bx, by, bs, B, bx, by+n_2, bs, b_cum6, 0, 0, n_2);
            ++count;
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, a_cum6, 0, 0, n_2, b_cum6, 0, 0, n_2, p6, 0, 0, n_2, d+1, s));
            
            // p7 = (a12 - a22) x (b21 + b22)
            double* a_cum7 = (double *) alloca (sizeof(double) * n_2 * n_2);
            double* b_cum7 = (double *) alloca (sizeof(double) * n_2 * n_2);
            matrix_sub(n_2, n_2, A, ax, ay+n_2, as, A, ax+n_2, ay+n_2, as, a_cum7, 0, 0, n_2);
            matrix_add(n_2, n_2, B, bx+n_2, by, bs, B, bx+n_2, by+n_2, bs, b_cum7, 0, 0, n_2);
            ++count;
            list.push_back (*new (allocate_child ( ))
                            StrassenMultiply (n_2, a_cum7, 0, 0, n_2, b_cum7, 0, 0, n_2, p7, 0, 0, n_2, d+1, s));
            set_ref_count (count);
            spawn_and_wait_for_all (list);
            
            // c11 = p1 + p4 - p5 + p7
            matrix_add(n_2, n_2, p1, 0, 0, n_2, p4, 0, 0, n_2, C, cx, cy, cs);
            matrix_sub(n_2, n_2, C, cx, cy, cs, p5, 0, 0, n_2, C, cx, cy, cs);
            matrix_add(n_2, n_2, C, cx, cy, cs, p7, 0, 0, n_2, C, cx, cy, cs);
            
            // c12 = p3 + p5
            matrix_add(n_2, n_2, p3, 0, 0, n_2, p5, 0, 0, n_2, C, cx, cy+n_2, cs);
            
            // c21 = p2 + p4
            matrix_add(n_2, n_2, p2, 0, 0, n_2, p4, 0, 0, n_2, C, cx+n_2, cy, cs);
            
            // c22 = p1 + p3 - p2 + p6
            matrix_add(n_2, n_2, p1, 0, 0, n_2, p3, 0, 0, n_2, C, cx+n_2, cy+n_2, cs);
            matrix_sub(n_2, n_2, C, cx+n_2, cy+n_2, cs, p2, 0, 0, n_2, C, cx+n_2, cy+n_2, cs);
            matrix_add(n_2, n_2, C, cx+n_2, cy+n_2, cs, p6, 0, 0, n_2, C, cx+n_2, cy+n_2, cs);
        } // end of else
        return NULL;
    }
};


// Example 11-3. strassen_mult_par ( Strassen parallel)

void strassen_mult_par (
                        // dimensions of A, B, and C submatrices
                        int n,
                        // (ax,ay) = origin of A submatrix for multiplicand
                        double *A, int ax, int ay, int as,
                        // (bx,by) = origin of B submatrix for multiplicand
                        double *B, int bx, int by, int bs,
                        // (cx,cy) = origin of C submatrix for result
                        double *C, int cx, int cy, int cs,
                        // current depth of Strassen's recursion
                        int d,
                        // Strassen's recursion limit for array dimensions
                        int s
                        )
{
    StrassenMultiply& t = *new (tbb::task::allocate_root ())
    StrassenMultiply (n, A, ax, ay, as,B, bx, by, bs, C, cx, cy, cs, d, s);
    tbb::task::spawn_root_and_wait (t);
}
*/

/*
 
 // Example 11-36. The main program to call the various matrix multiply routines
 
 void init (size_t size, double* A, double* B) {
 srand((unsigned int)time(NULL));
 for (size_t i = 0; i < size; i++) {
 for (size_t j = 0; j < size; j++) {
 A[i*size + j] = ((float) rand( )) / ((float) RAND_MAX);
 B[i*size + j] = ((float) rand( )) / ((float) RAND_MAX);
 }
 }
 }
 
 int main(int argc, char *argv[])
 {
 double *A, *B, *C, *D, *E;
 // Allocate array storage
 A = new double [size * size];
 B = new double [size * size];
 C = new double [size * size];
 D = new double [size * size];
 E = new double [size * size];
 // Set up input matrices with random values
 init (size, A, B);
 
 std::cerr << "Parameters:" << std::endl <<
 " array size: " << size << "x" << size << std::endl <<
 " threads: " << threadreq << std::endl <<
 " strassen minimum: " << strass << std::endl <<
 " matmul blocksize: " << block << std::endl;
 
 // Blocked serial matrices multiplication
 tbb::tick_count simple1 = tbb::tick_count::now ( );
 matrix_mult (size, size, size,
 A, 0, 0, size,
 B, 0, 0, size,
 D, 0, 0, size, 0);
 tbb::tick_count simple2 = tbb::tick_count::now ( );
 std::cerr << "Simple serial algorithm: " <<
 (simple2-simple1).seconds ( ) << " seconds" << std::endl;
 
 // Strassen algorithm [Serial]
 tbb::tick_count serial1 = tbb::tick_count::now ( );
 strassen_mult (size, A, 0, 0, size,
 B, 0, 0, size,
 C, 0, 0, size, 1, strass);
 
 tbb::tick_count serial2 = tbb::tick_count::now ( );
 std::cerr << "Strassen algorithm serial: " <<
 (serial2-serial1).seconds ( ) << " seconds" << std::endl;
 
 // Strassen algorithm [Parallel]
 tbb::task_scheduler_init init (threadreq);
 tbb::tick_count parallel1 = tbb::tick_count::now ( );
 strassen_mult_par (size, A, 0, 0, size,
 B, 0, 0, size,
 E, 0, 0, size, 1, strass);
 tbb::tick_count parallel2 = tbb::tick_count::now ( );
 std::cerr << "Strassen algorithm parallel: " <<
 (parallel2-parallel1).seconds ( ) << " seconds" << std::endl;
 
 delete[] A;
 delete[] B;
 delete[] C;
 delete[] D;
 delete[] E;
 
 return 0;
 }
 */

#endif
