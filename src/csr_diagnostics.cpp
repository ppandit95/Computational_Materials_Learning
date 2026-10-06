#include <iostream>
#include "csr_matrix.hpp"
#include "csr_diagnostics.hpp"
#include <vector>

CSRMatvecWork estimate_matvec_work(const CSRMatrix &matrix) noexcept
{
    CSRMatvecWork result;
    result.nonzeros = matrix.nnz();
    result.multiplications = result.nonzeros;
    result.additions = result.nonzeros;
    return result;
}