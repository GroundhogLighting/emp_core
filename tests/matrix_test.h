#include "../include/emp_core.h"
//#include "common/geometry/matrix.h"


TEST(Matrix_TEST, BasicConstructor) {
    int nrows = 2;
    int ncols = 3;
    
    Matrix m = Matrix(nrows, ncols);
    
    ASSERT_EQ(m.nrows(),nrows);
    ASSERT_EQ(m.ncols(),ncols);
}


TEST(Matrix_TEST, Resize) {
    int nrows = 10;
    int ncols = 11;
    
    Matrix m = Matrix(2, 2);
    
    m.resize(nrows,ncols);
    ASSERT_EQ(m.nrows(),nrows);
    ASSERT_EQ(m.ncols(),ncols);
}

TEST(Matrix_TEST, MultiplicationFactorsSizeMismatch) {
    
    // Size mismatch in factors
    Matrix A = Matrix(2, 2);
    Matrix B = Matrix(4, 2);
    Matrix res = Matrix(2, 2);
    
    ASSERT_ANY_THROW(A.multiply(&B,&res));
}


TEST(Matrix_TEST, MultiplicationResultSizeMismatch) {
    // Size mismatch with result
    Matrix A = Matrix(5, 2);
    Matrix B = Matrix(2, 6);
    Matrix res = Matrix(3, 3);
    A.multiply(&B,&res); // expected resize
    ASSERT_EQ(res.ncols(),B.ncols());
    ASSERT_EQ(res.nrows(),A.nrows());
}

TEST(Matrix_TEST, Multiplication) {
    // Size mismatch with result
    Matrix A = Matrix(2, 4);
    A[0]->at(0) = 4; A[0]->at(1) = 0; A[0]->at(2) = 2; A[0]->at(3) = 3;
    A[1]->at(0) = 1; A[1]->at(1) = 5; A[1]->at(2) = 6; A[1]->at(3) = 7;
    
    Matrix B = Matrix(4, 2);
    B[0]->at(0) = 4; B[0]->at(1) = 8;
    B[1]->at(0) = 9; B[1]->at(1) = -2;
    B[2]->at(0) = 1; B[2]->at(1) = 0;
    B[3]->at(0) = 5; B[3]->at(1) = -3;
    
    Matrix res = Matrix(A.nrows(), B.ncols());
    
    A.multiply(&B,&res);
        
    ASSERT_EQ(res[0]->at(0),33);
    ASSERT_EQ(res[0]->at(1),23);
    ASSERT_EQ(res[1]->at(0),90);
    ASSERT_EQ(res[1]->at(1),-23);
}


TEST(Matrix_TEST, MultiplyLocate) {
    
    int nrows = 5;
    int ncols = 5;
    int destCol = rand()%ncols;
    
    Matrix res = Matrix(nrows,ncols);
    Matrix res2 = Matrix(nrows,1);
    Matrix A = Matrix(nrows,ncols);
    Matrix b = Matrix(nrows,1);
    
    // Fill with random
    for(int row=0; row < nrows; row++){
        b[row]->at(0)=rand();
        for(int col=0; col < ncols; col++){
            A[row]->at(col)=rand();
        }
    }
    
    
    A.multiply(&b,&res2);
    A.multiplyToColumn(&b,destCol,&res);
        
    // Check results
    for(int row = 0; row<nrows; row++){
        for(int col = 0; col < ncols; col++){
            if(col == destCol){
                ASSERT_EQ(res2[row]->at(0),res[row]->at(col));
            }else{
                ASSERT_EQ(res[row]->at(col),0.0);
            }
        }
    }
    
}
