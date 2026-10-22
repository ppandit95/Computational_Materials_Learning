# Day 77: Week 11 Consolidation

## Scope and progress

Week 11 moved from a single Forward Euler update to a small, verified workflow:

- **Day 71:** implemented unforced decay and explored stable, marginal, and unstable timesteps.
- **Day 72:** added the analytical solution, final-time error calculation, and first-order accuracy tests.
- **Day 73:** extended the method to constant forcing and added its analytical solution.
- **Day 74:** tested equilibrium preservation and stability edge cases for the forced problem.
- **Day 75:** added error, convergence-order, and profiling diagnostics.
- **Day 76:** benchmarked accuracy against computational cost and saved the results for analysis.
- **Day 77:** added focused regression tests for equilibrium preservation and propagation of the difference between two trajectories.

## 1. Numerical models reviewed

### Unforced decay

The model

$$
\frac{du}{dt}=-\lambda u, \qquad \lambda>0
$$

has the Forward Euler update

$$
u^{n+1}=(1-\lambda\Delta t)u^n
$$

and analytical solution

$$
u(t)=u_0e^{-\lambda t}.
$$

### Forced decay

For

$$
\frac{du}{dt}=-\lambda u+s,
$$

the numerical update is

$$
u^{n+1}=(1-\lambda\Delta t)u^n+s\Delta t,
$$

the equilibrium is $u_{\mathrm{eq}}=s/\lambda$, and the analytical solution is

$$
u(t)=u_{\mathrm{eq}}+(u_0-u_{\mathrm{eq}})e^{-\lambda t}.
$$

Writing the perturbation as $v^n=u^n-u_{\mathrm{eq}}$ reduces the forced update to

$$
v^{n+1}=(1-\lambda\Delta t)v^n.
$$

Therefore the forcing changes the equilibrium but not the stability condition.

## 2. Stability

- **Amplification factor:** $G=1-\lambda\Delta t$.
- **Asymptotic stability:** $|G|<1$, or $0<\lambda\Delta t<2$.
- **Monotone decay:** $0<\lambda\Delta t<1$.
- **Oscillatory decay:** $1<\lambda\Delta t<2$.
- **Marginal boundary:** $\lambda\Delta t=2$ gives $G=-1$, so the perturbation alternates without decaying.
- **Instability:** $\lambda\Delta t>2$ gives growing oscillations.

Physically, a timestep can be too large to resolve a decaying process even when the differential equation itself is stable. In the forced problem, stability means convergence toward $s/\lambda$, not necessarily toward zero.

## 3. Accuracy

- **Expected convergence order:** Forward Euler has first-order global accuracy, $E(\Delta t)=O(\Delta t)$.
- **Measured forced-problem errors** for $u_0=1$, $\lambda=2$, $s=1$, and $T=1$:

| $\Delta t$ | Steps | Absolute final-time error | Observed order |
|---:|---:|---:|---:|
| 0.200 | 5 | $2.878764\times10^{-2}$ | -- |
| 0.100 | 10 | $1.398055\times10^{-2}$ | 1.0420 |
| 0.050 | 20 | $6.87931\times10^{-3}$ | 1.0231 |
| 0.025 | 40 | $3.41156\times10^{-3}$ | 1.0118 |

The observed order approaches one under refinement, matching the theoretical result.

## 4. Computational cost

- **Expected runtime scaling:** $O(N)$ for $N$ timesteps because each step performs constant work.
- **Measured runtime scaling:** the Day 76 data for 2,000--32,000 steps gives a log-log fitted exponent of **1.006**. Pairwise exponents range from **0.970 to 1.039**.
- **Accuracy/cost relation:** doubling $N$ approximately doubles runtime and halves the final-time error. The fitted error exponent with respect to $N$ is **-1.000**.

Measurement uncertainty remains because the recorded runs are short (about 0.6--9 ms), use one machine and Python interpreter, allocate a full solution array on every call, and report only the median of seven timings. Background load, interpreter overhead, and timer resolution can influence these results. Repeating batches and reporting dispersion would make the timing claim stronger.

## 5. Scientific regression tests

- **Equilibrium preservation:** when $u_0=s/\lambda$, every update remains at that equilibrium. This is covered in both `test_euler_forced.py` and the Day 77 regression file.
- **Difference propagation:** for two trajectories with the same $\lambda$, $s$, and $\Delta t$,

  $$
  u_a^n-u_b^n=(1-\lambda\Delta t)^n(u_{0,a}-u_{0,b}).
  $$

  The new regression test verifies this discrete identity across all saved steps.
- **Other coverage:** known updates, analytical solutions, zero forcing, zero steps, invalid inputs, stable decay, the marginal boundary, unstable growth, decreasing error, first-order convergence, and benchmark error trends.
- **Week 11 result:** all **65** Euler-focused tests pass.

## 6. Verification results

Commands were run from the repository root on 2026-10-22.

- `python -m pytest -q`: **72 passed** in 4.64 s.
- Euler-focused Python suite: **65 passed** in 0.79 s.
- `pytest -q`: collection fails because the console entry point cannot resolve the local `python` package, while `python -m pytest` can.
- `ctest --test-dir build --output-on-failure`: **1/9 passed** in this environment because LeakSanitizer cannot run under the active tracing/sandbox setup.
- `ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build --output-on-failure`: **7/9 passed**. The remaining failures are `unit_test_3` (five Arrhenius expectations) and `numerical_foundations_doctest` (one large-magnitude `nearly_equal` expectation). These failures are outside the Week 11 Euler work.

## 7. Review findings and next actions

1. Make invocation-independent imports part of the project configuration so both `pytest` and `python -m pytest` collect the suite consistently.
2. Import `forward_euler_forced` directly from `python.euler_forced` in the Day 77 regression test; importing it indirectly from another test module unnecessarily couples test files. Remove the unused `exact_forced_decay` import.
3. Keep one equilibrium regression or give the two tests distinct purposes. The older test already checks equilibrium to near machine precision, whereas the new $10^{-3}$ tolerance is too loose for an identity that should be exact here.
4. Strengthen the benchmark test to check the measured runtime exponent and report timing spread. It currently verifies only the error trend, not the claimed computational scaling.
5. Correct the Day 76 scaling plot labels: the plotted coordinates are timestep count on the x-axis and compute time on the y-axis, but the current labels describe them in reverse.
6. Harmonize validation between the unforced and forced solvers. The forced solver rejects nonfinite inputs, booleans, and noninteger step counts more consistently than the unforced solver.
7. Investigate the two unrelated C++ test failures and configure sanitizer behavior appropriately for the test environment.

## Conclusion

The week established a coherent chain from model derivation to implementation, stability analysis, accuracy verification, and cost measurement. The scientific conclusions are supported by the saved data and Python tests: Forward Euler is stable for $0<\lambda\Delta t<2$, first-order accurate in time, and linear in the number of steps. The next improvement should be test and benchmark infrastructure rather than adding more model features.
