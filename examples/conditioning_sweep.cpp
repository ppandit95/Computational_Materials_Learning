#include "numerical_foundations.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
int main() {
  std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);
  const std::array<double, 7> epsilons{1e-1, 1e-2, 1e-3, 1e-4,
                                       1e-5, 1e-6, 1e-7};
  std::cout << "Epsilon             Determinant         Cond_inf            "
               "epsilon*cond_inf  "
            << std::endl;
  std::cout << std::scientific;
  for (double eps : epsilons) {
    Matrix2x2 A{1.0, 1.0, 1.0, 1.0 + eps};
    std::cout << eps << "           " << determinant(A) << "            "
              << condition_number_inf(A) << "         "
              << eps * condition_number_inf(A) << std::endl;
  }

  return 0;
}