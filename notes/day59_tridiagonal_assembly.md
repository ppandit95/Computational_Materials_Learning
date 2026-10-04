## Tridiagonal Matrix CSR Representation

### Example: 5×5 Matrix

Given a tridiagonal matrix with:
- **Lower diagonal**: -2
- **Main diagonal**: 5
- **Upper diagonal**: -3

The matrix  T is:
```
[[5, -3,  0,  0,  0],
 [-2,  5, -3,  0,  0],
 [0, -2,  5, -3,  0],
 [0,  0, -2,  5, -3],
 [0,  0,  0, -2,  5]]
```

**Non-zero entries**: `nnz = 3n - 2 = 13`

### CSR (Compressed Sparse Row) Representation

```
values    = [5, -3, -2, 5, -3, -2, 5, -3, -2, 5, -3, -2, 5]
col_indices = [0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4]
row_ptr    = [0, 2, 5, 8, 11, 13]
```

```
For a vector x = {1, 1, 1, 1, 1}, the product Tx would be:
```
Tx = [2, 0, 0, 0, 3]
```
```
