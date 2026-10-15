#include <iostream>
#include <ctime>
#include <vector>
#include <optional>


struct Matrix{
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;
};


Matrix transpose(Matrix mat){

    std::vector<std::vector<double>> new_data;

    for (int colIdx{0}; colIdx < mat.cols; colIdx++){
        std::vector<double> transRow;
        for (int rowIdx{0}; rowIdx < mat.cols; rowIdx++){
            transRow.push_back(mat.data[rowIdx][colIdx]);
        }
        new_data.push_back(transRow);
    }

    return Matrix{new_data, mat.cols, mat.rows};

}



void printMatrixData(Matrix mat){
    std::cout << "[ " << "\n";
    for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++){
        std::cout << "[ ";
        for (int colIdx{0}; colIdx < mat.cols; colIdx++){
            std::cout << mat.data[rowIdx][colIdx] << " ";
        }
        std::cout << " ]" << "\n";
    }
    std::cout << "]" << "\n";

}

double dotProduct(std::vector<double> vecA, std::vector<double> vecB){
    double dotProd{0.0};
    for (int rowIdx; rowIdx < vecA.size(); rowIdx++){
        dotProd += (vecA[rowIdx] * vecB[rowIdx]);
    }
    return dotProd;
}


std::optional<Matrix> multiplyMatrices(Matrix matA, Matrix matB){
    if (matA.cols != matB.rows){
        return std::nullopt;
    }

    std::vector<std::vector<double>> multData(matA.rows, std::vector<double>(matB.cols,0.0));
    for (int matArowIdx{0}; matArowIdx < matA.rows; matArowIdx++){
        for (int matBcolIdx{0}; matBcolIdx < matB.cols; matBcolIdx++){
            double dotProd{0.0};
            for (int k{0}; k < matA.cols; k++){
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



std::vector<std::vector<int>> heapPermuations(std::vector<int> vec, int size){
    std::vector<std::vector<int>> heapPerms;
    if (size == 1){
        heapPerms.push_back(vec);
        return heapPerms;
    }

    for (int idx{0}; idx < size; idx++){
        std::vector<std::vector<int>> heap = heapPermuations(vec, size-1);

        for (int pIdx{0}; pIdx < heap.size(); pIdx++){
            heapPerms.push_back(heap[pIdx]);
        }

        if (rem_euclid(size, 2) == 1){
            int a = vec[0];
            int b = vec[size-1];
            vec[0] = b;
            vec[size-1] = a;
        } else {
            int a = vec[idx];
            int b = vec[size-1];
            vec[idx] = b;
            vec[size-1] =a;
        }


    }
    return heapPerms;
}


std::vector<std::vector<int>> getPerms(int size){
    std::vector<int> vec;
    for (int i{0}; i < size; i++){
        vec.push_back(i);
    }
    std::vector<std::vector<int>> permutations = heapPermuations(vec, size);
    return permutations;
}


int modifiedBubbleSort(std::vector<int> perm){
    int operations{0};

    for (int pIdx{0}; pIdx < perm.size(); pIdx++){
        bool swapped = false;
        int upperBound = perm.size() - pIdx - 1;
        
        for (int bIdx{0}; bIdx < upperBound; bIdx++){
            if (perm[bIdx] > perm[bIdx+1]){
                int a = perm[bIdx];
                int b = perm[bIdx+1];
                perm[bIdx+1] = a;
                perm[bIdx] = b;
                operations +=1;
                swapped = true;
            }
        }
        if (!swapped){
            return operations;
        }

    }
    return operations;
}


int get_permutation_sign(std::vector<int> perm){
    int operations = modifiedBubbleSort(perm);
    if (rem_euclid(operations, 2) == 2) {
        return 1;
    } else {
        return -1;
    }
}


double getDeterminant(Matrix mat){
    double determinant{0.0};
    std::vector<std::vector<int>> perms = getPerms(mat.rows);

    for (int pIdx{0}; pIdx < perms.size(); pIdx++){
        int sign = get_permutation_sign(perms[pIdx]);
        double term{1.0};
        std::vector<int> perm = perms[pIdx];
        for (int idx{0}; idx < mat.rows; idx++) {
            term *= mat.data[idx][perm[idx]];
        }
        determinant += (sign * term);
    }

    return determinant;
}

Matrix cramerInverse(Matrix mat){

    // step 1. get matrix determinant
    double determinant = getDeterminant(mat);
    // step 2. get adjugate matrix

    // step3. divide adjugate matrix by determinant

    // step 4. return result


}



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





    std::cout << "Linear Regression" << "\n";
    std::cout << "-----------------" << "\n";


}