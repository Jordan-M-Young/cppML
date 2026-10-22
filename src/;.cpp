#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_set>
std::vector<double> scalarDivide(std::vector<double> vec, double scalar){
    std::vector<double> newVec;
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
std::vector<T> getUnique(std::vector<T> vec){
    std::unordered_set<T> valSet;
    std::vector<T> uniqueVals;

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
        if (value == -1){
            return true;
        }
        return false;
    }
};


std::vector<double> binCount(std::vector<int> vec){
    auto maxIterator = std::max_element(vec.begin(),vec.end());
    int maxElement = *maxIterator;

    std::vector<double> binCounter (maxElement,0.0);

    for (int idx{0}; idx < vec.size(); idx++){
        binCounter[vec[idx]] += 1.0;
    }
    return binCounter;
}


std::vector<double> applyPLog2(std::vector<double> vec){
    std::vector<double> newVec;
    for (int idx{0};idx < vec.size(); idx++){
        if (vec[idx] > 0.0){
            newVec.push_back(vec[idx] * std::log2(vec[idx]));
        }
    }
    return newVec;
}

double vectorSum(std::vector<double> vec) {
    double sum{0.0};
    for (int idx{0}; idx < vec.size(); idx++) {
      sum += vec[idx];
    }
    return sum;
  }


  std::vector<double> getMatrixCol(Matrix mat, int colIdx){
    std::vector<double> colData;
    for (int rowIdx{0}; rowIdx < mat.rows; rowIdx++){
        colData.push_back(mat.data[rowIdx][colIdx]);
    }
    return colData;
  }

struct IndexSplitResult{
    std::vector<int> left;
    std::vector<int> right;
};


IndexSplitResult splitOnThreshold(std::vector<double> vec, double threshold){
    std::vector<int> left;
    std::vector<int> right;

    for (int idx{0}; idx < vec.size(); idx++){
        if (vec[idx] <= threshold){
            left.push_back(idx);
        } else {
            right.push_back(idx);
        }
    }
    return IndexSplitResult{left, right};

}


std::vector<int> selectValuesByIndices(std::vector<int> vec, std::vector<int> indices){
    std::vector<int> subVec;
    for (int idx : indices){
        subVec.push_back(vec[idx]);
    }
    return subVec;
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


    SplitResult findBestSplit(Matrix features, std::vector<int> targets){
        double bestGain = -1.0;
        int splitIdx = -1;
        double splitThreshold = 0.0;
        int n = targets.size();


        for (int featIdx{0}; featIdx < features.cols; featIdx++){
            std::vector<double> featColumn = getMatrixCol(features, featIdx);
            std::vector<double> thresholds = getUnique(featColumn);
            
            for (int tIdx{0}; tIdx < thresholds.size(); tIdx++){
                double parentEntropy = entropy(targets);
                IndexSplitResult indexSplit = splitOnThreshold(featColumn, thresholds[tIdx]);
                std::vector<int> leftIdxs = indexSplit.left;
                std::vector<int> rightIdxs = indexSplit.right;


                if ((leftIdxs.size() == 0) || (rightIdxs.size() == 0)){
                    continue;
                }

                size_t nLeft = leftIdxs.size(); 
                size_t nRight = rightIdxs.size();

                std::vector<int> leftTargets = selectValuesByIndices(targets, leftIdxs);
                int nL = leftTargets.size();
                std::vector<int> rightTargets = selectValuesByIndices(targets, rightIdxs);
                int nR = rightTargets.size();


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


    Node growTree(Matrix features, std::vector<int> targets, int depth){

    }


    double entropy(std::vector<int> vec){
        std::vector<double> binCounter = binCount(vec);
        std::vector<double> scaledBinCounter = scalarDivide(binCounter, vec.size());
        double entropy = -1.0 * vectorSum(applyPLog2(scaledBinCounter));
        return entropy;
    }
};


int main(){
    std::cout << "Decision Tree" << "\n";
    std::cout << "-----------------" << "\n";
    int val = -1;
    int f = 0;
    double t = 0.5;


}