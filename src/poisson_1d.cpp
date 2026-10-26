#include <cmath>
#include <cstddef>
#include <stdexcept>

#include "poisson_1d.hpp"
#include "tridiagonal.hpp"

CSRMatrix build_poisson_1d(std::size_t num_interior, double length)
{
    if (num_interior == 0 || !std::isfinite(length) || length <= 0.0)
    {
        throw std::invalid_argument(
            "Number of interior points must be positive and domain length finite and positive");
    }

    const double h = length / static_cast<double>(num_interior + 1);

    if (!std::isfinite(h) || h <= 0.0)
    {
        throw std::invalid_argument("Invalid grid spacing");
    }

    const double inv_h2 = 1.0 / (h * h);

    if (!std::isfinite(inv_h2) || !std::isfinite(2.0 * inv_h2))
    {
        throw std::invalid_argument("Invalid Poisson operator coefficients");
    }

    return make_tridiagonal_matrix(
        num_interior,
        -inv_h2,
        2.0 * inv_h2,
        -inv_h2);
}

std::vector<double> build_poisson_rhs(const std::vector<double> &source_values, double length, double u_left, double u_right)
{
    const std::size_t N = source_values.size();
    if (N == 0)
    {
        throw std::invalid_argument(
            "The source vector must contain at least one interior point.");
    }
    if (!std::isfinite(length) || length <= 0.0)
    {
        throw std::invalid_argument("The provided domain length is incompatible.");
    }
    if (!std::isfinite(u_left) || !std::isfinite(u_right))
    {
        throw std::invalid_argument("The provided boundary values are incompatible.");
    }
    for (double source : source_values)
    {
        if (!std::isfinite(source))
            throw std::invalid_argument("The source values vector is incompatible");
    }
    double h = length / static_cast<double>(N + 1);
    double scaling = 1.0 / (h * h);
    if (!std::isfinite(h) || h <= 0.0)
    {
        throw std::invalid_argument("Invalid grid spacing.");
    }
    if (!std::isfinite(scaling))
    {
        throw std::invalid_argument("Invalid Poisson scaling coefficient.");
    }
    std::vector<double> rhs = source_values;
    if (N >= 1)
    {
        rhs.front() += u_left * scaling;
        rhs.back() += u_right * scaling;
    }
    for (double value : rhs)
    {
        if (!std::isfinite(value))
        {
            throw std::invalid_argument(
                "Nonfinite value generated in Poisson RHS.");
        }
    }
    return rhs;
}