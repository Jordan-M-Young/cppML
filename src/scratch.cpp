#include <iostream>
#include <ctime>

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
    double a{4.0};
    int b{2};

    int iterations = 10000000;
    double sum{0};
    std::vector<double> samples;
    for (int i{0}; i < iterations; i++){
        auto start = std::chrono::steady_clock::now();
        double c = a / 2;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> dur = end - start;

        // std::cout << "Total Time (ns): " << dur.count() << "\n";
        double duration = dur.count();
        sum += duration;
        samples.push_back(duration);
    }


    double stDev = getStDev(samples);
    std::cout << "Total Time (ns): " << sum << "\n";
    std::cout << "Average Iteration Time (ns): " << sum / iterations << "\n";
    std::cout << "StDev (ns): +/- " << stDev << "\n";
}