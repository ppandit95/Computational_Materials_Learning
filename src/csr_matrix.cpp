
#include "csr_matrix.hpp"
#include <utility>
#include <stdexcept>
#include <vector>

/**
 * @brief Constructor for CSRMatrix class to create a CSR representation of a sparse matrix.
 * @param rows Number of rows in the sparse matrix.
 * @param cols Number of columns in the sparse matrix.
 * @param values Vector storing non-zero values of the matrix.
 * @param col_indices Vector storing column indices corresponding to non-zero entries.
 * @param row_ptr Vector tracking the start position of non-zero entries for each row.
 * @throws std::invalid_argument if row_ptr size does not equal rows + 1.
 * @throws std::invalid_argument if values and col_indices sizes do not match.
 * @throws std::invalid_argument if the first element of row_ptr is not 0.
 * @throws std::invalid_argument if the last element of row_ptr does not equal nnz (size of values).
 * @throws std::invalid_argument if row_ptr is not monotonically increasing.
 * @throws std::invalid_argument if col_indices contains indices outside the valid column range.
 */
CSRMatrix::CSRMatrix(std::size_t rows, std::size_t cols, std::vector<double> values, std::vector<std::size_t> col_indices, std::vector<std::size_t> row_ptr)
    : rows_(rows), cols_(cols)
{
    if (row_ptr.size() != rows_ + 1)
    {
        throw std::invalid_argument("Row_ptr array is not of expected size and leads to invalid CSR representation.\n");
    }
    if (values.size() != col_indices.size())
        throw std::invalid_argument("Sizes of values array and col_indices array donot match leading to invalid CSR representation.\n");
    if (row_ptr.front() != 0)
        throw std::invalid_argument("First element of row_ptr array isnt 0 leading to invalid CSR representation.\n");
    if (row_ptr.back() != values.size())
        throw std::invalid_argument("The last element of row_ptr array isnt nnz which is not valid CSR representation \n");
    for (std::size_t i = 0; i < row_ptr.size() - 1; ++i)
    {
        if (row_ptr[i] > row_ptr[i + 1])
            throw std::invalid_argument("Element values in row_ptr are not incresing with increasing indices which is invalid CSR representation.\n");
        for (std::size_t j = row_ptr[i]; j < row_ptr[i + 1]; ++j)
        {
            if (col_indices[j] >= cols_)
                throw std::invalid_argument("the index values are greter than maximum column indices possible in A matrix \n");
        }
    }
    row_ptr_ = std::move(row_ptr);
    values_ = std::move(values);
    col_indices_ = std::move(col_indices);
}
/**
 * @brief Member function to compute matrix vector multiplication through CSR representation.
 * @param x Vector to be multiplied with the corresponding matrix CSR representation.
 * @throws std::invalid_argument if size of vector does not match the number of columns in the matrix.
 * @return A vector result which depicts the matrix vector product.
 */
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
/**
 * @brief Returns the number of rows in the sparse matrix.
 *
 * @return std::size_t The number of rows in the CSR matrix.
 *
 * @note This function does not throw exceptions.
 */
std::size_t CSRMatrix::rows() const noexcept
{
    return rows_;
}
/**
 * @brief Returns the number of columns in the CSR matrix.
 *
 * @return std::size_t The number of columns in the matrix.
 *
 * @note This function is marked with noexcept and does not modify the matrix state.
 */
std::size_t CSRMatrix::cols() const noexcept
{
    return cols_;
}
/**
 * @brief Returns the number of non-zero elements in the CSR matrix.
 *
 * @return std::size_t The count of non-zero elements stored in the values array.
 *
 * @note This operation has O(1) time complexity as it simply returns the size
 *       of the internal values vector.
 */
std::size_t CSRMatrix::nnz() const noexcept
{
    return values_.size();
}