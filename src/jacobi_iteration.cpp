#include "jacobi_iteration.hpp"
#include "linear_solver_diagnostics.hpp"
#include "numerical_foundations.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>
#include <cstddef>

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

JacobiResult solve_jacobi(
    const CSRMatrix &A,
    const std::vector<double> &b,
    const std::vector<double> &diagonal,
    const std::vector<double> &x0,
    double tolerance,
    std::size_t max_iterations)
{
    // 1. Validate dimensions.
    if (A.rows() != A.cols())
    {
        throw std::invalid_argument(
            "Jacobi requires a square matrix.");
    }

    if (x0.size() != A.cols())
    {
        throw std::invalid_argument(
            "Initial approximation size does not match matrix columns.");
    }

    if (b.size() != A.rows())
    {
        throw std::invalid_argument(
            "RHS size does not match matrix rows.");
    }

    if (diagonal.size() != A.rows())
    {
        throw std::invalid_argument(
            "Diagonal size does not match matrix rows.");
    }

    if (!std::isfinite(tolerance) || tolerance <= 0.0)
    {
        throw std::invalid_argument(
            "Tolerance must be positive and finite.");
    }

    if (max_iterations == 0)
    {
        throw std::invalid_argument(
            "Maximum iterations must be positive.");
    }

    for (double d : diagonal)
    {
        if (!std::isfinite(d) || d == 0.0)
        {
            throw std::invalid_argument(
                "Jacobi requires finite, nonzero diagonal entries.");
        }
    }

    // 2. Initialize the approximate solution.
    std::vector<double> tmp = x0;

    // 3. Construct a zero vector for computing L2 norms.
    const std::vector<double> reference(b.size(), 0.0);

    const double b_norm = l2_error(b, reference);

    // 4. Calculate the initial residual.
    auto residual = compute_residual(A, tmp, b);

    double residual_norm = l2_error(residual, reference);

    auto convergence_measure = [&]() -> double
    {
        return (b_norm > 0.0)
                   ? residual_norm / b_norm
                   : residual_norm;
    };

    // 5. Check whether the initial guess already converged.
    if (convergence_measure() <= tolerance)
    {
        return JacobiResult{
            tmp,
            0,
            residual_norm,
            true};
    }

    // 6. Perform Jacobi iterations.
    for (std::size_t i = 1; i <= max_iterations; ++i)
    {
        tmp = jacobi_step(A, tmp, b, diagonal);

        residual = compute_residual(A, tmp, b);
        residual_norm = l2_error(residual, reference);

        if (convergence_measure() <= tolerance)
        {
            return JacobiResult{
                tmp,
                i,
                residual_norm,
                true};
        }
    }

    // 7. Maximum iterations reached without convergence.
    return JacobiResult{
        tmp,
        max_iterations,
        residual_norm,
        false};
}
