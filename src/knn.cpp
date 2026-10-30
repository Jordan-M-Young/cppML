#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include "utils.h"

template <int Cols>
double euclideanDistance(std::array<double,Cols>& vecA, std::array<double,Cols>& vecB){
    double dist{0.0};
    double diff;
    for (int idx{0}; idx < Cols; idx++){
        diff = (vecA[idx]-vecB[idx]);
        dist += (diff * diff);
    }
    return std::sqrt(dist);
}

template <typename T, int Cols, int Classes>
std::vector<double> binCount(std::array<T,Cols>& vec, double scalar){
    auto maxIterator = std::max_element(vec.begin(),vec.end());
    int maxElement = *maxIterator;

    std::array<double,Classes-1> binCounter (Classes,0.0);
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


// Main Template Class
template <int Rows, int Cols, int k> 
class kNearestNeighbors{
public:
    std::array<std::array<double,Cols>,Rows> features;
    std::array<int,Rows> targets;

    kNearestNeighbors(const std::array<std::array<double,Cols>,Rows>& features, const std::array<int,Rows>& targets) 
        : features(features), targets(targets) {}

    std::array<Pair, k> getKNearest(std::array<double,Cols>& sample){
        std::array<Pair,Rows> pairs;
        for (int jdx{0}; jdx < Rows; jdx++) {
            double dist = euclideanDistance<Cols>(sample, features[jdx]);
            pairs[jdx] = Pair{dist, jdx};
        }

        std::partial_sort(pairs.begin(), pairs.begin() + k, pairs.end(), 
        [](const Pair& a, const Pair& b) {
            return a.dist < b.dist;
        }
    );
        
        std::array<Pair, k> result;
        std::copy(pairs.begin(), pairs.begin() + k, result.begin());
        return result;
    }  

    template<int nClasses> 
    std::vector<double> predictProba(const std::array<double,Cols>& sample){
        std::array<Pair, k> pairs = getKNearest(sample);
        std::array<int, k> selectedTargs;
        for (int pIdx{0}; pIdx < k; pIdx++){
            selectedTargs[pIdx] = targets[pairs[pIdx].index];
        }
        return binCount<int, k, nClasses>(selectedTargs, k);
    }
};



template <int Rows, int Cols>
struct RandomVecData{
    std::array<std::array<double,Cols>,Rows> features;
    std::array<int,Rows> targets;
    std::array<double, Cols> randomSample;
};

template <int Rows, int Cols>
RandomVecData<Rows, Cols> genRandomVecData(int nClasses){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int>  intDist(0, nClasses-1);
    std::uniform_real_distribution<double> dDist(0.0, 1.0);

    std::array<std::array<double,Cols>,Rows> features;
    std::array<int,Rows> targets;

    for (int sIdx{0}; sIdx < Rows; sIdx++){
        for (int fIdx{0}; fIdx < Cols; fIdx++){
            features[sIdx][fIdx] = dDist(gen);
        }
        targets[sIdx] = intDist(gen);
    }

    std::array<double,Cols> randomSample;
    for (int fIdx{0}; fIdx < Cols; fIdx++){
        randomSample[fIdx] = dDist(gen);
    } 


    return RandomVecData<Rows, Cols>{features, targets, randomSample};




}

int main(){
    const int k = 10;
    const int nFeatures = 10;
    const int nClasses = 2;
    const int nSamples = 10000;


    RandomVecData<nSamples, nFeatures> vecData = genRandomVecData<nSamples, nFeatures>(nClasses);

    auto knn = kNearestNeighbors<nSamples, nFeatures, k>(vecData.features, vecData.targets);

    int iterations = 1000;
    double sum{0};
    std::vector<double> samples;
    std::cout << "KNearestNeighbors:" << "\n";
    std::cout << "-------------------------" << "\n";
    for (int i{0}; i < iterations; i++){

        auto start =  std::chrono::steady_clock::now();
        std::array<Pair, k> pairs = knn.getKNearest(vecData.randomSample);
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