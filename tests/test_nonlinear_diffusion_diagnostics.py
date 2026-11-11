import numpy as np
from python.nonlinear_diffusion_solver import solve_nonlinear_diffusion
import pytest


def test_conservation_across_time_steps():
    """
    Test that mass is conserved across different time-step factors.
    """
    D0 = 1.0e-10
    L = 1.0e-3
    N = 64
    beta = 1.0

    dx = L / N
    n_steps = 100

    i = np.arange(N)
    c_initial = 0.5 + 0.2 * np.cos(2 * np.pi * (i + 0.5) / N)

    dt_ref = dx**2 / (2 * D0 * (1 + beta))
    factors = np.array([0.25, 0.5, 0.9])
    initial_mass = np.sum(c_initial) * dx

    for factor in factors:
        dt = factor * dt_ref

        c_final = solve_nonlinear_diffusion(
            c_initial,
            dx,
            D0,
            beta,
            dt,
            n_steps,
        )

        final_mass = np.sum(c_final) * dx
        mass_error = abs(final_mass - initial_mass)

        assert mass_error <= 1.0e-12 * max(
            abs(initial_mass), 1.0e-13
        ), f"Mass conservation failed for factor {factor}"


def test_concentration_bounds():
    """
    Test that concentration remains within physical bounds.
    """
    D0 = 1.0e-10
    L = 1.0e-3
    N = 64
    beta = 1.0

    dx = L / N
    n_steps = 100

    i = np.arange(N)
    c_initial = 0.5 + 0.2 * np.cos(2 * np.pi * (i + 0.5) / N)

    dt_ref = dx**2 / (2 * D0 * (1 + beta))
    factors = np.array([0.25, 0.5, 0.9])

    for factor in factors:
        dt = factor * dt_ref
        c_final = solve_nonlinear_diffusion(
            c_initial,
            dx,
            D0,
            beta,
            dt,
            n_steps,
        )

        min_c = np.min(c_final)
        max_c = np.max(c_final)

        assert (
            min_c >= np.min(c_initial) - 1.0e-12
        ), f"Concentration below min for factor {factor}"
        assert (
            max_c <= np.max(c_initial) + 1.0e-12
        ), f"Concentration above max for factor {factor}"


def test_conservation_stability_limit_rejection():
    """
    Test to verify that solver raises a ValueError for s=1.1.
    """
    D0 = 1.0e-10
    L = 1.0e-3
    N = 64
    beta = 1.0

    dx = L / N
    n_steps = 100

    i = np.arange(N)
    c_initial = 0.5 + 0.2 * np.cos(2 * np.pi * (i + 0.5) / N)

    dt_ref = dx**2 / (2 * D0 * (1 + beta))
    dt = 1.1 * dt_ref

    with pytest.raises(ValueError):
        solve_nonlinear_diffusion(
            c_initial,
            dx,
            D0,
            beta,
            dt,
            n_steps,
        )
