## Why CSR Storage Changes Computational Representation Without Changing the Mathematical Operator

Storing matrices in CSR (Compressed Sparse Row) format optimizes computation during matrix-vector multiplication. Rather than multiplying every entry in a sparse matrix (which contains many zeros) by vector elements, CSR allows multiplication of only non-zero entries. This reduces computational cost while preserving the mathematical result of the matrix-vector multiplication operator.

### Day 63 Exercise Results

| Matrix Dimension | Nonzeros | Mean Matvec Time |
| ---: | ---: | ---: |
| 1,000 | 2,998 | 11.20 µs |
| 10,000 | 29,998 | 87.75 µs |
| 100,000 | 299,998 | 920.47 µs |
| 1,000,000 | 2,999,998 | 10.15 ms |

**Scaling factor:** p ≈ 0.9893

### Performance Limitations

Despite exhibiting linear algorithmic complexity due to CSR representation in matrix-vector operations, CSR representation is limited by memory bandwidth. Significant data movement is required compared to the actual matrix-vector product computation.