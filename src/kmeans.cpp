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


template <int Cols>
void printVec(std::array<double,Cols> vec) {
  std::cout << "[ ";
  for (int i{0}; i < Cols; i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << "]" << "\n";
}

template <int Rows, int Cols>
void printVecVec(std::array<std::array<double,Cols>,Rows> vecVec) {
  for (int i{0}; i < Rows; i++) {
    printVec<Cols>(vecVec[i]);
  }
}

double cartesianDistance(std::vector<double> &a, std::vector<double> &b) {
  double sqsum{0};
  for (int i{0}; i < a.size(); i++) {
    sqsum += std::pow((a[i] - b[i]), 2);
  }
  return std::pow(sqsum, 0.5);
}

struct ClusterResult {
  std::vector<std::vector<double>> centroids;
  std::vector<std::vector<std::vector<double>>> clusters;
  int nClusters;
};

std::vector<std::vector<double>>
initkpp(int nClusters, std::vector<std::vector<double>> &points) {
  std::vector<std::vector<double>> centroids;
  centroids.reserve(nClusters);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> int_dist(0, points.size() - 1);
  int rZero = int_dist(gen);

  centroids.push_back(points[rZero]);

  for (int i{1}; i < nClusters; i++) {
    std::vector<double> dists;
    size_t npts = points.size();
    dists.reserve(npts);

    for (int j{0}; j < npts; j++) {
      std::vector<double> pt = points[j];
      double d = DBL_MAX;

      for (int k{0}; k < centroids.size(); k++) {
        double dist = cartesianDistance(pt, centroids[k]);
        d = std::min(dist, d);
      }
      dists.push_back(d);
    }
    double mx = 0.0;
    int mxIdx;

    for (int j{0}; j < npts; j++) {
      if (dists[j] > mx) {
        mx = dists[j];
        mxIdx = j;
      }
    }

    centroids.push_back(points[mxIdx]);
  }
  return centroids;
}

std::vector<double> addVec(std::vector<double> &a, std::vector<double> &b) {
  std::vector<double> c;
  size_t nElements = a.size();
  c.reserve(nElements);
  for (int vIdx{0}; vIdx < nElements; vIdx++) {
    c.push_back(a[vIdx] + b[vIdx]);
  }
  return c;
}

std::vector<double> scalarDivide(std::vector<double> &vec, double scalar) {
  std::vector<double> newVec;
  for (int vIdx{0}; vIdx < vec.size(); vIdx++) {
    newVec.push_back(vec[vIdx] / scalar);
  }
  return newVec;
}

std::vector<std::vector<double>>
updateCentroids(std::vector<std::vector<std::vector<double>>> &clusters, int nClusters) {
  std::vector<std::vector<double>> newCentroids;
  newCentroids.reserve(nClusters);

  for (int cIdx{0}; cIdx < clusters.size(); cIdx++) {
    std::vector<double> newCentroid;
    double count{0.0};

    for (int pIdx{0}; pIdx < clusters[cIdx].size(); pIdx++) {
      if (pIdx == 0) {
        newCentroid = clusters[cIdx][pIdx];
      } else {
        newCentroid = addVec(newCentroid, clusters[cIdx][pIdx]);
      }
      count += 1.0;
    }

    newCentroid = scalarDivide(newCentroid, count);
    newCentroids.push_back(newCentroid);
  }
  return newCentroids;
}

ClusterResult kMeans(std::vector<std::vector<double>> &data, int nClusters) {
  std::vector<std::vector<double>> centroids = initkpp(nClusters, data);
  std::vector<std::vector<std::vector<double>>> clusters;
  
  bool converged = false;
  int count{0};
  while (!converged) {

    clusters.clear();
    clusters.resize(nClusters);

    // for each point
    for (int pIdx{0}; pIdx < data.size(); pIdx++) {
      double minDist = DBL_MAX;
      int minIdx = 0;

      // for each centroid
      for (int cIdx{0}; cIdx < centroids.size(); cIdx++) {

        double dist = cartesianDistance(data[pIdx], centroids[cIdx]);
        if (dist < minDist) {
          minDist = dist;
          minIdx = cIdx;
        }
      }
      std::vector<double> point = data[pIdx];

      clusters[minIdx].push_back(point);
    }
    std::vector<std::vector<double>> newCentroids = updateCentroids(clusters, nClusters);

    if (centroids == newCentroids) {
      converged = true;
    } else {
      centroids = newCentroids;
    }
  }

  return ClusterResult{centroids, clusters, nClusters};
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
  //    std::vector<std::vector<double>>points = {{1.0, 2.0}, {3.0, 4.0},
  //    {5.0, 6.0}, {7.0, 8.0}};

  int nClusters = 3;
  constexpr int nDataPoints = 150;
  constexpr int nDataColumns = 6;


  const char *filepath = "./data/iris.csv";
  std::array<std::array<std::string, nDataColumns>, nDataPoints+1> data = loadCSV<nDataPoints+1, nDataColumns>(filepath);
  printVecVecString<nDataPoints+1, nDataColumns>(data);



  std::array<std::array<double,nDataColumns-2>,nDataPoints> numericFeatures = loadNumericFeatures<nDataPoints+1,nDataColumns,nDataPoints,nDataColumns-2>(data);
  printVecVec<nDataPoints,nDataColumns-2>(numericFeatures);



  // std::cout << "Kmeans" << '\n';
  // std::cout << "--------------" << "\n";

  // int iterations = 1000;
  // double sum{0};
  // std::vector<double> samples;
  // for (int idx{0}; idx < iterations; idx++) {
  //   auto start = std::chrono::steady_clock::now();
  //   ClusterResult res = kMeans(numericFeatures, nClusters);
  //   auto end = std::chrono::steady_clock::now();
  //   std::chrono::duration<double, std::milli> dur = end - start;
  //   std::vector<std::vector<double>> centroids = res.centroids;
  //   std::vector<std::vector<std::vector<double>>> clusters = res.clusters;

  //   // std::cout << "Total Time (ns): " << dur.count() << "\n";
  //   double duration = dur.count();
  //   sum += duration;
  //   samples.push_back(duration);
  // }

  // double stDev = getStDev(samples);
  // std::cout << "Data Size: " << numericFeatures.size() << " elements" << "\n";
  // std::cout << "N Clusters: " << nClusters << "\n";
  // std::cout << "Total Time (ms): " << sum << "\n";
  // std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";
  // std::cout << "StDev (ms): +/- " << stDev << "\n";
}