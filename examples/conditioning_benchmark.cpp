#include <iostream>
#include <cmath>
#include <iomanip>
#include "numerical_foundations.hpp"
#include <array>
#include <fstream>
#include <limits>
int main()
{
    std::ofstream out("data/conditioning_results.csv");
    out << std::scientific << std::setprecision(std::numeric_limits<double>::max_digits10);
    out << "epsilon,determinant,condition_number,scaled_condition\n";
    const std::array<double, 10> epsilons{1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9, 1e-10};
    Matrix2x2 A;
    A.a11 = 1;
    A.a12 = 1;
    A.a21 = 1;
    for (double epsilon : epsilons)
    {
        A.a22 = 1 + epsilon;
        out << epsilon << "," << determinant(A) << "," << condition_number_inf(A) << "," << epsilon * condition_number_inf(A) << std::endl;
    }
    out.close();
    return 0;
}