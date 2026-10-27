#include <vector>
std::vector<std::vector<std::string>> loadCSV(const char *filepath);
std::vector<std::vector<double>>loadNumericFeatures(std::vector<std::vector<std::string>> data);
template <typename T>
void printVec(std::vector<T> vec);
template <typename T>
void printVecVec(std::vector<std::vector<double>> vecVec);