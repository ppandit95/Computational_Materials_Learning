# Benchmarking CSR Matrix-Vector Multiplication

For different matrix sizes with n = 1,000, 10,000, 100,000, 1,000,000, benchmark using the following procedure:

## For each n:

1. Construct the tridiagonal matrix once
2. Construct vector x once
3. Perform a few warm-up matrix-vector products
4. Time many repeated matrix-vector products
5. Divide total elapsed time by repetitions
6. Record n, nnz, and mean time

**Note:** A single matrix-vector product is not benchmarked individually because it may not take appreciable time to record, and measurement overhead might be comparatively large for small matrices.