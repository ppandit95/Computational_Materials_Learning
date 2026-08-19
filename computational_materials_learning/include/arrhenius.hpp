#pragma once
#include<vector>
double arrhenius_diffusivity(double T,
                             double D0,
                             double Q,
                             double R);
/**
 * @brief A public function which returns a vector of diffusivities for the supplied temperature range using arrhenius relationship
 *
 * @param temperatures - A Range of supplied temperatures wherein one wishes to determine the corresponding diffusivities(in K)
 * @param D0 - pre exponential factor used while calculation of Diffuisitie fitted into arrhenius equation in the literature(in m^2/sec)
 * @param Q - Activation energy for jump of atoms of that specific element so that corresponding diffusivity could be calculated(in J/mol)
 * @param R - Universal Gas Constant required in Arrhenius Relationship(in J/(mol K))
 * @return - A vector of diffusivites at corresponding temperatures(in m^2/sec)
 *
 * @note - To check the scientific feasibility of arrhenius relationship as a dignostic, monotonicity of increasing Diffusivities has been checked with increase in temperatures.Also, the provided temperature should either be greater than 0 K or equal to it else the diffusivity will not make sense physically.
 *
 * @throws std::invalid_argument which ensures that Temperature should be greater than 0
 */
std::vector<double> arrhenius_diffusivities(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R);
std::vector<double> arrhenius_diffusivities_empty_pushback(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R);
/**
 * @brief A diagnostic function to check the monootonicity of Diffusivities with Temperature
 *
 * @param values a vector of values
 * @return a boolean value
 *
 * @note It checks for monotonicity of values in Diffusivity Vector
 */
bool is_monotonically_non_decreasing(
     std::vector<double>& values);
     /**
 * @brief A diagnostic function to check whether 2 float quantities are approximately equal or not
 *
 * @param a , b,  relative tolerance as float values 
 * @return true/false stating whether the quantities are true or false
 * @throws an invalid_argument which informs that relative tolerance is negative which donot make sense
 * @note In case if a and b are less than machine precision then we can consider them to be equal 
 */
bool approximately_equal(
    double a,
    double b,
    double relative_tolerance);
