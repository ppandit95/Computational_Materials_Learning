import pytest
import numpy as np
from python.heat_1d import solve_heat_ftcs, two_mode_initial_condition


def test_heat_convergence():
    # 1. Define configurations to run sequentially
    n_interior_list = [15, 31, 63, 127]
    alpha = 0.1
    r = 0.4
    tf = 0.1

    error_val = []
    dx_val = []

    # 2. Collect errors across all resolutions
    for n_interior in n_interior_list:
        dx = 1.0 / (n_interior + 1)
        t_target = r * (dx**2) / alpha
        n_steps = int(np.ceil(tf / t_target))
        dt = tf / n_steps

        x, u_numerical, t_actual = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

        # Calculate exact solution and store max-norm error
        u_exact = np.exp(-alpha * t_actual * np.pi**2) * np.sin(np.pi * x)
        error = np.linalg.norm(u_numerical - u_exact, np.inf)

        error_val.append(error)
        dx_val.append(dx)

    # 3. Calculate spatial convergence rates: log2(error_new / error_old) / log2(dx_new / dx_old)
    # Since dx cuts roughly in half each step, log2(error_old / error_new) should be close to 2.0
    error_val = np.array(error_val)
    dx_val = np.array(dx_val)

    p = np.log2(error_val[1:] / error_val[:-1]) / np.log2(dx_val[1:] / dx_val[:-1])

    print(f"\nCalculated convergence rates (p): {p}")

    # 4. Assert that the convergence rate approaches 2 (FTCS is 2nd-order in space)
    # The finest grids should be very close to 2.0

    assert 1.8 < p[-1] < 2.2, (
        f"Finest-grid convergence order {p[-1]:.4f} "
        "is outside the expected second-order range"
    )
