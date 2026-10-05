## Why would a CSRMatrix representation be invalid?

Considering the matrix A = [[2,-1],[-1,2]] with CSR representation:

**values** = [2,-1,-1,2]
**col_indices** = [0,1,0,1]
**row_ptr** = [0,2,4]

- If **row_ptr** = [0,2], then |row_ptr| ≠ n+1 and should be rejected.

- If **values** = [2,-1] and **col_indices** = [0], then the size of values and col_indices arrays are unequal, and size(values) and size(col_indices) ≠ nnz.

- If **row_ptr** = [1,2,4], then row_ptr[0] ≠ 0, and the representation is invalid.

- If **row_ptr** = [0,3,2], then row_ptr[2] < row_ptr[1], which is invalid.

- If **row_ptr** = [0,2,5] and nnz = 4, then row_ptr[2] ≠ nnz, which is invalid.

- If **col_indices** = [0,2,0,1] and cols = 2, then col_indices is invalid as col_indices[2] = 2 is not a valid column index.
