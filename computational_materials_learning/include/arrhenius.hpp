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
