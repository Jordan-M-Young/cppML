#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>

double euclideanDistance(std::vector<double> vecA, std::vector<double> vecB){
    double dist{0.0};
    for (int idx{0}; idx < vecA.size(); idx++){
        dist += pow((vecA[idx]-vecB[idx]),2);
    }
    return pow(dist, 0.5);
}

template <typename T>
std::vector<double> binCount(std::vector<T>& vec, double scalar){
    auto maxIterator = std::max_element(vec.begin(),vec.end());
    int maxElement = *maxIterator;

    std::vector<double> binCounter (maxElement+1,0.0);
    double increment{1.0};
    if (scalar > 0){
        increment = increment / scalar;
    }


    for (int idx{0}; idx < vec.size(); idx++){
        binCounter[vec[idx]] += increment;
    }
    return binCounter;
}


struct Pair{
    double dist;
    int index;
};

class kNearestNeighbors{
public:
std::vector<std::vector<double>> features;
std::vector<int> targets;

kNearestNeighbors(std::vector<std::vector<double>> features, std::vector<int> targets) : features(features), targets(targets) {}

std::vector<Pair> getKNearest(std::vector<double> sample, int k){

    std::vector<Pair> pairs;

    for (int jdx{0}; jdx < features.size(); jdx++) {
        double dist = euclideanDistance(sample,features[jdx]);
        pairs.push_back(Pair{dist, jdx});
    }

    std::sort(pairs.begin(), pairs.end(), [](const Pair& a, const Pair& b) {
        return a.dist < b.dist; 
    });

    return std::vector<Pair>(pairs.begin(), pairs.begin() + k);
};


std::vector<double> predictProba(std::vector<double> sample, int k){
    std::vector<Pair> pairs = getKNearest(sample, k);
    std::vector<int> selectedTargs;
    for (auto pair : pairs){
        selectedTargs.push_back(targets[pair.index]);
    }
    return binCount(selectedTargs,k);


}


};


struct RandomVecData{
    std::vector<std::vector<double>> features;
    std::vector<int> targets;
    std::vector<double> randomSample;
};


RandomVecData genRandomVecData(int nFeatures, int nSamples, int nClasses){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int>  intDist(0, nClasses-1);
    std::uniform_real_distribution<double> dDist(0.0, 1.0);

    std::vector<std::vector<double>> features;
    features.reserve(nSamples); 
    std::vector<int> targets;
    targets.reserve(nSamples);

    for (int sIdx{0}; sIdx < nSamples; sIdx++){
        std::vector<double> featureRow;
        featureRow.reserve(nFeatures);
        for (int fIdx{0}; fIdx < nFeatures; fIdx++){
            featureRow.push_back(dDist(gen));
        }
        features.push_back(featureRow);
        targets.push_back(intDist(gen));
    }

    std::vector<double> randomSample;
    randomSample.reserve(nFeatures);
    for (int fIdx{0}; fIdx < nFeatures; fIdx++){
        randomSample.push_back(dDist(gen));
    } 


    return RandomVecData{features, targets, randomSample};




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


int main(){
    int k = 10;
    int nFeatures = 10;
    int nClasses = 2;
    int nSamples = 10000;
    RandomVecData vecData = genRandomVecData(nFeatures, nSamples, nClasses);

    std::vector<double> sample = {0.0,0.0};
    kNearestNeighbors knn = kNearestNeighbors(vecData.features, vecData.targets);

    int iterations = 1000;
    double sum{0};
    std::vector<double> samples;
    std::cout << "KNearestNeighbors:" << "\n";
    std::cout << "-------------------------" << "\n";
    for (int i{0}; i < iterations; i++){

        auto start =  std::chrono::steady_clock::now();
        std::vector<Pair> pairs = knn.getKNearest(vecData.randomSample, k);
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> dur = end - start;

        double duration = dur.count();
        sum += duration;
        samples.push_back(duration);
    }

    double stDev = getStDev(samples);
    std::cout << "Data Size: " << nSamples << " elements" << "\n";
    std::cout << "Total Time (ms): " << sum << "\n";
    std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";
    std::cout << "StDev (ms): +/- " << stDev << "\n";

}