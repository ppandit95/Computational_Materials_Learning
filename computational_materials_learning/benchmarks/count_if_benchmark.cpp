#include "workspace_algorithms.hpp"
#include <chrono>
#include<iostream>
#include<vector>
const std::vector<std::size_t> sizes{10'000,100'000,1'000'000,10'000'000};
int main(){
  for(const auto size:sizes){
    std::vector<double> values(size,1200.0);
    const auto start = std::chrono::high_resolution_clock::now();
    const auto count_positive = count_if(values,[](double x){return x > 1000.0;});
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout<<"Size: "<<size<<", Count: "<<count_positive<<", Duration: "<<duration<<" ms"<<std::endl;
  }
  return 0;
}