#include "linear_solver_diagnostics.hpp"

#include <cstddef>
#include <stdexcept>
#include <vector>

std::vector<double> compute_residual(
    const CSRMatrix &A,
    const std::vector<double> &x,
    const std::vector<double> &b)
{
    // Validate dimensions.
    if (A.rows() != b.size())
    {
        throw std::invalid_argument(
            "RHS size must match matrix rows");
    }

    if (A.cols() != x.size())
    {
        throw std::invalid_argument(
            "Solution size must match matrix columns");
    }

    // Compute A*x using the existing CSR implementation.
    const std::vector<double> Ax = A.matvec(x);

    // Allocate residual vector.
    std::vector<double> residual(b.size());

    // Compute r = b - A*x.
    for (std::size_t i = 0; i < b.size(); ++i)
    {
        residual[i] = b[i] - Ax[i];
    }

    return residual;
}