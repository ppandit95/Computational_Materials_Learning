#include "diffusion_1d.hpp"
#include "tridiagonal.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <utility>
CSRMatrix make_1d_diffusion_matrix(std::size_t n, double dx)
{
    if (n == 0)
        throw std::invalid_argument("Number of interior points must be positive.");
    if (dx <= 0.0)
        throw std::invalid_argument("Grid spacing must be positive");
    const double inv_dx2 = 1.0 / (dx * dx);
    return make_tridiagonal_matrix(n, -1.0 * inv_dx2, 2.0 * inv_dx2, -1.0 * inv_dx2);
}