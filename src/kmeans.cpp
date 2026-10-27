#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "utils.h"


template <int Rows, int Cols>
void printVecVec(std::array<std::array<double,Cols>,Rows> vecVec) {
  for (int i{0}; i < Rows; i++) {
    printVec<Cols>(vecVec[i]);
  }
}

template<int Cols>
double cartesianDistance(std::array<double,Cols> &a, std::array<double,Cols> &b) {
  double sqsum{0};
  for (int i{0}; i < Cols; i++) {
    sqsum += std::pow((a[i] - b[i]), 2);
  }
  return std::pow(sqsum, 0.5);
}

template <int Rows, int Cols, int Clusters>
struct ClusterResult {
  std::array<std::array<double,Cols>,Clusters> centroids;
  std::array<std::array<std::array<double,Cols>,Rows>,Clusters> clusters;
  int nClusters;
};


template<int Rows, int Cols, int Clusters>
std::array<std::array<double,Cols>,Clusters>
initkpp(std::array<std::array<double,Cols>,Rows> &points) {
  std::array<std::array<double,Cols>,Clusters> centroids;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> int_dist(0, points.size() - 1);
  int rZero = int_dist(gen);

  centroids[0] = points[rZero];

  for (int i{1}; i < Clusters; i++) {
    std::array<double,Rows> dists;

    for (int j{0}; j < Rows; j++) {
      std::array<double,Cols> pt = points[j];
      double d = DBL_MAX;

      for (int k{0}; k < Clusters; k++) {
        double dist = cartesianDistance<Cols>(pt, centroids[k]);
        d = std::min(dist, d);
      }
      dists[j] = d;
    }
    double mx = 0.0;
    int mxIdx;

    for (int j{0}; j < Rows; j++) {
      if (dists[j] > mx) {
        mx = dists[j];
        mxIdx = j;
      }
    }

    centroids[i] = points[mxIdx];
  }
  return centroids;
}

template<int Cols>
std::array<double,Cols> addVec(std::array<double,Cols> &a, std::array<double,Cols> &b) {
  std::array<double,Cols> c;
  for (int vIdx{0}; vIdx < Cols; vIdx++) {
    c[vIdx] = a[vIdx] + b[vIdx];
  }
  return c;
}

template<int Cols>
std::array<double,Cols> scalarDivide(std::array<double,Cols> &vec, double scalar) {
  std::array<double,Cols> newVec;
  for (int vIdx{0}; vIdx < Cols; vIdx++) {
    newVec[vIdx] = vec[vIdx] / scalar;
  }
  return newVec;
}

template <int Rows, int Cols, int Clusters>
std::array<std::array<double,Cols>,Clusters>
updateCentroids(std::array<std::array<std::array<double,Cols>,Rows>,Clusters> &clusters) {
  std::array<std::array<double,Cols>,Clusters> newCentroids;

  for (int cIdx{0}; cIdx < Clusters; cIdx++) {
    std::array<double,Cols> newCentroid{0.0};
    double count{0.0};

    for (int pIdx{0}; pIdx < Rows; pIdx++) {

      if (std::isnan(clusters[cIdx][pIdx][0])){
        break;
      }

      if (pIdx == 0) {
        newCentroid = clusters[cIdx][pIdx];
      } else {
        newCentroid = addVec<Cols>(newCentroid, clusters[cIdx][pIdx]);
      }
      count += 1;
    }
    if (count > 0.0){
      newCentroid = scalarDivide<Cols>(newCentroid, count);
    }
    newCentroids[cIdx] = newCentroid;
  }
  return newCentroids;
}

template <int Rows, int Cols, int Clusters>
ClusterResult<Rows, Cols, Clusters> kMeans(std::array<std::array<double,Cols>,Rows> &data) {

  std::array<std::array<double,Cols>,Clusters> centroids = initkpp<Rows, Cols, Clusters>(data);
  std::array<std::array<std::array<double, Cols>, Rows>, Clusters> clusters;

  // Initialize all slots to NAN
  std::array<int,Clusters> clusterCount{0};
  bool converged = false;
  int count{0};
  while (!converged) {


    std::fill(&clusters[0][0][0], &clusters[0][0][0] + (Clusters * Rows * Cols), NAN);
    clusterCount.fill(0);

    // for each point
    for (int pIdx{0}; pIdx < Rows; pIdx++) {
      double minDist = DBL_MAX;
      int minIdx = 0;

      // for each centroid
      for (int cIdx{0}; cIdx < Clusters; cIdx++) {
        double dist = cartesianDistance<Cols>(data[pIdx], centroids[cIdx]);
        if (dist < minDist) {
          minDist = dist;
          minIdx = cIdx;
        }
      }
      std::array<double,Cols> point = data[pIdx];

      // need to do clusters[minIdx][clusterCount] = point
      clusters[minIdx][clusterCount[minIdx]] = point;
      clusterCount[minIdx] += 1;
    } 


    std::array<std::array<double,Cols>,Clusters> newCentroids = updateCentroids<Rows, Cols, Clusters>(clusters);

    if (centroids == newCentroids) {
      converged = true;
    } else {
      centroids = newCentroids;
    }

    count += 1;
  }

  return ClusterResult<Rows, Cols, Clusters>{centroids, clusters, Clusters};
}

template <int Rows, int Cols>
std::array<std::array<std::string,Cols>,Rows> loadCSV(const char *filepath) {

  std::array<std::array<std::string, Cols>, Rows> content;
  std::string line, word;

  std::fstream file(filepath);
  if (!file.is_open()) {
    std::cout << "FILE : " << filepath << " NOT OPENED." << "\n";
    return content;
  }


  int rowCount{0};
  // Read file line by line
  while (std::getline(file, line)) {
    std::array<std::string, Cols> row;
    std::stringstream ss(line);

    int colCount{0};
    // Split each line by comma
    while (std::getline(ss, word, ',')) {
      row[colCount] = word;
      colCount += 1;
    }
    content[rowCount] = row;
    rowCount += 1;
  }

  file.close();

  return content;
}


template <int Cols>
void printVecString(std::array<std::string,Cols> &vec) {
  std::cout << "[";
  for (int idx{0}; idx < Cols; idx++) {
    std::cout << " " << vec[idx];
  }
  std::cout << " ]" << "\n";
}

template <int Rows, int Cols>
void printVecVecString(std::array<std::array<std::string,Cols>,Rows> &vecVec) {
  for (int idx{0}; idx < Rows; idx++) {
    printVecString<Cols>(vecVec[idx]);
  }
}

template <int Rows, int Cols, int Rows2Keep, int Cols2Keep>
std::array<std::array<double,Cols2Keep>,Rows2Keep>loadNumericFeatures(std::array<std::array<std::string,Cols>, Rows> data) {
  std::array<std::array<double,Cols2Keep>,Rows2Keep> numericFeatures;

  for (int idx{1}; idx < Rows2Keep+1; idx++) {
    std::array<double,Cols2Keep> row;

    for (int jdx{1}; jdx < Cols2Keep+1; jdx++) {
      row[jdx-1]= std::stod(data[idx][jdx]);
    }
    numericFeatures[idx-1] = row;
  }

  return numericFeatures;
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

  constexpr int nClusters = 3;
  constexpr int nDataPoints = 150;
  constexpr int nDataColumns = 6;


  const char *filepath = "./data/iris.csv";
  std::array<std::array<std::string, nDataColumns>, nDataPoints+1> data = loadCSV<nDataPoints+1, nDataColumns>(filepath);


  std::array<std::array<double,nDataColumns-2>,nDataPoints> numericFeatures = loadNumericFeatures<nDataPoints+1,nDataColumns,nDataPoints,nDataColumns-2>(data);



  std::cout << "Kmeans" << '\n';
  std::cout << "--------------" << "\n";

  int iterations = 1000;
  double sum{0};
  std::vector<double> samples;

  for (int idx{0}; idx < iterations; idx++) {
    auto start = std::chrono::steady_clock::now();
    ClusterResult<nDataPoints,nDataColumns-2,nClusters> result = kMeans<nDataPoints,nDataColumns-2,nClusters>(numericFeatures);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;
    std::array<std::array<double,nDataColumns-2>,nClusters> centroids = result.centroids;
    std::array<std::array<std::array<double,nDataColumns-2>,nDataPoints>,nClusters> clusters = result.clusters;

    // std::cout << "Total Time (ns): " << dur.count() << "\n";
    double duration = dur.count();
    sum += duration;
    samples.push_back(duration);
  }

  double stDev = getStDev(samples);
  std::cout << "Data Size: " << nDataPoints << " elements" << "\n";
  std::cout << "N Clusters: " << nClusters << "\n";
  std::cout << "Total Time (ms): " << sum << "\n";
  std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";
  std::cout << "StDev (ms): +/- " << stDev << "\n";
}