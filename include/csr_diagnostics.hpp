#pragma once
#include "csr_matrix.hpp"
#include <cstddef>
struct CSRMatvecWork
{
    std::size_t nonzeros{};
    std::size_t multiplications{};
    std::size_t additions{};

    [[nodiscard]] std::size_t flops() const noexcept
    {
        return multiplications + additions;
    }
};

/**
 * @brief Estimate arithmetic work for one CSR matrix-vector product.
 *
 * Counts one multiplication and one addition per stored matrix entry.
 *
 * @param matrix CSR matrix being analysed.
 * @return Arithmetic-work counts for one matvec.
 */

[[nodiscard]] CSRMatvecWork estimate_matvec_work(const CSRMatrix &matrix) noexcept;