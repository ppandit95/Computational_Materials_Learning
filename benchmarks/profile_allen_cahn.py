import cProfile
import pstats
import numpy as np
from python.allen_cahn_solver import forward_euler_step


def run_simulation():
    N = 200
    L = 10.0
    dx = L / N
    x = (np.arange(N) + 0.5) * dx
    eta = 0.8 * np.tanh((x - L / 2) / 0.8)

    for _ in range(2000):
        eta = forward_euler_step(eta, dx, 0.005, 1.0, 0.1, 1.0)
    return eta


if __name__ == "__main__":
    profiler = cProfile.Profile()
    profiler.enable()
    run_simulation()
    profiler.disable()
    stats = pstats.Stats(profiler)
    stats.sort_stats("cumulative").print_stats(15)
