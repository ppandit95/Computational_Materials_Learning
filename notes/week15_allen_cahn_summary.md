
# Week 15: Allen-Cahn Phase-Field Solver

## Governing equation

- Free-energy functional:
  \[
  F[\eta]=\int_0^L\left[\frac{W}{4}(\eta^2-1)^2+
  \frac{\kappa}{2}(\partial_x\eta)^2\right]dx.
  \]
- Variational derivative:
  \[
  \frac{\delta F}{\delta\eta}=W(\eta^3-\eta)-\kappa\partial_{xx}\eta,
  \qquad
  \partial_t\eta=-M\frac{\delta F}{\delta\eta}
  =M\left[\kappa\partial_{xx}\eta-W(\eta^3-\eta)\right].
  \]
- Physical assumptions: an isothermal, one-dimensional, nonconserved order
  parameter evolves by local gradient descent; `M`, `\kappa`, and `W` are
  spatially and temporally constant.  The double-well bulk energy has stable
  phases at \(\eta=\pm1\), while the gradient term gives interfaces a finite
  width.

## Numerical implementation

- Spatial discretization: uniform, one-dimensional cell-centred grid with the
  three-point second-difference Laplacian.  In the interior,
  \((\eta_{i+1}-2\eta_i+\eta_{i-1})/\Delta x^2\); the discrete energy is
  \(\Delta x\sum_i W(\eta_i^2-1)^2/4 + \kappa\sum_i(\eta_{i+1}-\eta_i)^2/(2\Delta x)\).
- Boundary conditions: homogeneous Neumann (zero flux), implemented by
  repeated-edge padding in Python and one-sided boundary stencils in C++.
  Hence the endpoint Laplacians are \((\eta_1-\eta_0)/\Delta x^2\) and
  \((\eta_{N-2}-\eta_{N-1})/\Delta x^2\), respectively.
- Temporal discretization: explicit Forward Euler,
  \(\eta^{n+1}=\eta^n+\Delta t\,\mathrm{RHS}(\eta^n)\).
- Linear stability restriction: the implemented conservative bound is
  \[
  \Delta t_{\mathrm{crit}}=
  \frac{2}{M\left(4\kappa/\Delta x^2+2W\right)},
  \]
  from linearization about \(\eta=\pm1\).  It does not guarantee nonlinear
  energy stability.

## Scientific verification

- Python tests passed: no `pytest` executable is installed in this environment,
  so the Python files were not run as a pytest suite.  The directly executed
  optimized-versus-original single-step assertions passed for \(N=100,200,400\)
  with maximum absolute difference 0.0 and tolerances `rtol=1e-12`,
  `atol=1e-14`.  The source test coverage includes the bulk-energy derivative,
  constant-field Laplacian, boundary stencil, stationary phases, input
  validation, energy dissipation, refinement residuals, and interfacial energy.
- C++ tests passed: `./build/AC_1d_test` was rebuilt and run locally on
  20 November 2026; 15/15 doctest cases and 290/290 assertions passed.  These
  cover the Neumann stencil, stationary phases, energy history, parameter
  validation, mobility scaling, a discrete-energy reference value, minimum
  grid size, interface-refinement residuals, and interfacial energy.
- Energy dissipation: the C++ test evolved the \(N=200\), \(L=10\) tanh
  initial condition for 100 Forward Euler steps with \(M=W=1\),
  \(\kappa=0.1\), \(\Delta x=0.05\), and \(\Delta t=0.005\).  It passed the
  check \(F^{n+1}-F^n\leq10^{-10}\) at every step and verified that final
  energy is below initial energy.  This is a discrete test at that timestep,
  not an unconditional property of explicit Euler.
- Numerical equivalence: the optimized Python step is bitwise identical to
  the original for the directly checked one-step cases at \(N=100,200,400\)
  (maximum absolute difference 0.0).  The existing nominal long-time test
  also returns 0.0, but it repeatedly updates neither field; it is therefore
  evidence only for repeatable one-step equality, not trajectory equivalence.

## Performance

- Profiling bottleneck: local `cProfile` of 2,000 original steps at \(N=200\)
  took 0.165 s.  The RHS path consumed 0.137 s cumulatively, the Neumann
  Laplacian 0.094 s, and `numpy.pad` 0.064 s.  Repeated field validation was
  material but not the largest cost: three `_validate_field` calls consumed
  0.044 s cumulatively.
- Original runtime: median time for 1,000 steps was 0.067065 s (`N=200`),
  0.105774 s (`N=1,000`), and 0.233521 s (`N=5,000`).
- Optimized runtime: 0.055561 s (`N=200`), 0.086095 s (`N=1,000`), and
  0.231907 s (`N=5,000`) for the same benchmark.
- Measured speedup: 1.207x, 1.229x, and 1.007x at `N=200`, `1,000`, and
  `5,000`, respectively.  The optimization reduces field validations per
  Euler step from three to one (and full-field finite scans from three to one);
  it does not eliminate scalar validation.  This operation-count reduction is
  distinct from the measured end-to-end speedup.
- Amdahl's law prediction: treating two-thirds of the profiler's 0.044 s
  validation time as removable gives \(f=(2/3)(0.044/0.165)\approx0.178\) and
  \(S_{\max}=1/(1-f)\approx1.22\times\).  This is an approximate upper bound
  for that profiled workload, consistent with the 1.207x result at `N=200`;
  it does not predict the large-grid result, where array arithmetic and
  allocation dominate.

## Limitations

- Explicit timestep restriction.
- One-dimensional implementation.
- Constant material coefficients.
- Nonconserved order parameter.

## Future extensions

- Cahn-Hilliard dynamics.
- Implicit time integration.
- MPI domain decomposition.
- GPU acceleration.
