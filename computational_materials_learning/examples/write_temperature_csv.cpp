#include "workspace_io.hpp"
#include <iostream>
#include <vector>

int main() {
  std::vector<double> temperatures = {20.5, 21.0, 22.5, 23.0, 24.5};
  write_csv("temperatures.csv", temperatures);
  return 0;
}