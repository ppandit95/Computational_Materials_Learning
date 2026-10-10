#pragma once

#include "csr_matrix.hpp"

#include <vector>

/**
 * @brief Perform one Jacobi Iteration
 *
 * Computes x_new = x + D^(-1)(b-A*x)
 *
 * @param A Sparse CSR system matrix
 * @param x Current approximate solution
 * @param b Right-hand-side vector
 * @param diagonal Diagonal entries of A,in row order
 *
 * @return Updated solution vector
 *
 * @throws std::invaslid_argument if dimensioons are incompatible or a diagonal entry is zero of non finite
 * @note The supplied diagonal must match A exactly.
 *       Input vectors are not modified.
 *       Returned vector owns its storage
 */
std::vector<double> jacobi_step(const CSRMatrix &A, const std::vector<double> &x, const std::vector<double> &b, const std::vector<double> &diagonal);