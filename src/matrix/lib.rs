#![allow(dead_code)]

use std::fmt::{self, Formatter};

pub struct Matrix{
    pub nrows: u32,
    pub ncols: u32,
    pub data: Vec<Vec<f32>>,
}

impl fmt::Display for Matrix {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        try!(write!(f,"\n"));
        for row in 0..self.data.len(){
            for col in 0..self.data[0].len() {
                try!(write!(f,"\t{}",self.data[row][col]));
            }
            try!(write!(f,"\n"));
        }
        write!(f,"\n")
    }
}


impl fmt::Debug for Matrix {
    // `f` is a buffer, this method must write the formatted string into it
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        write!(f,"Matrix({},{}) --> nrows: {}, ncols: {}",self.data.len(), self.data[0].len(),self.nrows,self.ncols)
    }
}


impl<'a> std::ops::Add for &'a Matrix {
    type Output = Matrix;

    fn add(self, other: &'a Matrix) -> Matrix {
        if self.nrows != other.nrows || self.ncols != other.ncols{
            panic!("Fatal: Adding matrices with incompatible dimensions");
        }

        let mut data = self.data.clone();
        for i in 0..self.data.len(){
            for j in 0..self.data[0].len(){
                data[i][j]+=other.data[i][j];
            }
        }

        Matrix{data: data, nrows:self.nrows, ncols:self.ncols}

    }
}

impl<'a> std::ops::Sub for &'a Matrix {
    type Output = Matrix;

    fn sub(self, other: &'a Matrix) -> Matrix {
        if self.nrows != other.nrows || self.ncols != other.ncols{
            panic!("Fatal: Substracting matrices with incompatible dimensions");
        }
        let mut data = self.data.clone();
        for i in 0..self.data.len(){
            for j in 0..self.data[0].len(){
                data[i][j]-=other.data[i][j];
            }
        }

        Matrix{data: data, nrows:self.nrows, ncols:self.ncols}
    }
}

impl<'a> std::ops::Mul for &'a Matrix {
    type Output = Matrix;

    fn  mul(self, other: &'a Matrix) -> Matrix {
        if self.ncols != other.nrows {
            panic!("Fatal: Multiplying matrices with incompatible dimensions");
        }
        let mut data = self.data.clone();
        for i in 0..self.data.len(){
            for j in 0..other.data[0].len(){
                data[i][j]=0.0;
                for k in 0..other.data[0].len(){
                    data[i][j]+=self.data[i][k]*other.data[k][j];
                }
            }
        }
        Matrix{data: data, nrows:self.nrows, ncols:other.ncols}
    }
}


impl Matrix {
    pub fn constant(nrows: u32, ncols: u32, value: f32) -> Matrix {
        // THIS MUST BE DONE MORE EFFICIENTLY!
        let mut data = vec![];
        for _ in 0..nrows{
            let mut aux = vec![];
            for _ in 0..ncols{
                aux.push(value);
            }
            data.push(aux);
        }

        Matrix {
            nrows: nrows,
            ncols: ncols,
            data: data
        }
    }

    pub fn new(nrows: u32, ncols: u32) -> Matrix {
        Matrix::constant(nrows, ncols, 0.0)
    }

    pub fn ones(nrows: u32, ncols: u32) -> Matrix {
        Matrix::constant(nrows,ncols,1.0)
    }

    pub fn zeros(nrows: u32, ncols: u32) -> Matrix {
        Matrix::constant(nrows,ncols,0.0)
    }


    pub fn scalar(nrows: u32, ncols: u32, value: f32) -> Matrix {
        // THIS MUST BE DONE MORE EFFICIENTLY!
        let mut data = vec![];
        for row in 0..nrows{
            let mut aux = vec![];
            for col in 0..ncols{
                if row == col {
                    aux.push(value);
                }else{
                    aux.push(0.0);
                }
            }
            data.push(aux);
        }
        Matrix {nrows: nrows,ncols: ncols,data: data}
    }

    pub fn eye(n: u32) -> Matrix {
        Matrix::scalar(n,n,1.0)
    }

    pub fn equal(&self, other: &Matrix) -> bool {
        for (i,row) in self.data.iter().enumerate(){
            for (j,&column) in row.iter().enumerate(){
                if column != other.data[i][j]{
                    return false
                }
            }
        }
        true
    }

    pub fn scale(&self, number: f32) -> Matrix {
        let mut data = vec![];
        for row in self.data.iter(){
            let mut aux = vec![];
            for &column in row.iter(){
                aux.push(column*number);
            }
            data.push(aux);
        }
        Matrix {nrows: self.nrows, ncols: self.ncols, data: data}
    }

}


/************************************/
/*************** TESTS **************/
/************************************/

#[test]
fn comparison() {
    let nrows=31;
    let ncols=32;

    let mut data = vec![];
    for _ in 0..nrows{
        let mut aux = vec![];
        for _ in 0..ncols{
            aux.push(2.123411);
        }
        data.push(aux);
    }

    let m =Matrix {nrows: nrows,ncols: ncols,data: data};

    if !m.equal(&m) {
        panic!("Comparison of a matrix with itself said they are different");
    }

}

#[test]
fn creation() {
    let nrows=6;
    let ncols=4;
    let zero = Matrix::new(nrows,ncols);
    let other = Matrix::constant(nrows,ncols,0.0);

    println!("zero(2,2):{}",zero);
    println!("constant(2,2,0.0):{}",other);

    if !zero.equal(&other) {
        panic!("zero(2,2) and constant(2,2,0.0) give different results");
    }

}

#[test]
fn scaling() {
    let nrows=12;
    let ncols=12;
    let value: f32=3.0;

    let one = Matrix::ones(nrows,ncols);
    let valued = Matrix::constant(nrows,ncols,value);

    let modified = one.scale(value);
    if !valued.equal(&modified) {
        println!("valued:{}",valued);
        println!("modified:{}",modified);
        panic!();
    }
}

#[test]
fn addition() {
    let nrows=12;
    let ncols=12;
    let value: f32=3.0;

    let one = Matrix::ones(nrows,ncols); //one
    let valued = Matrix::constant(nrows,ncols,value); //value
    let modified = one.scale(value); //value
    let operated=&valued+&modified; //value+value = 2*value
    if !operated.equal(&modified.scale(2.0)) {
        panic!();
    }
}

#[test]
#[should_panic]
fn addition_dimension_missmatch() {
    &Matrix::new(2,3)+&Matrix::new(4,2);
}

#[test]
fn substraction(){
    let nrows=12;
    let ncols=12;
    let value: f32=3.0;
    let one = Matrix::ones(nrows,ncols); //one
    let valued = Matrix::constant(nrows,ncols,value); //value
    let modified = one.scale(value); //value
    let operated=&valued-&modified; //value-value = 0

    if !operated.equal(&Matrix::new(nrows,ncols)) {
        panic!();
    }

}

#[test]
#[should_panic]
fn substraction_dimension_missmatch() {
    &Matrix::new(2,3)-&Matrix::new(4,2);
}

#[test]
fn product(){

    let n=12;
    let eye = Matrix::eye(n);
    let operated = &eye*&eye;
    if !operated.equal(&eye) {
        panic!();
    }

    let valued = Matrix::constant(2*n,n,2.123);
    if !valued.equal(&(&valued*&eye)) {
        println!("{}",&valued*&eye);
        panic!();
    }
}

#[test]
#[should_panic]
fn product_dimension_missmatch() {
    &Matrix::new(5,5)*&Matrix::new(1,3);
}
