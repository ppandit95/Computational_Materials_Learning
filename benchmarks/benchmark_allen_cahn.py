import csv
import timeit
from statistics import median

import numpy as np

from python.allen_cahn_solver import forward_euler_step as original_step

from python.allen_cahn_solver_optimized import forward_euler_step as optimized_step

from python.allen_cahn_stability import critical_timestep


def run_simulation(step_function, eta_initial, dx, dt, nsteps):
    eta = eta_initial.copy()
    for _ in range(nsteps):
        eta = step_function(eta, dx, dt, 1.0, 0.1, 1.0)
    return eta


if __name__ == "__main__":

    results = []

    for N in [200, 1000, 5000]:
        dx = 10.0 / N
        dt = 0.5 * critical_timestep(dx, 1.0, 0.1, 1.0)
        x = (np.arange(N) + 0.5) * dx
        eta_initial = 0.8 * np.tanh((x - 5.0) / 0.8)
        original_run = lambda: run_simulation(original_step, eta_initial, dx, dt, 1000)
        optimized_run = lambda: run_simulation(
            optimized_step, eta_initial, dx, dt, 1000
        )

        original_run()
        optimized_run()

        original_time = median(timeit.repeat(original_run, repeat=5, number=1))
        optimized_time = median(timeit.repeat(optimized_run, repeat=5, number=1))

        speed_up = original_time / optimized_time

        results.append([N, original_time, optimized_time, speed_up])

        print(
            f"N={N:5d} | "
            f"Original={original_time:.6f}s | "
            f"Optimized={optimized_time:.6f}s | "
            f"Speedup={speed_up:.3f} times"
        )

    with open("data/allen_cahn_performance.csv", "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["N", "Original_s", "Optimized_s", "Speedup"])
        writer.writerows(results)
