#include <iostream>
#include <sstream>
#include <vector>


void printVec(std::vector<std::string> vec){
    std::cout << "[" << "\n";
    for (auto v : vec) {
        std::cout << v << "\n";
    }
    std::cout << "]" << "\n";

}


std::vector<std::string> tokenizeSequence(std::string sequence){
    std::stringstream ss(sequence);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token){
        tokens.push_back(token);
    }

    return tokens;

}

std::vector<std::string> removeStopWords(std::vector<std::string> tokens){
    std::vector<std::string> tokensSansStops;
}



class TFIDF{
public:
std::vector<std::string> corpus;
std::vector<std::vector<std::string>> tokenizedCorpus;


TFIDF(std::vector<std::string> corpus) : corpus(corpus) {}


void tokenizeCorpus(){
    std::vector<std::vector<std::string>> tokenizedCorpus;
    for (auto sequence : corpus){
        std::vector<std::string> tokenizedSequence = tokenizeSequence(sequence);
        tokenizedCorpus.push_back(tokenizedSequence);
    }

    this->tokenizedCorpus = tokenizedCorpus;
}



};








int main(){
    std::cout << "TF-IDF" << "\n";
    std::cout << "-------------" << "\n";

    std::string sequence = "Hello there people.";
    std::vector<double> a = {1,2,3};
    auto tokens = tokenizeSequence(sequence);
    printVec(tokens);

}