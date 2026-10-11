# Jacobi Convergence Characteristics

| Scenario | Expected Iterations | Converged? | Final Residual Norm |
|---|---:|---|---:|
| Initial guess zero, tolerance \(10^{-6}\) | 40 | Yes | Calculate |
| Initial guess zero, maximum 2 iterations | 2 | No | 1.0 |
| Initial guess equals exact solution | 0 | Yes | 0 |

## Convergence Requirements

A solver needs both convergence tolerance and a maximum iteration limit. If the coefficient matrix is ill-conditioned, the iterative solver might get stuck and fail to converge.

## Residual vs. Error

We typically expect the residual norm \(\|\mathbf{r}_k\|_2\) to be small, but this doesn't guarantee that the error \(\|\mathbf{x}_k - \mathbf{x}^*\|_2\) is small, where \(\mathbf{x}^*\) is the exact solution. Since \(\mathbf{x}^*\) is unknown a priori, the residual norm is used as a proxy and is expected to correlate with error reduction.