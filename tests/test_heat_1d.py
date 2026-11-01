import pytest
import numpy as np
from python.heat_1d import solve_heat_ftcs


def test_boundary_conditions():
    n_interior = 31
    alpha = 0.01

    # Calculate a stable dt dynamically (r = 0.4)
    dx = 1.0 / (n_interior + 1)
    r = 0.4
    dt = r * (dx**2) / alpha
    n_steps = 5

    x, u, t_final = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

    # Check that boundary conditions are satisfied
    assert u.size == n_interior + 2
    assert u[0] == pytest.approx(0.0, abs=1e-10)
    assert u[-1] == pytest.approx(0.0, abs=1e-10)
    assert np.all(np.isfinite(u)) == True


def test_approx_vs_analytical_solution():
    N = 31
    alpha = 0.1
    r = 0.4
    t_steps = 32
    dx = 1 / (N + 1)
    dt = r * dx**2 / alpha
    x, u, t_final = solve_heat_ftcs(N, alpha, dt, t_steps)
    u_exact = np.exp(-0.1 * t_final * np.pi) * np.sin(np.pi * x)

    # Optional: If you calculated the norm, you might want to assert on it
    linf_norm = np.linalg.norm(np.abs(u - u_exact), ord=np.inf)

    assert np.all(np.abs(u - u_exact)) < 5e-3


def test_stability_safeguard():
    n_interior = 31
    alpha = 0.01
    dt = 0.1
    n_steps = 5

    # Set r > 0.5 to trigger instability
    dx = 1.0 / (n_interior + 1)
    r = alpha * dt / dx**2
    assert r > 0.5

    with pytest.raises(ValueError, match="FTCS is unstable"):
        solve_heat_ftcs(n_interior, alpha, dt, n_steps)
