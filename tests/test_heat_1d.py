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


@pytest.mark.parametrize(
    "n_interior, alpha, r, n_steps",
    [
        (31, 0.01, 0.4, 0),
        (31, 0.01, 0.4, 1),
        (31, 0.01, 0.4, 8),
        (31, 0.01, 0.4, 32),
        (31, 0.01, 0.5, 0),
        (31, 0.01, 0.5, 1),
        (31, 0.01, 0.5, 8),
        (31, 0.01, 0.5, 32),
        (50, 0.02, 0.4, 10),
        (100, 0.05, 0.4, 20),
    ],
)
def test_maximum_principle_regression(n_interior, alpha, r, n_steps):

    dx = 1.0 / (n_interior + 1)
    dt = r * dx**2 / alpha

    def initial_condition(x):
        return np.sin(2 * np.pi * x)

    x, u, t_final = solve_heat_ftcs(
        n_interior,
        alpha,
        dt,
        n_steps,
        initial_condition=initial_condition,
    )

    u_initial = initial_condition(x)

    tolerance = 1e-12

    assert np.all(np.isfinite(u))

    assert np.max(u) <= np.max(u_initial) + tolerance

    assert np.min(u) >= np.min(u_initial) - tolerance

    assert u[0] == pytest.approx(0.0, abs=tolerance)
    assert u[-1] == pytest.approx(0.0, abs=tolerance)


def test_zero_timesteps():
    n_interior = 31
    alpha = 0.01

    dx = 1.0 / (n_interior + 1)
    dt = 0.4 * dx**2 / alpha

    n_steps = 0

    x, u, t_final = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

    # Expected initial condition
    u_initial = np.sin(np.pi * x)

    # No time evolution should occur
    np.testing.assert_allclose(u, u_initial, rtol=0.0, atol=1e-14)

    # Final time must remain zero
    assert t_final == pytest.approx(0.0)


def test_min_spatial_resolution():
    n_interior = 1
    alpha = 0.01
    dx = 1.0 / (n_interior + 1)
    dt = 0.5 * (dx**2) / alpha  # Kept stable
    n_steps = 1

    x, u, t_final = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

    assert u.size == n_interior + 2
    assert u[0] == pytest.approx(0.0, abs=1e-10)
    assert u[-1] == pytest.approx(0.0, abs=1e-10)
    assert np.all(np.isfinite(u)) == True
    assert u[1] == pytest.approx(0.0, abs=1e-10)


def test_CFL_boundary_rejection():
    n_interior = 31
    alpha = 0.01
    dx = 1.0 / (n_interior + 1)

    dt = 0.5 * dx**2 / alpha
    dt2 = 0.5001 * dx**2 / alpha

    n_steps = 5

    # Test 1: CFL boundary r = 0.5 should be accepted
    x, u, t_final = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

    assert np.all(np.isfinite(u))
    assert t_final == pytest.approx(n_steps * dt)

    # Test 2: CFL violation r = 0.5001 should be rejected
    with pytest.raises(ValueError, match=".*"):
        solve_heat_ftcs(n_interior, alpha, dt2, n_steps)
