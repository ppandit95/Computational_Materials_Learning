#pragma once
#include<vector>
/**
 * @brief Computes diffusivity from the Arrhenius relation.
 *
 * Evaluates
 * @f[
 * D = D_0 \exp\left(-\frac{Q}{RT}\right).
 * @f]
 *
 * @param T Absolute temperature in K. Must be greater than 0.
 * @param D0 Pre-exponential factor in m^2/s.
 * @param Q Activation energy in J/mol.
 * @param R Gas constant in J/(mol K).
 *
 * @return Diffusivity in m^2/s.
 *
 * @throws std::invalid_argument If T is less than or equal to 0 K.
 */
double arrhenius_diffusivity(double T,
                             double D0,
                             double Q,
                             double R);
/**
 * @brief Computes Arrhenius diffusivities for a sequence of temperatures.
 *
 * @param temperatures Absolute temperatures in K.
 *                     Every temperature must be greater than 0.
 * @param D0 Pre-exponential factor in m^2/s.
 * @param Q Activation energy in J/mol.
 * @param R Gas constant in J/(mol K).
 *
 * @return Diffusivities in m^2/s in the same order as the input
 *         temperatures.
 *
 * @throws std::invalid_argument If any temperature is less than
 *         or equal to 0 K.
 */
std::vector<double> arrhenius_diffusivities(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R);
/**
 * @brief Computes Arrhenius diffusivities using incremental
 *        vector insertion.
 *
 * @param temperatures Absolute temperatures in K.
 * @param D0 Pre-exponential factor in m^2/s.
 * @param Q Activation energy in J/mol.
 * @param R Gas constant in J/(mol K).
 *
 * @return Diffusivities in m^2/s.
 *
 * @throws std::invalid_argument If any temperature is less than
 *         or equal to 0 K.
 *
 * @note This function is retained as an educational alternative
 *       implementation using reserve() and push_back().
 */
std::vector<double> arrhenius_diffusivities_empty_pushback(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R);
/**
 * @brief Tests whether a sequence is monotonically non-decreasing.
 *
 * @param values Sequence of values to inspect.
 *
 * @return true if every element is greater than or equal to its
 *         predecessor; false otherwise.
 *
 * @note Empty and single-element sequences are considered
 *       monotonically non-decreasing.
 */
bool is_monotonically_non_decreasing(
     std::vector<double>& values);
/**
 * @brief Tests two floating-point values for approximate equality
 *        using relative tolerance.
 *
 * The comparison satisfies
 * @f[
 * |a-b| \le r_{\mathrm{tol}}\max(|a|,|b|).
 * @f]
 *
 * @param a First value to compare.
 * @param b Second value to compare.
 * @param relative_tolerance Non-negative relative tolerance.
 *
 * @return true if the relative-tolerance criterion is satisfied;
 *         false otherwise.
 *
 * @throws std::invalid_argument If relative_tolerance is negative.
 *
 * @note This function currently uses relative tolerance only.
 *       No absolute tolerance is applied near zero.
 */
bool approximately_equal(
    double a,
    double b,
    double relative_tolerance=1.0e-6);
