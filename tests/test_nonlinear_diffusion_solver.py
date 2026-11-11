import numpy as np
from python.nonlinear_diffusion_solver import nonlinear_diffusion_rhs
from python.nonlinear_diffusion_solver import solve_nonlinear_diffusion


def test_mass_conservation():
    L = 1.0e-3  # m
    N = 64
    dx = L / N
    D0 = 1.0e-10
    beta = 1.0
    dt = 0.4 * dx**2 / (D0 * (1 + beta))  # Stability condition for Forward Euler
    n_steps = 100
    c_initial = np.zeros(N + 2)
    for i in range(N + 1):
        c_initial[i] = 0.5 + 0.2 * np.cos(
            2 * np.pi * (i + 0.5) / N
        )  # Non-uniform initial condition

    c = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_steps)
    initial_mass = np.sum(c_initial) * dx
    final_mass = np.sum(c) * dx
    mass_difference = np.abs(final_mass - initial_mass)
    assert mass_difference <= 1e-12 * max(
        np.abs(initial_mass), 1e-30
    ), f"Mass is not conserved. Difference: {mass_difference}"


def test_concentration_bounds():
    L = 1.0e-3  # m
    N = 64
    dx = L / N
    D0 = 1.0e-10
    beta = 1.0
    dt = 0.4 * dx**2 / (D0 * (1 + beta))  # Stability condition for Forward Euler
    n_steps = 100
    c_initial = np.zeros(N + 2)
    for i in range(N + 1):
        c_initial[i] = 0.5 + 0.2 * np.cos(
            2 * np.pi * (i + 0.5) / N
        )  # Non-uniform initial condition

    c = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_steps)
    assert (
        np.min(c) >= np.min(c_initial) - 1e-12
    ), f"Concentration went below initial minimum. Min: {np.min(c)}"
    assert (
        np.max(c) <= np.max(c_initial) + 1e-12
    ), f"Concentration went above initial maximum. Max: {np.max(c)}"


def test_constant_concentration():
    L = 1.0e-3  # m
    N = 64
    dx = L / N
    D0 = 1.0e-10
    beta = 1.0
    dt = 0.4 * dx**2 / (D0 * (1 + beta))  # Stability condition for Forward Euler
    n_steps = 100
    c_initial = np.full(N + 2, 0.4)  # Constant initial concentration

    c = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_steps)
    assert np.allclose(
        c, c_initial
    ), f"Concentration changed from constant initial value. Result: {c}"


def test_unstable_timestep_rejection():
    L = 1.0e-3  # m
    N = 64
    dx = L / N
    D0 = 1.0e-10
    beta = 1.0
    dt = 1.0 * dx**2 / (D0 * (1 + beta))  # Intentionally too large for stability
    n_steps = 100
    c_initial = np.zeros(N + 2)
    for i in range(N + 1):
        c_initial[i] = 0.5 + 0.2 * np.cos(
            2 * np.pi * (i + 0.5) / N
        )  # Non-uniform initial condition

    try:
        solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_steps)
        assert (
            False
        ), "Expected ValueError for unstable timestep, but no error was raised."
    except ValueError as e:
        assert "Time step dt is too large for stability" in str(
            e
        ), f"Unexpected error message: {str(e)}"
