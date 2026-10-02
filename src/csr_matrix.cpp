
#include "csr_matrix.hpp"
#include <utility>
#include <stdexcept>
#include <vector>
CSRMatrix::CSRMatrix(std::size_t rows, std::size_t cols, std::vector<double> values, std::vector<std::size_t> col_indices, std::vector<std::size_t> row_ptr)
    : rows_(rows), cols_(cols), values_(std::move(values)), col_indices_(std::move(col_indices)), row_ptr_(std::move(row_ptr))
{
}
std::vector<double> CSRMatrix::matvec(const std::vector<double> &x) const
{
    if (x.size() != cols_)
    {
        throw std::invalid_argument("Input vector size does not match the number of columns in the matrix.");
    }
    std::vector<double> result(rows_, 0.0);
    for (std::size_t i = 0; i < rows_; ++i)
    {
        for (std::size_t j = row_ptr_[i]; j < row_ptr_[i + 1]; ++j)
        {
            result[i] += values_[j] * x[col_indices_[j]];
        }
    }
    return result;
}
std::size_t CSRMatrix::rows() const noexcept
{
    return rows_;
}
std::size_t CSRMatrix::cols() const noexcept
{
    return cols_;
}
std::size_t CSRMatrix::nnz() const noexcept
{
    return values_.size();
}