from time import perf_counter
from statistics import median
import numpy as np
from heat_1d import solve_heat_ftcs
import pandas as pd
import os

n_interior_vals = [31, 63, 127, 255]
alpha = 0.1
r = 0.4
tf = 0.1

# 1. Collect data into a list of dictionaries first (much faster & cleaner than empty DF)
results = []

for n_interior in n_interior_vals:
    dx = 1.0 / (n_interior + 1)
    dt = r * dx**2 / alpha
    n_steps = int(np.ceil(tf / dt))
    timings = []

    for _ in range(5):
        start = perf_counter()
        x, u, t_final = solve_heat_ftcs(
            n_interior,
            alpha,
            dt,
            n_steps,
        )
        elapsed = perf_counter() - start
        timings.append(elapsed)

    # Note: Variable name is runtime_seconds but you multiplied by 1000 (ms)
    # Keeping it as ms to match your print statement below
    runtime_ms = float(median(timings)) * 1000
    u_exact = np.exp(-alpha * t_final * np.pi**2) * np.sin(np.pi * x)
    error = np.linalg.norm(u - u_exact, np.inf)

    results.append(
        {
            "n_interior": n_interior,
            "dx": dx,
            "n_steps": n_steps,
            "dt": dt,
            "r_actual": alpha * dt / dx**2,
            "runtime_seconds": runtime_ms,  # keeping tracking units matching your print
            "error_linf": error,
        }
    )

# 2. Create DataFrame all at once
df = pd.DataFrame(results)

# 3. Ensure the target directory exists before writing
csv_path = "/home/pushkar/computational_materials_learning/data/heat_1d_benchmark.csv"
os.makedirs(os.path.dirname(csv_path), exist_ok=True)

# 4. Save to CSV
df.to_csv(csv_path, index=False)
print(f"Success! CSV file written to: {csv_path}\n")

# 5. Math Post-Processing
runtimes = df["runtime_seconds"].to_numpy(dtype=float)
R_T = runtimes[1:] / runtimes[:-1]

# For algorithmic scaling scaling: total operations proportional to (n_interior * n_steps)
# Grid ratio should map to total work steps to accurately find 'q'
total_work = df["n_interior"].to_numpy(dtype=float) * df["n_steps"].to_numpy(
    dtype=float
)
W_T = total_work[1:] / total_work[:-1]

q = np.log2(R_T) / np.log2(W_T)
print(f"Runtimes (ms): {runtimes}")
print(f"R_T (Runtime ratio): {R_T}")
print(
    f"q (Algorithmic scaling exponent, should approach ~1.0 relative to total work): {q}"
)

error_val = df["error_linf"].to_numpy(dtype=float)
R_E = error_val[1:] / error_val[:-1]
print(f"R_E (Error reduction factor, should approach ~4.0 for dx halving): {1.0/R_E}")
