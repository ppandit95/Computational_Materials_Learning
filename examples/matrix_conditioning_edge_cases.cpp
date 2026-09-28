#include "numerical_foundations.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
int main()
{
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);
    const std::array<double, 8> epsilons{
        1e-10,
        1e-12,
        1e-14,
        1e-15,
        1e-16,
        1e-17,
        1e-18,
        0.0};
    std::cout << "Epsilon             Determinant         1+Epsilon            "
                 "Distinguishable  "
              << std::endl;
    std::cout << std::scientific;
    for (double eps : epsilons)
    {
        Matrix2x2 A{1.0, 1.0, 1.0, 1.0 + eps};
        /*For small values of epsilon like 1e-17, the machine actually rounds to a singular matrix 
        */
        const bool distinguishable = (1.0 + eps != 1.0);
        std::cout << eps << "           " << determinant(A) << "            "
                  << 1 + eps << "         "
                  << distinguishable << std::endl;
    }

    return 0;
}