#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>


void printVec(std::vector<double> vec) {
  std::cout << "[ ";
  for (int i{0}; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << "]" << "\n";
}

void printVecVec(std::vector<std::vector<double>> vecVec) {
  for (int i{0}; i < vecVec.size(); i++) {
    printVec(vecVec[i]);
  }
}

double cartesianDistance(std::vector<double> a, std::vector<double> b) {
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
initkpp(int nClusters, std::vector<std::vector<double>> points) {
  std::vector<std::vector<double>> centroids;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> int_dist(0, points.size() - 1);
  int rZero = int_dist(gen);

  centroids.push_back(points[rZero]);

  for (int i{1}; i < nClusters; i++) {
    std::vector<double> dists;

    for (int j{0}; j < points.size(); j++) {
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

    for (int j{0}; j < dists.size(); j++) {
      if (dists[j] > mx) {
        mx = dists[j];
        mxIdx = j;
      }
    }

    centroids.push_back(points[mxIdx]);
  }
  return centroids;
}

std::vector<double> addVec(std::vector<double> a, std::vector<double> b) {
  std::vector<double> c;
  for (int vIdx{0}; vIdx < a.size(); vIdx++) {
    c.push_back(a[vIdx] + b[vIdx]);
  }
  return c;
}

std::vector<double> scalarDivide(std::vector<double> vec, double scalar) {
  std::vector<double> newVec;
  for (int vIdx{0}; vIdx < vec.size(); vIdx++) {
    newVec.push_back(vec[vIdx] / scalar);
  }
  return newVec;
}

std::vector<std::vector<double>>
updateCentroids(std::vector<std::vector<std::vector<double>>> clusters) {
  std::vector<std::vector<double>> newCentroids;
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

ClusterResult kMeans(std::vector<std::vector<double>> data, int nClusters) {
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
    std::vector<std::vector<double>> newCentroids = updateCentroids(clusters);

    if (centroids == newCentroids) {
      converged = true;
    } else {
      centroids = newCentroids;
    }
  }

  return ClusterResult{centroids, clusters, nClusters};
}



std::vector<std::vector<std::string>> loadCSV(const char* filepath){

    std::vector<std::vector<std::string>> content;
    std::string line, word;

    std::fstream file(filepath);
    if (!file.is_open()){
        std::cout << "FILE : " << filepath << " NOT OPENED." << "\n";
        return content;
    }


    // Read file line by line
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);

        // Split each line by comma
        while (std::getline(ss, word, ',')) {
            row.push_back(word);
        }
        content.push_back(row);
    }

    file.close();

    return content;

}

void printVecString(std::vector<std::string>& vec){
    std::cout << "[";
    for (int idx{0}; idx < vec.size(); idx++){
        std::cout << " " << vec[idx];
    }
    std::cout << " ]" << "\n";
}

void printVecVecString(std::vector<std::vector<std::string>>& vecVec){
    for (int idx{0}; idx < vecVec.size(); idx++){
        printVecString(vecVec[idx]);
    }
}

std::vector<std::vector<double>> loadNumericFeatures(std::vector<std::vector<std::string>> data){
    std::vector<std::vector<double>> numericFeatures;

    for (int idx{1}; idx < data.size(); idx++){
        std::vector<double> row;
        for (int jdx{1}; jdx < 5; jdx++){
            row.push_back(std::stod(data[idx][jdx]));
        }
        numericFeatures.push_back(row);

    }

    return numericFeatures;

}

int main() {
//    std::vector<std::vector<double>>points = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {7.0, 8.0}};

  int nClusters = 3;
  const char* filepath = "./data/iris.csv";
  std::vector<std::vector<std::string>> data = loadCSV(filepath);
  std::vector<std::vector<double>> numericFeatures = loadNumericFeatures(data);
  std::cout << "Kmeans" << '\n';
  std::cout << "--------------" << "\n";


  int iterations = 1000;
  double sum{0};

  for (int idx{0}; idx < iterations; idx++){
    auto start = std::chrono::steady_clock::now();
    ClusterResult res = kMeans(numericFeatures, nClusters);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;
    std::vector<std::vector<double>> centroids = res.centroids;
    std::vector<std::vector<std::vector<double>>> clusters = res.clusters;
  
    // std::cout << "Total Time (ns): " << dur.count() << "\n";
    sum += dur.count();


  }
    std::cout << "Data Size: " << numericFeatures.size() << " elements" << "\n";
    std::cout << "N Clusters: " << nClusters << "\n";
    std::cout << "Total Time (ms): " << sum << "\n";
    std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";



}