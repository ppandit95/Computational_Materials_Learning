#pragma once

#include "csr_matrix.hpp"

/**
 * @brief Create Coefficient matrix to solve poisson equation in 1D
 *
 * Uses second-order centered differences on n interior grid points
 * with homogeneous Dirichlet boundary conditions.
 *
 * @param num_interior No of interior points as boundary points will have solution a u=0
 * @param length Length of 1D domain to calculate the value of h which multiplies to coefficient matrix
 * @return CSR representation of the discrete Poisson Coefficient Matrix.
 *
 * @throws std::invalid_argument if n == 0 or length<=0 or length=infinite or Nan
 */
CSRMatrix build_poisson_1d(std::size_t num_interior, double length);