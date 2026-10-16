#include "utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> loadCSV(const char *filepath) {

    std::vector<std::vector<std::string>> content;
    std::string line, word;
  
    std::fstream file(filepath);
    if (!file.is_open()) {
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


  std::vector<std::vector<double>>
loadNumericFeatures(std::vector<std::vector<std::string>> data) {
  std::vector<std::vector<double>> numericFeatures;

  for (int idx{1}; idx < data.size(); idx++) {
    std::vector<double> row;
    for (int jdx{1}; jdx < 5; jdx++) {
      row.push_back(std::stod(data[idx][jdx]));
    }
    numericFeatures.push_back(row);
  }

  return numericFeatures;
}