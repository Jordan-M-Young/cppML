#include <cmath>
#include <ctime>
#include <iostream>
#include <numbers>
#include <vector>
double sigmoid(double z) {
  double e = std::numbers::e;
  return 1.0 / (1 + pow(e, -z));
}

double vectorSum(std::vector<double> vec) {
  double sum{0.0};
  for (int idx{0}; idx < vec.size(); idx++) {
    sum += vec[idx];
  }
  return sum;
}

struct Matrix {
  std::vector<std::vector<double>> data;
  size_t rows;
  size_t cols;
};

std::vector<double> vectorSub(std::vector<double> vec_a,
                              std::vector<double> vec_b) {
  std::vector<double> newVec;
  for (int rowIdx{0}; rowIdx < vec_a.size(); rowIdx++) {
    newVec.push_back(vec_a[rowIdx] - vec_b[rowIdx]);
  }
  return newVec;
}

std::vector<std::vector<double>> applyMatrixSigmoid(Matrix &mat) {
  std::vector<std::vector<double>> sigmoidData;
  for (int idx{0}; idx < mat.rows; idx++) {
    std::vector<double> rowData;
    for (int jdx{0}; jdx < mat.cols; jdx++) {
      double sigValue = sigmoid(mat.data[idx][jdx]);
      rowData.push_back(sigValue);
    }
    sigmoidData.push_back(rowData);
  }

  return sigmoidData;
}

std::vector<double> applyVectorSigmoid(std::vector<double> vec) {
  std::vector<double> sigmoidData;
  for (int idx{0}; idx < vec.size(); idx++) {
    sigmoidData.push_back(sigmoid(vec[idx]));
  }
  return sigmoidData;
}

std::vector<double> matrixVecDot(Matrix mat, std::vector<double> vec) {
  std::vector<double> dotVec;

  for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++) {
    double sm = 0.0;
    for (int colIdx{0}; colIdx < mat.cols; colIdx++) {
      sm += mat.data[rowIdx][colIdx] * vec[colIdx];
    }
    dotVec.push_back(sm);
  }
  return dotVec;
}

Matrix scalarMatrixAdd(Matrix mat, double scalar) {
  std::vector<std::vector<double>> newData;
  for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++) {
    std::vector<double> newRow;
    for (int colIdx{0}; colIdx < mat.cols; colIdx++) {
      newRow.push_back(mat.data[rowIdx][colIdx] + scalar);
    }
    newData.push_back(newRow);
  }
  return Matrix{newData, mat.rows, mat.cols};
}

std::vector<double> scalarVectorAdd(std::vector<double> vec, double scalar) {
  std::vector<double> newData;
  for (int rowIdx{0}; rowIdx < vec.size(); rowIdx++) {
    newData.push_back(vec[rowIdx] + scalar);
  }
  return newData;
}

std::vector<double> ScalaraVectorMult(std::vector<double> vec, double scalar) {
  std::vector<double> newData;
  for (int rowIdx{0}; rowIdx < vec.size(); rowIdx++) {
    newData.push_back(vec[rowIdx] * scalar);
  }
  return newData;
}

Matrix transpose(Matrix &mat) {
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

class LogisticRegression {
public:
  double learningRate;
  int iterations;
  double bias;
  std::vector<double> weights;
  std::vector<double> losses;

  void fit(Matrix features, std::vector<double> targets) {

    double gradientScaleFactor = 1.0 / features.rows;

    if (weights.empty()) {
      weights = std::vector<double>(features.cols, 0.0);
    }

    if (!bias) {
      bias = 0.0;
    }

    for (int iteration{0}; iteration < iterations; iteration++) {

      std::vector<double> z =
          scalarVectorAdd(matrixVecDot(features, weights), bias);

      std::vector<double> predictions = applyVectorSigmoid(z);
      std::vector<double> predTargDiff = vectorSub(predictions, targets);
      Matrix featuresT = transpose(features);
      std::vector<double> dw = ScalaraVectorMult(
          matrixVecDot(featuresT, predTargDiff), gradientScaleFactor);
      double db = gradientScaleFactor * vectorSum(predTargDiff);

      weights = vectorSub(weights, ScalaraVectorMult(dw, learningRate));
      bias -= (db * learningRate);
    }
  }
  std::vector<double> predict_proba(Matrix features) {
    return applyVectorSigmoid(
        scalarVectorAdd(matrixVecDot(features, weights), bias));
  }
};
void printVec(std::vector<double> vec) {
  std::cout << "[ ";
  for (int i{0}; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << "]" << "\n";
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
int main() {

  std::vector<std::vector<double>> featureData = {
      {0.50}, {0.75}, {1.00}, {1.25}, {1.50}, {1.75}, {1.75},
      {2.00}, {2.25}, {2.50}, {2.75}, {3.00}, {3.25}, {3.50},
      {4.00}, {4.25}, {4.50}, {4.75}, {5.00}, {5.50}};
  Matrix features =
      Matrix{featureData, featureData.size(), featureData[0].size()};
  std::vector<double> targets = {0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
                                 1, 0, 1, 0, 1, 1, 1, 1, 1, 1};

  double lr = 0.001;
  int logreg_iterations = 10000;
  int iterations = 1000;
  double sum{0};
  std::vector<double> samples;
  for (int idx{0}; idx < iterations; idx++) {
    auto start = std::chrono::steady_clock::now();
    LogisticRegression l{lr, logreg_iterations, 0.0};
    l.fit(features, targets);
    std::vector<double> probs = l.predict_proba(features);
    //   printMatrixData(betas);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;

    // std::cout << "Total Time (ns): " << dur.count() << "\n";
    double duration = dur.count();
    sum += duration;
    samples.push_back(duration);
  }

  double stDev = getStDev(samples);
  std::cout << "N Samples: " << featureData.size() << " elements" << "\n";
  std::cout << "N Features: " << featureData[0].size() << "\n";
  std::cout << "LR: " << lr << "\n";
  std::cout << "LogReg Iterations: " << logreg_iterations << "\n";
  std::cout << "N Features: " << featureData[0].size() << "\n";
  std::cout << "Total Time (ms): " << sum << "\n";
  std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";
  std::cout << "StDev (ms): +/- " << stDev << "\n";
}
