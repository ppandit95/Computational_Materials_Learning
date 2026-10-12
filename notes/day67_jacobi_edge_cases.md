| Test category | Expected behavior | Actual result |
|---|---|---|
| Zero tolerance | Reject | Reject |
| Negative tolerance | Reject | Reject |
| Infinite tolerance | Reject | Reject |
| NaN tolerance | Reject | Reject |
| Zero diagonal | Reject | Reject |
| Wrong vector dimensions | Reject | Reject |
| Zero RHS and zero initial guess | Converge in 0 iterations | Converged at iteration=0 |
| Maximum iterations reached | Return `converged=false` | Return `converged=false` |
| Returned residual consistency | Match independent calculation | Match independently calculated norm |