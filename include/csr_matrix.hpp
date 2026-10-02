#pragma once

#include <cstddef>
#include <vector>
class CSRMatrix
{
public:
    CSRMatrix(std::size_t rows, std::size_t cols, std::vector<double> values, std::vector<std::size_t> col_indices, std::vector<std::size_t> row_ptr);
    [[nodiscard]] std::vector<double> matvec(const std::vector<double> &x) const;
    [[nodiscard]] std::size_t rows() const noexcept;
    [[nodiscard]] std::size_t cols() const noexcept;
    [[nodiscard]] std::size_t nnz() const noexcept;

private:
    std::size_t rows_;
    std::size_t cols_;

    std::vector<double> values_;
    std::vector<std::size_t> col_indices_;
    std::vector<std::size_t> row_ptr_;
};
