import numpy as np
from python.nonlinear_diffusion_solver import solve_nonlinear_diffusion
from python.nonlinear_diffusion_fv import nonlinear_diffusion_rhs
import pytest


@pytest.mark.parametrize(
    "n_interior, beta, r",
    [
        # n_interior = 16
        (16, 0.0, 0.2),
        (16, 0.0, 0.3),
        (16, 0.0, 0.4),
        (16, 0.5, 0.2),
        (16, 0.5, 0.3),
        (16, 0.5, 0.4),
        (16, 1.0, 0.2),
        (16, 1.0, 0.3),
        (16, 1.0, 0.4),
        # n_interior = 32
        (32, 0.0, 0.2),
        (32, 0.0, 0.3),
        (32, 0.0, 0.4),
        (32, 0.5, 0.2),
        (32, 0.5, 0.3),
        (32, 0.5, 0.4),
        (32, 1.0, 0.2),
        (32, 1.0, 0.3),
        (32, 1.0, 0.4),
        # n_interior = 64
        (64, 0.0, 0.2),
        (64, 0.0, 0.3),
        (64, 0.0, 0.4),
        (64, 0.5, 0.2),
        (64, 0.5, 0.3),
        (64, 0.5, 0.4),
        (64, 1.0, 0.2),
        (64, 1.0, 0.3),
        (64, 1.0, 0.4),
    ],
)
def test_parameterized_mass_conservation(n_interior, beta, r):
    L = 1.0e-3  # m
    N = n_interior
    dx = L / N
    D0 = 1.0e-10
    dt = r * dx**2 / (D0 * (1 + beta))  # Stability condition for Forward Euler
    n_steps = 50
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


def test_discrete_maximum_principle():
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
    assert np.all(np.isfinite(c)), "Concentration contains non-finite values."
    assert (
        np.min(c) >= np.min(c_initial) - 1e-12
    ), f"Concentration went below initial minimum. Min: {np.min(c)}"
    assert (
        np.max(c) <= np.max(c_initial) + 1e-12
    ), f"Concentration went above initial maximum. Max: {np.max(c)}"


def test_constant_diffusivity_case():
    L = 1.0e-3  # m
    N = 64
    dx = L / N
    D0 = 1.0e-10
    beta = 0.0  # Constant diffusivity case
    dt = 0.4 * dx**2 / (D0 * (1 + beta))  # Stability condition for Forward Euler
    n_steps = 100
    c_initial = np.zeros(N + 2)
    for i in range(N + 1):
        c_initial[i] = 0.5 + 0.2 * np.cos(
            2 * np.pi * (i + 0.5) / N
        )  # Non-uniform initial condition
    analytical_rhs = np.zeros(N + 2)
    analytical_rhs[0] = D0 * (c_initial[1] - c_initial[0]) / dx**2  # Left boundary
    analytical_rhs[N + 1] = (
        D0 * (c_initial[N] - c_initial[N + 1]) / dx**2
    )  # Right boundary
    for i in range(1, N + 1):
        analytical_rhs[i] = (
            D0 * (c_initial[i + 1] - 2 * c_initial[i] + c_initial[i - 1]) / dx**2
        )
    numerical_rhs = nonlinear_diffusion_rhs(c_initial, dx, D0, beta)
    np.testing.assert_allclose(
        numerical_rhs,
        analytical_rhs,
        rtol=1e-12,
        atol=1e-14,
        err_msg="Numerical RHS does not match analytical RHS for constant diffusivity case.",
    )
    # Verifying 1 forward Euler step with constant diffusivity
    c_numerical = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, 1)
    c_analytical = c_initial + dt * analytical_rhs
    np.testing.assert_allclose(
        c_numerical,
        c_analytical,
        rtol=1e-12,
        atol=1e-14,
        err_msg="Numerical solution does not match analytical solution after one Forward Euler step for constant diffusivity case.",
    )


INVALID_CASES = [
    pytest.param([1.01, 0.5], 1.0e-10, 0.4, 1.0e-5, 0.4 * 1.0e-10, 10, id="c0_above_1"),
    pytest.param(
        [np.nan, 0.5], 1.0e-10, 0.4, 1.0e-5, 0.4 * 1.0e-10, 10, id="c0_is_nan"
    ),
    pytest.param(
        [0.5, 0.5], -1.0e-10, 0.4, 1.0e-5, 0.4 * 1.0e-10, 10, id="negative_D0"
    ),
    pytest.param(
        [0.5, 0.5], 1.0e-10, -0.4, 1.0e-5, 0.4 * 1.0e-10, 10, id="negative_beta"
    ),
    pytest.param([0.5, 0.5], 1.0e-10, 0.4, 0.0, 0.4 * 1.0e-10, 10, id="zero_dx"),
    pytest.param(
        [0.5, 0.5], 1.0e-10, 0.4, -1.0e-5, 0.4 * 1.0e-10, 10, id="negative_dx"
    ),
    pytest.param(
        [0.5, 0.5], 1.0e-10, 0.4, 1.0e-5, -0.4 * 1.0e-10, 10, id="negative_dt"
    ),
    pytest.param(
        [0.5, 0.5], 1.0e-10, 0.4, 1.0e-5, 0.4 * 1.0e-10, -10, id="negative_n_steps"
    ),
    pytest.param(
        [0.5], 1.0e-10, 0.4, 1.0e-5, 1.01 * 1.0e-10, 10, id="cfl_unstable_r_eq_1"
    ),
]


@pytest.mark.parametrize("c0, D0, beta, dx, dt, n_steps", INVALID_CASES)
def test_invalid_inputs_raise_error(c0, D0, beta, dx, dt, n_steps):
    with pytest.raises(ValueError):
        solve_nonlinear_diffusion(c0, dx, D0, beta, dt, n_steps)
