#include "jacobi_iteration.hpp"
#include "linear_solver_diagnostics.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>

std::vector<double> jacobi_step(const CSRMatrix &A, const std::vector<double> &x, const std::vector<double> &b, const std::vector<double> &diagonal)
{
    if (diagonal.size() != b.size())
        throw std::invalid_argument("Jacobi diagonal and RHS sizes must match");
    const auto residual = compute_residual(A, x, b);
    std::vector<double> x_new = x;

    for (std::size_t i = 0; i < residual.size(); i++)
    {
        if (!std::isfinite(diagonal[i]) || diagonal[i] == 0.0)
        {
            throw std::invalid_argument("Jacoobi requires finite , non-zero diagonal entries.");
        }
        x_new[i] += residual[i] / diagonal[i];
    }
    return x_new;
}