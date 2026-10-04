#pragma once
#include "csr_matrix.hpp"
#include <cstddef>
/**
 * @brief Assemble a square tridiagonal matrix in CSR format.
 *
 * Constructs an n-by-n matrix with constant lower-diagonal,
 * diagonal, and upper-diagonal entries.
 *
 * @param n Matrix dimension. Must be positive.
 * @param lower Value on the lower diagonal.
 * @param diagonal Value on the main diagonal.
 * @param upper Value on the upper diagonal.
 * @return Matrix stored in CSR format.
 *
 * @throws std::invalid_argument if n == 0.
 */
[[nodiscard]] CSRMatrix make_tridiagonal_matrix(std::size_t n, double lower, double diagonal, double upper);