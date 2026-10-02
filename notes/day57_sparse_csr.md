
# Sparse Matrix Representations in CSR Format

## Overview
Compressed Sparse Row (CSR) format is an efficient way to store sparse matrices.

## Key Concepts
- **Storage efficiency**: Only non-zero elements are stored
- **CSR components**: 
    - `values`: Non-zero values
    - `col_indices`: Column indices
    - `row_ptr`: Row pointers

## Example
For a matrix A = [[2,-1,0],[-1,2,-1],[0,-1,2]]
Its CSR form should be -
values = [2,-1,-1,2,-1,-1,2]
col_indices = [0,1,0,1,2,1,2]
row_ptr = [0,2,5,7]

Since row_ptr[2] = 5 and row_ptr[3] = 7 that means row 2 in A matrix occupies entry number 5 and 6 in values at column index of entry 5 and 6 

For a N size sparse matrix, row_ptr.size() = N+1 and row_ptr[N] = number_of_non-zero elements = 3N-2

## Implementation Notes
- CSR format is particularly useful for iterating through rows efficiently
- Memory usage is O(nnz + n + 1) where nnz is the number of non-zero elements
- Conversion to/from dense format is a common operation in sparse matrix libraries

## Common Operations
- **Matrix-vector multiplication**: Efficient due to row-wise storage
- **Row slicing**: Direct access via row_ptr indices
- **Column access**: Less efficient, may require sorting by column

## References
- Widely used in scipy.sparse, Eigen, and other numerical libraries
- Variants include CSC (Compressed Sparse Column) for column-optimized access


The values are correct. For an N-point 1D tridiagonal matrix:

- **Dense entries**: N² (N×N matrix)
- **CSR nonzero values**: 3N-2 (diagonal + two off-diagonals, minus corner elements)
- **row_ptr entries**: N+1 (stores N row starts plus final sentinel value)

This aligns with your matrix example where N=3 gives 7 nonzero elements (3×3-2=7).


## Sparsity Ratio and PDE Simulations

As the matrix size increases, sparsity becomes increasingly significant. The ratio of non-zero elements to total elements is (3N-2)/N², which approaches 0 as N grows. This dramatic reduction in data storage is crucial for PDE simulations, where dense N² matrices become computationally infeasible for large-scale problems.

**Why this matters:** As problem size N grows, the fraction of non-zero elements shrinks dramatically, making sparse storage essential for large-scale PDE simulations where storing a dense N² matrix becomes infeasible, enabling efficient solvers like those in MFEM, HYPRE, and PETSc.




