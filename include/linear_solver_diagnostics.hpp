#pragma once

#include "csr_matrix.hpp"

#include <vector>

/**
 * @brief Compute the residual of a Linear System
 *
 * Computes r = b - A*x
 *
 * @param A Sparse matrix in CSR format
 * @param x Approximate solution vector
 * @param b Right hand side vector
 *
 * @return Residual vector with the same units as b
 *
 * @throws std::invalid_argument if rows of A donot match with length of approximate solution vector
 *
 * @note A is not modified.The returned vector owns its data
 */
std::vector<double> compute_residual(const CSRMatrix &A, const std::vector<double> &x, const std::vector<double> &b);