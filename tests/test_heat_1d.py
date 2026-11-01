import pytest
import numpy as np
from python.heat_1d import solve_heat_ftcs, two_mode_initial_condition


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


def test_Backward_compatibility():
    n_interior = 31
    alpha = 0.01

    # Calculate a stable dt dynamically (r = 0.4)
    dx = 1.0 / (n_interior + 1)
    r = 0.4
    dt = r * (dx**2) / alpha
    n_steps = 5

    # Call the function without the initial_condition argument
    x, u, t_final = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

    # Check that boundary conditions are satisfied
    assert u.size == n_interior + 2
    assert u[0] == pytest.approx(0.0, abs=1e-10)
    assert u[-1] == pytest.approx(0.0, abs=1e-10)
    assert np.all(np.isfinite(u)) == True


def test_Two_mode_analytical_verification():
    n_interior = 31
    alpha = 0.01

    # Calculate a stable dt dynamically (r = 0.4)
    dx = 1.0 / (n_interior + 1)
    r = 0.4
    dt = r * (dx**2) / alpha
    n_steps = 5

    # Call the function with the two-mode initial condition
    x, u, t_final = solve_heat_ftcs(
        n_interior, alpha, dt, n_steps, initial_condition=two_mode_initial_condition
    )

    u_exact = np.exp(-0.1 * t_final * np.pi) * np.sin(np.pi * x) + 0.25 * np.exp(
        -0.1 * t_final * 3 * np.pi
    ) * np.sin(3 * np.pi * x)

    # Check that boundary conditions are satisfied
    assert u.size == n_interior + 2
    assert u[0] == pytest.approx(0.0, abs=1e-10)
    assert u[-1] == pytest.approx(0.0, abs=1e-10)
    assert np.all(np.isfinite(u)) == True

    linf_norm = np.linalg.norm(np.abs(u - u_exact), ord=np.inf)

    assert np.all(np.abs(u - u_exact)) < 5e-3


def test_invalid_initial_condition_shape():
    n_interior = 31
    alpha = 0.01
    dx = 1.0 / (n_interior + 1)
    dt = 0.4 * (dx**2) / alpha  # Kept stable
    n_steps = 5

    # A callable that returns an incorrectly shaped array
    # (e.g., length 10 instead of n_interior or n_interior + 2)
    def bad_initial_condition(x):
        return np.zeros(10)

    with pytest.raises(
        ValueError, match=".*shape.*|.*dimension.*|.*initial condition.*"
    ):
        # Pass the bad callable into your solver
        # (Adjust arguments if your function signature uses a different parameter name)
        solve_heat_ftcs(
            n_interior, alpha, dt, n_steps, initial_condition=bad_initial_condition
        )
