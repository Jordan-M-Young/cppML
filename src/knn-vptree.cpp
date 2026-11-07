#include <iostream>
#include <vector>

class Node{
public:
int index;
double threshold;
Node* left;
Node* right;

Node(int index, double threshold) : left(nullptr), right(nullptr), index(index), threshold(threshold)  {}

bool isLeafNode(){
    return (left == nullptr && right == nullptr);
}

};


class VPTree{
    public:
    Node* root;

    VPTree() : root(nullptr) {}

    Node* growTree(std::vector<std::vector<double>>){


    }


};




int main(){
    std::vector<double> point = {0,1};
    Node node = Node(0,0.0);
    std::cout << node.isLeafNode() << "\n";
}