#include "numerical_foundations.hpp"
#include<fstream>
#include<iomanip>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
int main() {
  std::ofstream out("data/l2_stability.csv");
  out << std::scientific << std::setprecision(17);
  out << "exponent,naive,stable,reference" << std::endl;
  for (int i = -300; i <= 300; i += 10) {
    std::vector<double> error(2);
    error[0] = 1.0 * std::pow(10, i);
    error[1] = 1.0 * std::pow(10, i);
    const double naive = std::sqrt(error[0] * error[0] + error[1] * error[1]);
    const std::vector<double> zero{0.0, 0.0};
    const double stable = l2_error(error, zero);
    out << i << " , " << naive << " , " << stable << " , "
        << std::sqrt(2) * std::pow(10, i) << std::endl;
  }
  out.close();
  return 0;
}