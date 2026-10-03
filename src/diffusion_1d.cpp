#include "diffusion_1d.hpp"
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
    std::vector<double> values;
    std::vector<std::size_t> col_indices;
    std::vector<std::size_t> row_ptr;
    row_ptr.push_back(0);
    values.reserve(3 * n - 2);
    values.reserve(3 * n - 2);
    row_ptr.reserve(n + 1);
    // row_ptr[n] = 3*n-2;
    for (std::size_t i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            values.push_back(-inv_dx2);
            col_indices.push_back(i - 1);
        }
        values.push_back(2.0 * inv_dx2);
        col_indices.push_back(i);

        if (i + 1 < n)
        {
            values.push_back(-inv_dx2);
            col_indices.push_back(i + 1);
        }
        row_ptr.push_back(values.size());
    }
    return CSRMatrix(n, n, std::move(values), std::move(col_indices), std::move(row_ptr));
}