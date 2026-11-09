import numpy as np
import pytest
from python.nonlinear_diffusion_fv import nonlinear_diffusion_rhs


def test_uniform_concentration():
    c = np.array([0.4, 0.4, 0.4])
    dx = 1.0
    D0 = 1.0
    beta = 1.0
    result = nonlinear_diffusion_rhs(c, dx, D0, beta)
    expected = np.zeros_like(c)
    assert np.allclose(result, expected), f"Expected {expected}, but got {result}"


def test_global_mass_conservation():
    c = 0.5 * np.ones(64) + 0.2 * np.sin(
        2 * np.pi * np.arange(64) / 64
    )  # Non-uniform initial condition
    dx = (1e-3) / 64
    D0 = 1.0e-14
    beta = 1.0
    result = nonlinear_diffusion_rhs(c, dx, D0, beta)
    total_change = np.sum(result) * dx
    assert np.isclose(
        total_change, 0.0
    ), f"Total change should be zero for mass conservation, but got {total_change}"


def test_constant_diffusivity_case():
    c = np.array([0.0, 0.5, 1.0])
    dx = 1.0
    D0 = 1.0
    beta = 0.0

    result = nonlinear_diffusion_rhs(c, dx, D0, beta)

    expected = np.array([0.5, 0.0, -0.5])

    np.testing.assert_allclose(
        result,
        expected,
        rtol=1e-12,
        atol=1e-14,
    )
