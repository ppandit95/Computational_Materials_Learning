#pragma once
#include <cstddef>
#include "csr_matrix.hpp"

/**
 * @brief Assemble the 1D negative-Laplacian finite-difference matrix.
 *
 * Uses second-order centered differences on n interior grid points
 * with homogeneous Dirichlet boundary conditions.
 *
 * @param n Number of interior grid points. Must be positive.
 * @param dx Uniform grid spacing. Must be positive.
 * @return CSR representation of the discrete negative Laplacian.
 *
 * @throws std::invalid_argument if n == 0 or dx <= 0.
 */
[[nodiscard]] CSRMatrix make_1d_diffusion_matrix(std::size_t n, double dx);