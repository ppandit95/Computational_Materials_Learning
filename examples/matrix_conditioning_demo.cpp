#include "numerical_foundations.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
std::array<double, 2> solve_linear_system(Matrix2x2 A,
                                          std::array<double, 2> b) {
  Matrix2x2 A_inv = inverse(A);
  std::array<double, 2> x;
  x[0] = A_inv.a11 * b[0] + A_inv.a12 * b[1];
  x[1] = A_inv.a21 * b[0] + A_inv.a22 * b[1];
  return x;
}
int main() {
  std::setprecision(std::numeric_limits<double>::max_digits10);
  Matrix2x2 A;
  A.a11 = 1;
  A.a12 = 1;
  A.a21 = 1;
  A.a22 = 1.0001;
  std::array<double, 2> b = {2, 2.0001};
  std::array<double, 2> b_tilde = {2, 2.00011};
  std::array<double, 2> x = solve_linear_system(A, b);
  std::array<double, 2> x_tilde = solve_linear_system(A, b_tilde);
  std::array<double, 2> b_diff = {0.0, 0.00001};
  std::array<double, 2> x_diff = {x_tilde[0] - x[0], x_tilde[1] - x[1]};
  double r_b = infinity_norm_vec(b_diff) / infinity_norm_vec(b);
  double r_x = infinity_norm_vec(x_diff) / infinity_norm_vec(x);
  std::cout << "Determinant of A : " << determinant(A) << std::endl;
  std::cout << "Condition Number of A :" << condition_number_inf(A)
            << std::endl;
  std::cout << "Original B Vector :" << b[0] << "," << b[1] << std::endl;
  std::cout << "Perturbed B Vector :" << b_tilde[0] << "," << b_tilde[1]
            << std::endl;
  std::cout << "Original x Vector :" << x[0] << "," << x[1] << std::endl;
  std::cout << "Perturbed x Vector :" << x_tilde[0] << "," << x_tilde[1]
            << std::endl;
  std::cout << "Relative Perturbation in b Vector :" << r_b << std::endl;
  std::cout << "Relative Change in x Vector :" << r_x << std::endl;
  std::cout << "Amplification r_x/r_b:" << r_x / r_b << std::endl;
  std::cout<<" Condition Number times r_b : " << condition_number_inf(A)*r_b << std::endl;

  return 0;
}