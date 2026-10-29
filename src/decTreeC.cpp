#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <cfloat>
#include <unordered_map>
#include "utils.h"


std::vector<double> scalarDivide(std::vector<double>& vec, double scalar){
    std::vector<double> newVec;
    newVec.reserve(vec.size());

    for (int idx{0}; idx < vec.size(); idx++){
        newVec.push_back(vec[idx]/scalar);
    }
    return newVec;
}

struct Matrix {
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;
  };



template <typename T>
std::vector<T> getUnique(std::vector<T>& vec){
    std::unordered_set<T> valSet;
    std::vector<T> uniqueVals;
    uniqueVals.reserve(vec.size());

    for (int idx{0}; idx < vec.size(); idx++){
        if (valSet.contains(vec[idx])){
            continue;
        }
        uniqueVals.push_back(vec[idx]);
        valSet.insert(vec[idx]);
    }
    return uniqueVals;
}


class Node
{
public:
    Node* left;
    Node* right;
    int value;
    int feature;
    double threshold;

    Node(int val, int f, double t) : left(nullptr), right(nullptr), value(val), feature(f), threshold(t)  {}

    bool isLeafNode(){
        return (left == nullptr && right == nullptr);
    }
};


template <typename T>
std::vector<double> binCount(std::vector<T>& vec){
    auto maxIterator = std::max_element(vec.begin(),vec.end());
    int maxElement = *maxIterator;

    std::vector<double> binCounter (maxElement+1,0.0);

    for (int idx{0}; idx < vec.size(); idx++){
        binCounter[vec[idx]] += 1.0;
    }
    return binCounter;
}


std::vector<double> applyPLog2(std::vector<double>& vec){
    std::vector<double> newVec;
    newVec.reserve(vec.size());
    for (int idx{0};idx < vec.size(); idx++){
        if (vec[idx] > 0.0){
            newVec.push_back(vec[idx] * std::log2(vec[idx]));
        }
    }
    return newVec;
}

double vectorSum(std::vector<double>& vec) {
    double sum{0.0};
    for (int idx{0}; idx < vec.size(); idx++) {
      sum += vec[idx];
    }
    return sum;
  }


  std::vector<double> getMatrixCol(Matrix& mat, int colIdx){
    std::vector<double> colData;
    colData.reserve(mat.rows);
    for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++){
        colData.push_back(mat.data[rowIdx][colIdx]);
    }
    return colData;
  }

Matrix sampleMatrixRows(Matrix& mat, std::vector<int>& rowIndices){
    std::vector<std::vector<double>> newData;
    newData.reserve(rowIndices.size());
    for (int idx : rowIndices){
        newData.push_back(mat.data[idx]);
    }
    return Matrix {newData, rowIndices.size(), mat.cols};
}

struct IndexSplitResult{
    std::vector<int> left;
    std::vector<int> right;
};


IndexSplitResult splitOnThreshold(std::vector<double>& vec, double threshold){
    std::vector<int> left;
    std::vector<int> right;
    left.reserve(vec.size());
    right.reserve(vec.size());

    for (int idx{0}; idx < vec.size(); idx++){
        if (vec[idx] <= threshold){
            left.push_back(idx);
        } else {
            right.push_back(idx);
        }
    }
    return IndexSplitResult{left, right};

}


std::vector<int> selectValuesByIndices(std::vector<int>& vec, std::vector<int>& indices){
    std::vector<int> subVec;
    subVec.reserve(indices.size());
    for (int idx : indices){
        subVec.push_back(vec[idx]);
    }
    return subVec;
}

template <typename T>
int argMax(std::vector<T>& vec){
    auto maxIt = std::max_element(vec.begin(),vec.end());
    return std::distance(vec.begin(),maxIt);

}


std::vector<int> encodeTargets(std::vector<std::string>& targets){
    std::unordered_map<std::string, int> mp;
    std::vector<int> encodedTargets;
    int count{0};

    encodedTargets.reserve(targets.size());
    for (std::string target : targets){
        if (!mp.contains(target)){
            mp[target] = count;
            count += 1;
        }
        encodedTargets.push_back(mp[target]);
    }
    return encodedTargets;
}


std::vector<std::string> getTargetColumn(std::vector<std::vector<std::string>>& data){
    std::vector<std::string> targets;
    targets.reserve(data.size());
    for (std::vector<std::string> row : data){
        if (row[5] == "Species"){
            continue;
        }
        targets.push_back(row[5]);
    }
    return targets;
}

struct SplitResult{
    int splitIdx;
    double splitThresh;
};

class DecisionTree
{
public:
    int minSamplesSplit;
    int maxDepth;
    Node* root;

    DecisionTree(int minSamplesSplit, int maxDepth) : minSamplesSplit(minSamplesSplit), maxDepth(maxDepth), root(nullptr)  {}


    SplitResult findBestSplit(Matrix& features, std::vector<int>& targets){
        double bestGain = -1.0;
        int splitIdx = -1;
        double splitThreshold = 0.0;
        double n = targets.size();

        double parentEntropy = entropy(targets);

        for (int featIdx{0}; featIdx < features.cols; featIdx++){
            std::vector<double> featColumn = getMatrixCol(features, featIdx);
            std::vector<double> thresholds = getUnique(featColumn);
            
            for (int tIdx{0}; tIdx < thresholds.size(); tIdx++){
                IndexSplitResult indexSplit = splitOnThreshold(featColumn, thresholds[tIdx]);
                std::vector<int> leftIdxs = indexSplit.left;
                std::vector<int> rightIdxs = indexSplit.right;


                if ((leftIdxs.size() == 0) || (rightIdxs.size() == 0)){
                    continue;
                }

                size_t nLeft = leftIdxs.size(); 
                size_t nRight = rightIdxs.size();

                std::vector<int> leftTargets = selectValuesByIndices(targets, leftIdxs);
                double nL = leftTargets.size();
                std::vector<int> rightTargets = selectValuesByIndices(targets, rightIdxs);
                double nR = rightTargets.size();


                double eLeft = entropy(leftTargets);
                double eRight = entropy(rightTargets);
                
                double childEntropy = ((nL / n) * eLeft) + ((nR / n) * eRight);

                double infoGain = parentEntropy - childEntropy;

                if (infoGain > bestGain){
                    bestGain = infoGain;
                    splitIdx = featIdx;
                    splitThreshold = thresholds[tIdx];
                }

            }


        }
        return SplitResult{splitIdx, splitThreshold};

    }


    Node* growTree(Matrix& features, std::vector<int>& targets, int depth){
        int nSamples = features.rows;
        int nFeatures = features.cols;
        int nLabels = getUnique(targets).size();

        // if (targets.empty() || nLabels == 0) {
        //     return new Node(-1, -1, -DBL_MAX);
        // }

        if ((depth >= maxDepth) || (nLabels == 1) || (nSamples < minSamplesSplit)){
            auto binCounter = binCount(targets);
            int mostCommon = argMax(binCounter);
            return new Node(mostCommon,-1,-DBL_MAX);
        }

        SplitResult bestSplit = findBestSplit(features, targets);



        int featIdx = bestSplit.splitIdx;
        double threshold = bestSplit.splitThresh;


        if (featIdx == -1) {
            auto binCounter = binCount(targets);
            int mostCommon = argMax(binCounter);
            return new Node(mostCommon, -1, -DBL_MAX);
        }
        
        std::vector<double> featureColum = getMatrixCol(features, featIdx);

        IndexSplitResult indexSplit = splitOnThreshold(featureColum, threshold);

        std::vector<int> leftIdxs = indexSplit.left;
        std::vector<int> rightIdxs = indexSplit.right;
        

        Matrix leftFeatures = sampleMatrixRows(features, leftIdxs);
        std::vector<int> leftTargets = selectValuesByIndices(targets, leftIdxs);

        Matrix rightFeatures = sampleMatrixRows(features, rightIdxs);
        std::vector<int> rightTargets = selectValuesByIndices(targets, rightIdxs);

        Node* left = growTree(leftFeatures, leftTargets, depth+1);
        Node* right = growTree(rightFeatures, rightTargets, depth+1);
        
        Node* node = new Node(-1,featIdx, threshold);
        node->left = left;
        node->right = right;
        return node;
    }
    double entropy(std::vector<int>& vec){
        if (vec.empty()){
            return 0.0;
        }
        std::vector<double> binCounter = binCount(vec);
        std::vector<double> scaledBinCounter = scalarDivide(binCounter, vec.size());
        std::vector<double> logvals = applyPLog2(scaledBinCounter);
        double entropy = -1.0 * vectorSum(logvals);
        return entropy;
    }

    void fit(Matrix& features, std::vector<int>& targets){
        clearTree(root);
        root = growTree(features, targets, 0);
    }

    int traverseTree(std::vector<double>& x, Node* node){
        if (node->isLeafNode()){
            return node->value;
        }
        int feat = node->feature;
        double threshold = node->threshold;
        if (x[feat] <= threshold){
            return traverseTree(x, node->left);
        } 
        return traverseTree(x, node->right);
    }

    std::vector<int> predict(Matrix& features){
        std::vector<int> preds;
        preds.reserve(features.rows);
        for (auto& row : features.data){
            int pred = traverseTree(row, root);
            preds.push_back(pred);
        }
        return preds;
    }





private:
    void clearTree(Node* node){
        if (node == nullptr){
            return;
        }
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
};

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

    // There are many algorithms for decision tree induction: Hunts Algorithm, CART, ID3, C4.5, SLIQ, SPRINT
    
    const char* filepath = "./data/iris.csv";
    std::vector<std::vector<std::string>> csvData = loadCSV(filepath);
    std::vector<std::vector<double>> featuresData = loadNumericFeatures(csvData);
    std::vector<std::string> strTargets = getTargetColumn(csvData);
    std::vector<int> targets = encodeTargets(strTargets);
    DecisionTree decTree = DecisionTree(2,3);
    Matrix features = Matrix{featuresData, featuresData.size(), featuresData[0].size()};




    std::cout << "Decision Tree" << "\n";
    std::cout << "-----------------" << "\n";


  
    int iterations = 1000;
    double sum{0};
    std::vector<double> samples;
  
    for (int idx{0}; idx < iterations; idx++) {
      auto start = std::chrono::steady_clock::now();
      decTree.fit(features, targets);
      std::vector<int> preds = decTree.predict(features);      
      auto end = std::chrono::steady_clock::now();
      std::chrono::duration<double, std::milli> dur = end - start;
  
      // std::cout << "Total Time (ns): " << dur.count() << "\n";
      double duration = dur.count();
      sum += duration;
      samples.push_back(duration);
    }
  
    double stDev = getStDev(samples);
    std::cout << "Data Size: " << features.rows << " elements" << "\n";
    std::cout << "Total Time (ms): " << sum << "\n";
    std::cout << "Average Iteration Time (ms): " << sum / iterations << "\n";
    std::cout << "StDev (ms): +/- " << stDev << "\n";
}