#include <iostream>
#include <ctime>
#include <vector>
#include <optional>

struct Matrix {
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;
};

Matrix transpose(Matrix mat) {
    std::vector<std::vector<double>> new_data;

    for (size_t colIdx{0}; colIdx < mat.cols; colIdx++) {
        std::vector<double> transRow;
        for (size_t rowIdx{0}; rowIdx < mat.rows; rowIdx++) { // Fixed boundary
            transRow.push_back(mat.data[rowIdx][colIdx]);
        }
        new_data.push_back(transRow);
    }

    return Matrix{new_data, mat.cols, mat.rows};
}

void printMatrixData(Matrix mat) {
    std::cout << "[ " << "\n";
    for (size_t rowIdx{0}; rowIdx < mat.rows; rowIdx++) {
        std::cout << "[ ";
        for (size_t colIdx{0}; colIdx < mat.cols; colIdx++) {
            std::cout << mat.data[rowIdx][colIdx] << " ";
        }
        std::cout << " ]" << "\n";
    }
    std::cout << "]" << "\n";
}

double dotProduct(std::vector<double> vecA, std::vector<double> vecB) {
    double dotProd{0.0};
    for (size_t rowIdx{0}; rowIdx < vecA.size(); rowIdx++) { // Fixed initialization
        dotProd += (vecA[rowIdx] * vecB[rowIdx]);
    }
    return dotProd;
}

std::optional<Matrix> multiplyMatrices(Matrix matA, Matrix matB) {
    if (matA.cols != matB.rows) {
        return std::nullopt;
    }

    std::vector<std::vector<double>> multData(matA.rows, std::vector<double>(matB.cols, 0.0));
    for (size_t matArowIdx{0}; matArowIdx < matA.rows; matArowIdx++) {
        for (size_t matBcolIdx{0}; matBcolIdx < matB.cols; matBcolIdx++) {
            double dotProd{0.0};
            for (size_t k{0}; k < matA.cols; k++) {
                dotProd += matA.data[matArowIdx][k] * matB.data[k][matBcolIdx];
            }
            multData[matArowIdx][matBcolIdx] = dotProd;
        }
    }
    return Matrix{multData, matA.rows, matB.cols};
}

int rem_euclid(int a, int b) {
    int r = a % b;
    if (r < 0) {
        r += (b < 0 ? -b : b);
    }
    return r;
}

// Fixed: vec is now passed by reference to correctly update states globally across loops
void heapPermutations(std::vector<int>& vec, int size, std::vector<std::vector<int>>& heapPerms) {
    if (size == 1) {
        heapPerms.push_back(vec);
        return;
    }

    for (int idx{0}; idx < size; idx++) {
        heapPermutations(vec, size - 1, heapPerms);

        if (rem_euclid(size, 2) == 1) {
            std::swap(vec[0], vec[size - 1]);
        } else {
            std::swap(vec[idx], vec[size - 1]);
        }
    }
}

std::vector<std::vector<int>> getPerms(int size) {
    std::vector<int> vec;
    for (int i{0}; i < size; i++) {
        vec.push_back(i);
    }
    std::vector<std::vector<int>> permutations;
    heapPermutations(vec, size, permutations);
    return permutations;
}

int modifiedBubbleSort(std::vector<int> perm) {
    int operations{0};

    for (size_t pIdx{0}; pIdx < perm.size(); pIdx++) {
        bool swapped = false;
        size_t upperBound = perm.size() - pIdx - 1;
        
        for (size_t bIdx{0}; bIdx < upperBound; bIdx++) {
            if (perm[bIdx] > perm[bIdx + 1]) {
                std::swap(perm[bIdx], perm[bIdx + 1]);
                operations += 1;
                swapped = true;
            }
        }
        if (!swapped) {
            return operations;
        }
    }
    return operations;
}

int get_permutation_sign(std::vector<int> perm) {
    int operations = modifiedBubbleSort(perm);
    if (rem_euclid(operations, 2) == 0) { // Fixed: checking for 0 instead of 2
        return 1;
    } else {
        return -1;
    }
}

double getDeterminant(Matrix mat) {
    double determinant{0.0};
    std::vector<std::vector<int>> perms = getPerms(mat.rows);

    for (size_t pIdx{0}; pIdx < perms.size(); pIdx++) {
        int sign = get_permutation_sign(perms[pIdx]);
        double term{1.0};
        std::vector<int> perm = perms[pIdx];
        for (size_t idx{0}; idx < mat.rows; idx++) {
            term *= mat.data[idx][perm[idx]];
        }
        determinant += (sign * term);
    }

    return determinant;
}

// Matrix cramerInverse(Matrix mat){

//     // step 1. get matrix determinant
//     double determinant = getDeterminant(mat);
//     // step 2. get adjugate matrix

//     // step3. divide adjugate matrix by determinant

//     // step 4. return result


// }



int main(){

    // b=(XTX)−1XTy
    std::vector<std::vector<double>> data = {{1.0,2.0,3.0},{4.0,5.0,6.0},{7.0,8.0,9.0}};
    Matrix mat = Matrix{data, data.size(), data[0].size()};

    // X
    std::cout << "Standard" << "\n";
    printMatrixData(mat);

    // XT
    std::cout << "Transpose" << "\n";
    Matrix tmat = transpose(mat);
    printMatrixData(tmat);

    // XTX
    auto prod = multiplyMatrices(mat, tmat);
    std::cout << "Multiplied" << "\n";
    printMatrixData(prod.value());


    double determinant = getDeterminant(mat);
    std::cout << "Mat Determinant: " << determinant << "\n";



    std::cout << "Linear Regression" << "\n";
    std::cout << "-----------------" << "\n";


}