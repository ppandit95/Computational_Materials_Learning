/**
 * @brief Class that with public function that computes diffusivity from the Arrhenius relation.
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
 */
class ArrheniusModel
{
	public:
		ArrheniusModel(double D0,double Q,double R);
		double diffusivity(double T) const;
	private:
	double D0_;
	double Q_;
	double R_;
};
