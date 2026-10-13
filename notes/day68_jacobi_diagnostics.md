| Property | Matrix \(A_1\) | Matrix \(A_2\) |
|---|---|---|
| Matrix size | \(3\times3\) | \(2\times2\) |
| Strictly diagonally dominant? | No | No |
| Spectral radius | 0.707107 | 2 |
| Observed residual behavior | Decreases | Increases |
| Expected Jacobi behavior | Convergent | Divergent for generic initial errors |
| Solver implementation defective? | No | No |

When rho(B) < 1, it guarantees convergence as the ratio of residual specifically for A1 keeps on decreasing with  iterations.

As can be seen for A2, which is a valid linear system as it is invertible but upon jacobi iteration the residual keeps on increasing which suggests the jacobi solution diverges from the final solution so not every invertible matrix can converge with jacobi iteration from arbitraty approximate.

If the final iterate is calculated for max_iteration number then it is a safe practice to report that solution is not converged rather that accepting final solution because the corresponding residual norm isnt satisfying convergence criteria . In context of phase field computations, it is very necessary to converge linear solve as an error in this can contaminate the accuracy of entire time step.
