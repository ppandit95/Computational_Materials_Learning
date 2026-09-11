#include "workspace_io.hpp"
#include <cstddef>
#include <iostream>
#include <vector>
int main() {
  const double dx_micrometers = 0.25;
  const std::size_t number_of_points = 5;
  std::vector<double> coordinates(number_of_points);
  const std::vector<double> concentration{0.10, 0.18, 0.32, 0.41, 0.50};
  for (std::size_t i = 0; i < number_of_points; ++i) {
    coordinates[i] = i * dx_micrometers;
  }
  write_field_csv(coordinates, concentration, "concentration_profile.csv");
  return 0;
}