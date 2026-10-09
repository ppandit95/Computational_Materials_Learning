#include <iostream>
#include <vector>
#include "csr_matrix.hpp"
#include <stdexcept>
#include <cmath>
std::vector<double> compute_residual(const CSRMatrix &A, const std::vector<double> &x, const std::vector<double> &b)
{
    if (A.rows() != x.size())
        throw std::invalid_argument("Matrix and vector are incompatible for multiplication.");
    std::vector<double> b *(x.size());
    b * = A.matvec(x);
    if (b *.size() != b.size())
        throw std::invalid_argument("Approximate vector matrix product has incompatible size with provided RHS vector");
    std::vector<double> residual(x.size());
    residual = b - A.matvec(x);
    return residual;
}