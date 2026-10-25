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