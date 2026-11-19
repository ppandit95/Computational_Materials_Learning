# Day 104: Allen--Cahn validation-path optimization

**Week 15, Day 6 -- 18 November 2026**

## Aim

Measure the effect of avoiding repeated input validation inside one explicit
Allen--Cahn Euler update, while retaining validation at the public
`forward_euler_step` interface.

## Change identified

`python/allen_cahn_solver_optimized.py` adds unchecked internal helpers for
the Neumann Laplacian and Allen--Cahn RHS.  `forward_euler_step` validates its
field and scalar inputs once, then calls the unchecked RHS.  The public
`laplacian_neumann` and `allen_cahn_rhs` functions remain validating entry
points.

For one call to `forward_euler_step`, the call paths are:

| Item | Original | Optimized |
| --- | ---: | ---: |
| `_validate_field(eta, dx)` calls | 3 | 1 |
| Full-field finite-value scans | 3 | 1 |
| Positive-scalar validations (`dt`, `M`, `kappa`, `W`) | 4 | 4 |

Thus the specific reduction is **two field validations (including two
full-array `isfinite` scans) per Euler step**.  It is not a removal of all
validation: the optimized public step still rejects invalid fields and scalar
parameters before calculation.

## Recorded runtime

The checked-in benchmark data in `data/allen_cahn_performance.csv` was
produced by `benchmarks/benchmark_allen_cahn.py`: 1,000 Euler steps on a
length-10 domain, `M=1`, `kappa=0.1`, `W=1`, and a timestep equal to one half
of the computed critical timestep.  Each number is the median of five
one-run `timeit` measurements after one warm-up run.

| Cells `N` | Original (s) | Optimized (s) | Ratio, original / optimized | Wall-clock reduction |
| ---: | ---: | ---: | ---: | ---: |
| 200 | 0.067065 | 0.055561 | 1.207x | 17.2% |
| 1,000 | 0.105774 | 0.086095 | 1.229x | 18.6% |
| 5,000 | 0.233521 | 0.231907 | 1.007x | 0.7% |

The drop from three to one field-validation calls is a fixed operation-count
change, whereas the table is the measured end-to-end timing result.  They
should not be conflated: at `N=5,000` the same validation reduction produced
only a 0.7% measured improvement, because array arithmetic and allocation
dominate the runtime.  On the two smaller grids it corresponded to about a
1.21--1.23x speedup in this benchmark environment.

## Numerical-equivalence results

The assertions in `tests/test_allen_cahn_optimized.py` were executed directly
with Python 3 because `pytest` is not installed in this environment.  The
single-step cases all passed with the test tolerances
`rtol=1e-12`, `atol=1e-14`:

| Test case | Result | Maximum absolute difference |
| --- | --- | ---: |
| Single step, `N=100` | pass | 0.0 |
| Single step, `N=200` | pass | 0.0 |
| Single step, `N=400` | pass | 0.0 |
| Existing nominal “long-time” case | pass | 0.0 |

The optimized and original expressions use the same NumPy operations, so
these tested updates were bitwise identical.  The test file also specifies
input-immutability and invalid-input checks for the optimized public step;
those tests were not run under pytest here because the test runner is absent.

## Limitations and follow-up

- The reported times are hardware- and load-dependent medians, not a general
  performance guarantee.  They also cover only this explicit, one-dimensional
  NumPy solver and a 1,000-step workload.
- The nominal long-time test does **not** evolve either solution: its loop
  repeatedly computes both one-step updates from the unchanged initial
  `eta`.  It therefore confirms repeatable single-step equality, not
  999-step trajectory equivalence.  A valid long-time test should advance two
  separate fields on every iteration and compare them after the final step.
- The optimization preserves public-step validation but exposes unchecked
  helpers as module-level private functions.  They must remain internal;
  calling them directly bypasses input checks.
- The speedup decreases sharply at the largest tested grid.  Further
  optimization should be guided by profiling rather than assuming that more
  validation removal will yield a meaningful end-to-end gain.
