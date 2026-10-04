#include "tridiagonal.hpp"
#include <stdexcept>
#include <utility>
#include <vector>
CSRMatrix make_tridiagonal_matrix(std::size_t n, double lower, double diagonal, double upper)
{
    if (n == 0)
        throw std::invalid_argument("Matrix Dimension must be positive.\n");
    std::vector<double> values;
    std::vector<std::size_t> col_indices;
    std::vector<std::size_t> row_ptr;

    values.reserve(3 * n - 2);
    col_indices.reserve(3 * n - 2);
    row_ptr.reserve(n + 1);

    row_ptr.push_back(0);
    for (std::size_t i = 0; i < n; ++i)
    {
        if (lower != 0.0 && i > 0)
        {
            values.push_back(lower);
            col_indices.push_back(i - 1);
        }
        values.push_back(diagonal);
        col_indices.push_back(i);
        if (i + 1 < n && upper != 0)
        {
            values.push_back(upper);
            col_indices.push_back(i + 1);
        }
        row_ptr.push_back(values.size());
    }
    return CSRMatrix(n, n, std::move(values), std::move(col_indices), std::move(row_ptr));
}