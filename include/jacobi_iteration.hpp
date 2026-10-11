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

/**
 * @brief Result of a Jacobi Iterative Solve
 * The residual norm corresponds to the returned iterate
 */
struct JacobiResult
{
    std::vector<double> x;
    std::size_t iterations = 0;
    double residual_norm = 0.0;
    bool converged = false;
};
/**
 * @brief Solve Ax=b using repeated Jacoobi iterations
 * @param A Square CSR Matrix
 * @param b Right-hand-side vector
 * @param diagonal Diagonal entries of A
 * @param x0 Initial Approximation
 * @param tolerance Positive,finite Residual Tolerance
 * @param max_iterations Maximum number of Jacobi updates
 * @return Final iterate,iterationo count,residual norm and coonvergence status.
 * @throws std::invalid_argument for incoompatible dimensions ,invalid tolerance,zero iteration limit,or invalid diagonal
 * @note Uses relative residual for non zero b and absolute residual for zero b.The supplied diagonal must match A.
 */
JacobiResult solve_jacobi(const CSRMatrix &A, const std::vector<double> &b, const std::vector<double> &diagonal, const std::vector<double> &x0, double tolerance, std::size_t max_iterations);