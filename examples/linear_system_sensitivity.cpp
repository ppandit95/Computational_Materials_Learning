#include "numerical_foundations.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <cmath>
int main()
{
    const Matrix2x2 A{1.0, 1.0, 1.0, 1.0001};
    std::cout << std::setprecision(6);
    const Vector2 x_exact{1.0, 1.0};
    const Vector2 b = matvec(A, x_exact);
    const double delta = 1e-8;
    const Vector2 b_perturbed{b.x1, b.x2 + delta};
    const Vector2 x_calc = solve(A, b);
    const Vector2 x_perturbed = solve(A, b_perturbed);
    const Vector2 db = {b_perturbed.x1 - b.x1, b_perturbed.x2 - b.x2};
    const Vector2 dx = {x_perturbed.x1 - x_calc.x1, x_perturbed.x2 - x_calc.x2};
    const double r_b = infinity_norm(db) / infinity_norm(b);
    const double r_x = infinity_norm(dx) / infinity_norm(x_calc);
    const double B = condition_number_inf(A) * r_b;
    const double R = r_x / r_b;
    std::cout << "Kappa_inf(A) : "<<condition_number_inf(A)<<std::endl;
    std::cout << "Relative_b_perturbation : " << r_b <<std::endl;
    std::cout << "Relative_x_change : " << r_x << std::endl;
    std::cout << "Kappa times relative_b : " << B << std::endl;
    std::cout << "observed_amplification : " << R << std::endl;
    return 0;
}