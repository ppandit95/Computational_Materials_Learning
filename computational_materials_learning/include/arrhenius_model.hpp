#pragma once

#include <vector>
     /* @brief Structure to store parameters required to calculate diffusivities via Arrhenius Law
     *
     * @param Pre-exponential factor D0 in m^2/s.
     * @param Activation energy Q in J/mol.
     * @param Gas constant R in J/(mol K).
     */
struct ArrheniusParameters
{
    double D0;
    double Q;
    double R;
};
/**
 * @brief Represents an Arrhenius diffusivity model.
 *
 * Stores the parameters required for the Arrhenius relation
 *
 * @f[
 * D(T) = D_0 \exp\left(-\frac{Q}{RT}\right)
 * @f]
 *
 * and provides scalar and batch diffusivity evaluation.
 *
 * The model enforces physically meaningful parameter values
 * during construction.
 */
class ArrheniusModel
{
public:

    /**
     * @brief Constructs an Arrhenius diffusivity model.
     * @param D0 Pre-exponential factor in m^2/s. Must be greater than 0.
     * @param Q Activation energy in J/mol. Must be greater than or equal to 0.
     * @param R Gas constant in J/(mol K). Must be greater than 0.
     * @note The explicit specifier prevents this constructor from being
     *       used for implicit conversion from ArrheniusParameters to
     *       ArrheniusModel.
     * @throws std::invalid_argument If D0 <= 0, Q < 0, or R <= 0.
     */
    ArrheniusModel(double D0, double Q, double R);
    explicit ArrheniusModel(const ArrheniusParameters& parameters);


    /**
     * @brief Computes the diffusivity at a single temperature.
     *
     * Evaluates
     * @f[
     * D(T) = D_0 \exp\left(-\frac{Q}{RT}\right).
     * @f]
     *
     * @param T Absolute temperature in K. Must be greater than 0.
     *
     * @return Diffusivity in m^2/s.
     *
     * @throws std::invalid_argument If T <= 0.
     */
    double diffusivity(double T) const;


    /**
     * @brief Computes diffusivities for a sequence of temperatures.
     *
     * Each temperature is evaluated using diffusivity().
     *
     * @param temperatures Absolute temperatures in K.
     *                     Every value must be greater than 0.
     *
     * @return Diffusivities in m^2/s in the same order as the
     *         supplied temperatures.
     *
     * @throws std::invalid_argument If any temperature is <= 0.
     */
    std::vector<double> diffusivities(
        const std::vector<double>& temperatures) const;


    /**
     * @brief Returns the Arrhenius pre-exponential factor.
     *
     * @return Pre-exponential factor D0 in m^2/s.
     */
    double pre_exponential_factor() const;


    /**
     * @brief Returns the activation energy of the model.
     *
     * @return Activation energy Q in J/mol.
     */
    double activation_energy() const;


    /**
     * @brief Returns the gas constant used by the model.
     *
     * @return Gas constant R in J/(mol K).
     */
    double gas_constant() const;
    
    /**
     * @brief Campare two ArrheniusModel Objects by comparing their data members
     *
     * @return Gas wheter two ArrheniusModel objects are equal or not
     */
    bool operator=(const ArrheniusModel& b); 

private:
    const double D0_;
    const double Q_;
    const double R_;
};


