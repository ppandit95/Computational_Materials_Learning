#include "numerical_foundations.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
int main() {
  std::setprecision(std::numeric_limits<double>::max_digits10);
  const double x = 100.0;
  const double relative_input_perturbation = 1e-6;
  const double perturbed_x = x * (1.0 + relative_input_perturbation);
  const double y = std::sqrt(x);
  const double y_perturbed = std::sqrt(perturbed_x);
  const double rx = std::abs(perturbed_x - x) / std::abs(x);
  const double ry = std::abs(y_perturbed - y) / std::abs(y);
  std::cout << "Relative Input Perturbation : " << rx << std::endl;
  std::cout << "Relative Output Perturbation : " << ry << std::endl;
  std::cout << "Condition Number : " << sqrt_relative_condition_number(x)
            << std::endl;
  std::cout << "Predicted Output Perturbation : "
            << sqrt_relative_condition_number(x) * rx << std::endl;

  return 0;
}