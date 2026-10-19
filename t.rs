use crate::{error::CustomErrors, matrix::Matrix};

use super::operations::get_determinant;

pub fn get_minor<T: Copy + Into<f64> + std::convert::From<u8> + std::ops::AddAssign>(
    row_pos: usize,
    col_pos: usize,
    matrix: &Matrix<T>,
) -> Result<Matrix<T>, CustomErrors> {
    // worst solution in terms of O(T) lol
    // TODO implement dynamic solution for getting minors of a matrix?
    // or maybe some other solution besides O(n^2) for each element

    let n = matrix.n;
    let m = matrix.m;
    let rows = &matrix.rows;

    let mut minor_rows: Vec<Vec<T>> = vec![];

    for i in 0..m {
        if i == row_pos {
            continue;
        }
        let mut minor_row: Vec<T> = vec![];

        for j in 0..n {
            if j == col_pos {
                continue;
            }
            minor_row.push(rows[i][j])
        }
        minor_rows.push(minor_row)
    }

    // TODO add result matching....
    match Matrix::new(minor_rows) {
        Ok(matrix) => Ok(matrix),
        Err(err) => Err(err),
    }
}

pub fn get_cofactor<
    T: Copy
        + std::ops::MulAssign
        + std::fmt::Debug
        + std::convert::From<u8>
        + std::convert::From<i32>
        + std::ops::Mul
        + std::ops::Mul<Output = T>
        + Into<f64>
        + std::ops::AddAssign,
>(
    matrix: &Matrix<T>,
) -> Result<Matrix<T>, CustomErrors> {
    let m = matrix.m;
    let n = matrix.n;
    let mut cofactor_rows: Vec<Vec<T>> = vec![];
    let neg_one_cast: i32 = -1;

    let mut row_count = 0;
    for i in 0..m {
        let mut cofactor_row: Vec<T> = vec![];
        let mut col_count = 0;
        for j in 0..n {
            let element_minor = match get_minor(i, j, &matrix) {
                Ok(minor) => minor,
                Err(err) => return Err(err),
            };
            let minor_det = match get_determinant(&element_minor) {
                Ok(det) => det,
                Err(err) => return Err(err),
            };

            // since im zero indexing we need to translate into normal matrix
            // nomeclature i.e. one indexing by adding two to our exp sum
            // so 0 + 0 + 2 == 1 + 1 etc

            let exp: u32 = row_count + col_count + 2;
            let sign = neg_one_cast.pow(exp);
            let sign: T = sign.into();
            let cofactor = sign * minor_det;
            cofactor_row.push(cofactor);
            col_count += 1
        }
        cofactor_rows.push(cofactor_row);
        row_count += 1
    }

    match Matrix::new(cofactor_rows) {
        Ok(matrix) => Ok(matrix),
        Err(err) => Err(err),
    }
}
