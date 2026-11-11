import numpy as np
import pandas as pd

from pathlib import Path
from time import perf_counter

from python.nonlinear_diffusion_solver import solve_nonlinear_diffusion

L = 1.0e-3
D0 = 1.0e-10
beta = 1.0
t_f = 100.0

r_target = 0.4

grid_sizes = [32, 64, 128, 256]
repetitions = 5

results = []
solutions = {}

for N in grid_sizes:

    timings = []

    dx = L / N

    # Conservative stability limit
    dt_max = dx**2 / (2 * D0 * (1 + beta))

    dt_target = 0.8 * dt_max

    n_steps = int(np.ceil(t_f / dt_target))

    dt = t_f / n_steps

    # Cell-centred initial condition
    j = np.arange(N)

    cell_centres = (j + 0.5) * dx

    c_initial = 0.5 + 0.2 * np.cos(2 * np.pi * cell_centres / L)

    # Warm-up run
    c_warmup = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_steps)

    # Timed runs
    for _ in range(repetitions):

        start = perf_counter()

        c_final = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_steps)

        elapsed = perf_counter() - start

        timings.append(elapsed)

        np.testing.assert_allclose(
            c_final,
            c_warmup,
            rtol=1e-12,
            atol=1e-14,
        )

    runtime_median_s = float(np.median(timings))

    initial_mass = dx * np.sum(c_initial)

    final_mass = dx * np.sum(c_final)

    mass_relative_error = abs(final_mass - initial_mass) / max(abs(initial_mass), 1e-30)

    solutions[N] = c_final.copy()

    results.append(
        {
            "N": N,
            "dx_m": dx,
            "dt_s": dt,
            "n_steps": n_steps,
            "runtime_median_s": runtime_median_s,
            "mass_relative_error": mass_relative_error,
        }
    )

    print(
        f"N={N:4d} | "
        f"steps={n_steps:6d} | "
        f"runtime={runtime_median_s:.6f} s | "
        f"mass error={mass_relative_error:.3e}"
    )


df = pd.DataFrame(results)

# Runtime scaling ratios
runtimes = df["runtime_median_s"].to_numpy()

runtime_ratios = np.concatenate(([np.nan], runtimes[1:] / runtimes[:-1]))

df["runtime_ratio"] = runtime_ratios


# Fine-grid comparison
N_reference = max(grid_sizes)

c_reference = solutions[N_reference]

reference_errors = []

for N in grid_sizes:

    if N == N_reference:

        reference_errors.append(np.nan)

        continue

    refinement_ratio = N_reference // N

    c_reference_coarse = c_reference.reshape(N, refinement_ratio).mean(axis=1)

    error = np.max(np.abs(solutions[N] - c_reference_coarse))

    reference_errors.append(error)


df["reference_Linf_error"] = reference_errors


# Save results
output_dir = Path("data")
output_dir.mkdir(parents=True, exist_ok=True)

csv_path = output_dir / "day97_nonlinear_benchmark.csv"

df.to_csv(csv_path, index=False)

print("\nBenchmark summary:")
print(df.to_string(index=False))

print(f"\nCSV written to: {csv_path}")
