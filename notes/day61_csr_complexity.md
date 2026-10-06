In CSR matvec, each stored entry performs one multiplication and one addition, so approximately `N_FLOPS = 2 * nnz` operations are performed for CSR matrix-vector multiplication, while a dense matrix requires `n²` operations. Extrapolating to larger values of `n`:

| n | nnz | Approximate FLOPs (CSR) | FLOPs (Dense) |
|---:|---:|---:|---:|
| 10 | 50 | 100 | 100 |
| 100 | 500 | 1,000 | 10,000 |
| 1,000 | 5,000 | 10,000 | 1,000,000 |
| 10,000 | 50,000 | 100,000 | 100,000,000 |

For `n = 10⁶`, the difference between CSR and dense matrix-vector multiplication is dramatic, which demonstrates why CSR matrix representation is essential for solving PDE systems efficiently.

## Memory Access Overhead

For every stored CSR entry, `matvec()` requires `values_[j]`, `col_indices_[j]`, and `x[col_indices_[j]]`, which eventually updates `result[i]`. This necessitates moving a relatively large amount of data to perform calculations. Although FLOPs are relatively fewer than dense matrix operations, the memory bandwidth requirement can limit matvec performance due to low arithmetic intensity.

**Arithmetic Intensity** = FLOPs / Bytes Moved