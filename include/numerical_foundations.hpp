#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

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
bool nearly_equal(double a,
                  double b,
                  double rtol,
                  double atol)
{
    if (rtol < 0.0 || atol < 0.0) {
        throw std::invalid_argument(
            "Error: rtol and atol must be non-negative.");
    }

    return std::abs(a - b) <=
           (atol + rtol * std::max(std::abs(a), std::abs(b)));
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
double l1_error(const std::vector<double>& numerical,
                const std::vector<double>& reference)
{
    if (numerical.size() != reference.size()) {
        throw std::invalid_argument(
            "The size of 2 vectors should match");
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
 * @brief Computes the discrete L2 error between two vectors.
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
 */
double l2_error(const std::vector<double>& numerical,
                const std::vector<double>& reference)
{
    if (numerical.size() != reference.size()) {
        throw std::invalid_argument(
            "The size of 2 vectors should match");
    }

    if (numerical.empty()) {
        return 0.0;
    }

    double sum = 0.0;

    for (std::size_t i = 0; i < numerical.size(); ++i) {
        const double error = numerical[i] - reference[i];

        sum += error * error;
    }

    return std::sqrt(sum);
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
 * @return Maximum absolute error. Returns 0.0 for two empty vectors.
 * @throws std::invalid_argument if the vector sizes differ.
 */
double linf_error(const std::vector<double>& numerical,
                  const std::vector<double>& reference)
{
    if (numerical.size() != reference.size()) {
        throw std::invalid_argument(
            "The size of 2 vectors should match");
    }

    if (numerical.empty()) {
        return 0.0;
    }

    double max_error = 0.0;

    for (std::size_t i = 0; i < numerical.size(); ++i) {
        const double error =
            std::abs(numerical[i] - reference[i]);

        max_error = std::max(max_error, error);
    }

    return max_error;
}