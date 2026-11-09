# 1D Heat Equation: Verification Summary

## Governing Equation

The solver models diffusion on the unit interval, \(0 \le x \le 1\), with

\[
\frac{\partial u}{\partial t} = \alpha \frac{\partial^2 u}{\partial x^2},
\]

where \(\alpha > 0\) is thermal diffusivity in m\(^2\)/s. Spatial coordinates
are nondimensionalized to the unit interval, and time is measured in seconds.

## FTCS Discretization

For \(\Delta x = 1/(N+1)\) and \(r=\alpha\Delta t/\Delta x^2\), the explicit
forward-time, centered-space update at an interior node is

\[
u_i^{n+1}=u_i^n+r\left(u_{i+1}^n-2u_i^n+u_{i-1}^n\right).
\]

`solve_heat_ftcs` applies this update to the interior nodes and retains the
endpoint values.

## Stability Restriction

The explicit FTCS method requires

\[
r=\frac{\alpha\Delta t}{\Delta x^2}\leq\frac{1}{2}.
\]

The solver raises `ValueError` when `r > 0.5`; the equality case is accepted.

## Initial and Boundary Conditions

The reference initial condition is

\[
u(x,0)=\sin(\pi x),
\]

with homogeneous Dirichlet boundaries \(u(0,t)=u(1,t)=0\). A vectorized
initial-condition callable may be passed to the solver; the repository also
provides `two_mode_initial_condition(x) = sin(pi*x) + 0.25*sin(3*pi*x)`.
The callable must return one value per grid node. Its endpoint values are
preserved by the scheme; use zero endpoints for the reference boundary
conditions.

## Regression Tests

The heat-focused test inventory contains 20 collected cases: 19 from
`tests/test_heat_1d.py` (including the ten-case maximum-principle
parameterization) and one convergence case from `tests/test_heat_convergence.py`.

An actual current pytest terminal summary is not persisted in the repository.
The only retained pytest failure-cache entry names
`test_Invalid_initial_conditions`, which is absent from the current test file,
so it is stale and must not be reported as a current result. A fresh pytest
invocation is required to state an actual pass/fail result; it was not rerun
for this documentation update.

## Analytical Verification

For the reference initial and boundary conditions, the analytical solution is

\[
u(x,t)=e^{-\alpha\pi^2t}\sin(\pi x).
\]

The solver and convergence test compare the numerical solution against this
expression in the infinity norm.

## Convergence Results

Day 89 defines a refinement study at \(N=15,31,63,127\), with
\(\alpha=0.1\), \(r=0.4\), and \(t_f=0.1\), and computes consecutive
logarithmic orders from infinity-norm errors. The notebook does not persist
the resulting errors or measured orders, so no numerical convergence orders
are reported here.

## Performance Results

Day 90 results are available in `data/heat_1d_benchmark.csv`. The recorded
median runtimes are:

| Interior points | Steps | Recorded runtime (ms) | \(\lVert e\rVert_\infty\) |
| ---: | ---: | ---: | ---: |
| 31 | 26 | 0.113582 | 1.021505e-4 |
| 63 | 103 | 0.398288 | 2.528178e-5 |
| 127 | 410 | 1.692243 | 6.290640e-6 |
| 255 | 1639 | 7.298663 | 1.571661e-6 |

Although the CSV field is named `runtime_seconds`, the Day 90 producer
multiplies the median elapsed time by 1000 before writing it; the values above
are therefore milliseconds.

## Known Limitations

FTCS is explicit, so spatial refinement makes its stability bound
\(\Delta t\le\Delta x^2/(2\alpha)\) increasingly restrictive. Halving
\(\Delta x\) requires roughly a fourfold smaller stable time step; over a
fixed physical time this increases the number of steps by about four.
