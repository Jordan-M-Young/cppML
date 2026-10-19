#include <iostream>
#include <ctime>
#include <vector>
#include <optional>
#include "utils.h"

struct Matrix {
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;
};


Matrix scalarDivide(Matrix mat, double scalar){
    std::vector<std::vector<double>> dividedData; 

    for (int rowIdx{0}; rowIdx < mat.rows;rowIdx++){
        std::vector<double> dividedRow;
        for (int colIdx{0}; colIdx < mat.cols; colIdx++){
            dividedRow.push_back(mat.data[rowIdx][colIdx]/scalar);
        }
        dividedData.push_back(dividedRow);
    }
    return Matrix{dividedData, mat.rows, mat.cols};
}


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


Matrix getMinor(int rowPosition, int colPosition, Matrix mat){
    std::vector<std::vector<double>> minorData;

    for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++){
        if (rowIdx == rowPosition){
            continue;
        }
        std::vector<double> minorRow;

        for (int colIdx{0}; colIdx < mat.cols; colIdx++){
            if (colIdx == colPosition){
                continue;
            }
            minorRow.push_back(mat.data[rowIdx][colIdx]);
        }
        minorData.push_back(minorRow);
    }

    return Matrix{minorData, minorData.size(), minorData[0].size()};

}


Matrix getCofactorMatrix(Matrix mat){

    std::vector<std::vector<double>> cofactorData;
    int rowCount{0};

    for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++){
        std::vector<double> cofactorRow;
        int colCount{0};

        for (int colIdx{0}; colIdx < mat.cols; colIdx++){
            Matrix minor = getMinor(rowIdx, colIdx, mat);
            double minorDet = getDeterminant(minor);

            int exp = rowCount + colCount + 2;
            int sign = pow(-1,exp);
            double cofactor = sign * minorDet;
            cofactorRow.push_back(cofactor);
            colCount += 1;
        }
        cofactorData.push_back(cofactorRow);
        rowCount +=1;
    }

    return Matrix{cofactorData, cofactorData.size(), cofactorData[0].size()};

}

Matrix getAdjugateMatrix(Matrix mat){

    Matrix cofactorMat = getCofactorMatrix(mat);
    Matrix transCofactorMat = transpose(cofactorMat);

    return transCofactorMat;
}



Matrix cramerInverse(Matrix mat){



    // step 1. get matrix determinant
    double determinant = getDeterminant(mat);
    // step 2. get adjugate matrix
    Matrix adjugate = getAdjugateMatrix(mat);
    // step3. divide adjugate matrix by determinant

    Matrix divAdjugate = scalarDivide(adjugate, determinant);
    // step 4. return resuult

    return divAdjugate;
}


Matrix linearRegression(Matrix features, Matrix targets){
        // XT
      Matrix tmat = transpose(features);
  
      // XTX
      auto prod = multiplyMatrices(tmat, features);

      Matrix left = cramerInverse(prod.value());
  
      auto right = multiplyMatrices(tmat, targets);
  
      auto betas = multiplyMatrices(left, right.value());

      return betas.value();
}

double getStDev(std::vector<double> samples) {
    double stDev;
    double sum{0.0};
    double sqsum{0.0};
    double avg;
    size_t nsamples = samples.size();
  
    for (int idx{0}; idx < samples.size(); idx++) {
      sum += samples[idx];
    }
    avg = sum / samples.size();
  
    for (int idx{0}; idx < nsamples; idx++) {
      samples[idx] = std::pow(samples[idx] - avg, 2);
    }
  
    for (int idx{0}; idx < nsamples; idx++) {
      sqsum += samples[idx];
    }
  
    return sqsum / nsamples;
  }



  void printVecString(std::vector<std::string> &vec) {
    std::cout << "[ ";
    for (size_t idx{0}; idx < vec.size(); idx++) {
        std::string cell = vec[idx];
        
        // Clean out any hidden carriage returns causing terminal overwrite
        if (!cell.empty() && cell.back() == '\r') {
            cell.pop_back();
        }
        
        std::cout << cell;
        if (idx < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]\n";
}

void printVecVecString(std::vector<std::vector<std::string>> &vecVec) {
    std::cout << "[\n";
    for (size_t idx{0}; idx < vecVec.size(); idx++) {
        std::cout << "  "; // Indent rows for readability
        printVecString(vecVec[idx]);
    }
}


Matrix getTargets(std::vector<std::vector<std::string>> &csvData){
    std::vector<std::vector<double>> targets;

    for (int idx{1}; idx < csvData.size(); idx++){
        std::vector<double> targetRow;
        targetRow.push_back(std::stod(csvData[idx][0]));
        targets.push_back(targetRow);
    }

    return Matrix{targets, targets.size(), targets[0].size()};
}

Matrix getFeatures(std::vector<std::vector<std::string>> &csvData){
    std::vector<std::vector<double>> features;

    for (int idx{1}; idx < csvData.size(); idx++){
        std::vector<double> featureRow;
        for (int jdx{1}; jdx < 8; jdx++){
            featureRow.push_back(std::stod(csvData[idx][jdx]));

        }
        features.push_back(featureRow);
    }

    return Matrix{features, features.size(), features[0].size()};
}

int main(){
    const char* filename =  "./data/mpg.csv";
    std::vector<std::vector<std::string>> csvData = loadCSV(filename);

    Matrix targets = getTargets(csvData);
    Matrix features = getFeatures(csvData);


    std::cout << "Linear Regression" << '\n';
    std::cout << "--------------" << "\n";
  
    int iterations = 1000;
    double sum{0};
    std::vector<double> samples;
  
    for (int idx{0}; idx < iterations; idx++) {
      auto start = std::chrono::steady_clock::now();
      Matrix betas = linearRegression(features, targets);
    //   printMatrixData(betas);
      auto end = std::chrono::steady_clock::now();
      std::chrono::duration<double, std::milli> dur = end - start;
  
      // std::cout << "Total Time (ns): " << dur.count() << "\n";
      double duration = dur.count();
      sum += duration;
      samples.push_back(duration);
    }
  
    double stDev = getStDev(samples);
    std::cout << "N Samples: " << targets.data.size() << " elements" << "\n";
    std::cout << "N Features: " << targets.data.size() << "\n";
    std::cout << "Total Time (ms): " << sum << "\n";
    std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";
    std::cout << "StDev (ms): +/- " << stDev << "\n";
  }



