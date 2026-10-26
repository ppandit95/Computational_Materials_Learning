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

/**
 * @brief Construct the right-hand-side vector for the 1D Poisson equation.
 *
 * Uses second-order centered finite differences on N interior grid points
 * with prescribed (possibly nonzero) Dirichlet boundary conditions.
 *
 * Solves the discrete formulation of:
 *
 *     -d²u/dx² = f(x),  0 < x < L
 *
 * with boundary conditions:
 *
 *     u(0) = u_left,  u(L) = u_right.
 *
 * The grid spacing is h = length / (N + 1).
 * Boundary contributions are incorporated into the RHS:
 *
 *     rhs[0]   = source_values[0]   + u_left / h²
 *     rhs[N-1] = source_values[N-1] + u_right / h²
 *
 * For N = 1, both boundary contributions are added to the same entry.
 *
 * @param source_values Source function evaluated at N interior grid points.
 *                      The vector must be nonempty and contain finite values.
 * @param length Length of the 1D computational domain (L > 0).
 * @param u_left Prescribed Dirichlet boundary value at x = 0.
 * @param u_right Prescribed Dirichlet boundary value at x = L.
 *
 * @return std::vector<double> Right-hand-side vector of size N,
 *         including contributions from nonzero Dirichlet boundaries.
 *
 * @throws std::invalid_argument if source_values is empty, length is
 *         nonpositive or nonfinite, boundary/source values are nonfinite,
 *         or the computed grid spacing, scaling, or RHS is invalid.
 *
 * @note The coefficient matrix remains unchanged when Dirichlet
 *       boundary values are modified; only the RHS is affected.
 * @note The returned RHS is consistent with the Poisson matrix
 *       scaled by 1/h².
 */
std::vector<double> build_poisson_rhs(const std::vector<double> &source_values, double length, double u_left, double u_right);