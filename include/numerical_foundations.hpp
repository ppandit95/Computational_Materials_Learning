#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include<array>

/**
 * @brief Checks whether two floating-point numbers are nearly equal.
 *
 * Uses a combination of absolute and relative tolerances:
 *
 * |a - b| <= atol + rtol * max(|a|, |b|)
 *
 * @param a First floating-point number.
 * @param b Second floating-point number.
 * @param rtol Relative tolerance.
 * @param atol Absolute tolerance.
 * @return true if the numbers are nearly equal, false otherwise.
 * @throws std::invalid_argument if rtol or atol is negative.
 */
bool nearly_equal(double a, double b, double rtol, double atol) {
  if (rtol < 0.0 || atol < 0.0) {
    throw std::invalid_argument("Error: rtol and atol must be non-negative.");
  }

  return std::abs(a - b) <= (atol + rtol * std::max(std::abs(a), std::abs(b)));
}

/**
 * @brief Computes the discrete L1 error between two vectors.
 *
 * Computes
 *
 *     sum_i |numerical[i] - reference[i]|
 *
 * This is an unweighted discrete vector norm.
 *
 * @param numerical Numerical solution values.
 * @param reference Reference solution values.
 * @return Discrete L1 error. Returns 0.0 for two empty vectors.
 * @throws std::invalid_argument if the vector sizes differ.
 */
double l1_error(const std::vector<double> &numerical,
                const std::vector<double> &reference) {
  if (numerical.size() != reference.size()) {
    throw std::invalid_argument("The size of 2 vectors should match");
  }

  if (numerical.empty()) {
    return 0.0;
  }

  double sum = 0.0;

  for (std::size_t i = 0; i < numerical.size(); ++i) {
    const double error = numerical[i] - reference[i];

    sum += std::abs(error);
  }

  return sum;
}

/**
 * @brief Computes the discrete unweighted L2 error between two vectors.
 *
 * Computes
 *
 *     sqrt(sum_i (numerical[i] - reference[i])^2)
 *
 * This is an unweighted discrete vector norm. It is not yet a
 * quadrature approximation of the continuous spatial L2 norm,
 * which would also involve the grid spacing.
 *
 * @param numerical Numerical solution values.
 * @param reference Reference solution values.
 * @return Discrete L2 error. Returns 0.0 for two empty vectors.
 * @throws std::invalid_argument if the vector sizes differ.
 * @note the implementation uses scaled sum-of-squares to reduce intermediate
 * overflow/underflow.
 */
double l2_error(const std::vector<double> &numerical,
                const std::vector<double> &reference) {
  if (numerical.size() != reference.size()) {
    throw std::invalid_argument("The size of 2 vectors should match");
  }

  if (numerical.empty()) {
    return 0.0;
  }

  double scale = 0.0;
  double sumsq = 1.0;

  for (std::size_t i = 0; i < numerical.size(); ++i) {

    const double a = std::abs(numerical[i] - reference[i]);

    if (a != 0.0) {

      if (scale < a) {

        const double ratio = scale / a;

        sumsq = 1.0 + sumsq * ratio * ratio;

        scale = a;

      } else {

        const double ratio = a / scale;

        sumsq += ratio * ratio;
      }
    }
  }

  return scale * std::sqrt(sumsq);
}

/**
 * @brief Computes the discrete infinity norm of the error.
 *
 * Computes
 *
 *     max_i |numerical[i] - reference[i]|
 *
 * This is an unweighted discrete vector norm.
 *
 * @param numerical Numerical solution values.
 * @param reference Reference solution values.
 * @return Maximum absolute error. Returns 0.0 for two empty vectors as well as
 * when one of the vector consists of NaN
 * @throws std::invalid_argument if the vector sizes differ.
 */
double linf_error(const std::vector<double> &numerical,
                  const std::vector<double> &reference) {
  if (numerical.size() != reference.size()) {
    throw std::invalid_argument("The size of 2 vectors should match");
  }

  if (numerical.empty()) {
    return 0.0;
  }

  double max_error = 0.0;

  for (std::size_t i = 0; i < numerical.size(); ++i) {
    const double error = std::abs(numerical[i] - reference[i]);

    max_error = std::max(max_error, error);
  }

  return max_error;
}
/**
 * @brief Computes the relative L2 error between two vectors .
 *
 * Computes
 *
 *     sqrt(sum_i (numerical[i] - reference[i])^2)/sqrt(sum_i(reference[i])^2)
 *
 * This is an relative vector norm. It is not yet a
 * quadrature approximation of the continuous spatial L2 norm,
 * which would also involve the grid spacing.
 *
 * @param numerical Numerical solution values.
 * @param reference Reference solution values.
 * @return Relative L2 error. Returns 0.0 for two empty vectors.
 * @throws std::invalid_argument if the vector sizes differ as well as if
 * reference vecotr is a zero vector
 */
double relative_l2_error(const std::vector<double> &numerical,
                         const std::vector<double> &reference) {
  if (numerical.size() != reference.size()) {
    throw std::invalid_argument("The size of 2 vectors should match");
  }

  if (numerical.empty()) {
    throw std::invalid_argument("The input arrays are not invalid");
  }
  unsigned int count = 0;
  if (numerical.size() == reference.size()) {
    for (std::size_t i = 0; i < numerical.size(); ++i)
      if (nearly_equal(reference[i], 0.0, 1e-6, 1e-6))
        count++;
  }
  if (count == reference.size())
    throw std::invalid_argument(
        "Relative L2 error is undefined for a zero reference norm");
  double sum = 0.0;
  double sum1 = 0.0;

  for (std::size_t i = 0; i < numerical.size(); ++i) {
    const double error = numerical[i] - reference[i];
    sum1 += reference[i] * reference[i];
    sum += error * error;
  }

  return std::sqrt(sum) / std::sqrt(sum1);
}
/**
 * @brief Computes the relative condition number of the mathematical sqaure root
 * problem
 *
 * @param x Double precision
 * @return Condition Number which dictates how much f(x) perturbs upon
 * perturbing x
 * @throws std::invalid_argument if the input double is negative
 */
double sqrt_relative_condition_number(double x) {
  if (x <= 0.0 )
    throw std::invalid_argument(
        "Condition Numer of Negative Number cannot be determined");
  else {
    const double f = std::sqrt(x);
    const double fprime = 1.0 / (2.0 * std::sqrt(x));
    return std::abs(x * fprime) / std::abs(f);
  }
}
struct Matrix2x2 {
    double a11;
    double a12;
    double a21;
    double a22;
};
double determinant(const Matrix2x2& A){
  return A.a11*A.a22 - A.a12*A.a21;
}
double infinity_norm(const Matrix2x2& A)
{
    const double row1 =
        std::abs(A.a11) + std::abs(A.a12);

    const double row2 =
        std::abs(A.a21) + std::abs(A.a22);

    return std::max(row1, row2);
}
double infinity_norm_vec(const std::array<double,2> b){
  double max = 0.0;
  max = std::max(b[0],max);
  max = std::max(b[1],max);
  return max;
}

Matrix2x2 inverse(const Matrix2x2& A)
{
    const double det = determinant(A);

    if (det == 0.0) {
        throw std::invalid_argument(
            "Matrix is singular and inverse cannot be determined."
        );
    }

    Matrix2x2 A_inv;

    A_inv.a11 =  A.a22 / det;
    A_inv.a12 = -A.a12 / det;
    A_inv.a21 = -A.a21 / det;
    A_inv.a22 =  A.a11 / det;

    return A_inv;
}

double condition_number_inf(const Matrix2x2& A){
  return infinity_norm(A)*infinity_norm(inverse(A));
}