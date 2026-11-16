import numpy as np
from python.allen_cahn_solver import (
    forward_euler_step,
    total_free_energy,
    laplacian_neumann,
)
import pytest


def test_constant_field_laplacian():
    eta = 5 * np.zeros(25)
    dx = 0.5
    result = laplacian_neumann(eta, dx)
    expected = np.zeros(25)
    np.testing.assert_allclose(result, expected)


def test_boundary_stencil():
    exp_lap = np.array([1, 0, -1])
    dx = 1.0
    eta = np.array([0, 1, 2])
    np.testing.assert_allclose(laplacian_neumann(eta, dx), exp_lap)


def test_stationary_pure_phases():
    eta = np.ones(25)
    eta_new = forward_euler_step(eta, 0.1, 0.05, 1, 1, 0.25)
    np.testing.assert_allclose(eta, eta_new)


def test_energy_dissipation():
    N = 200
    L = 10.0
    dx = L / N

    x = (np.arange(N) + 0.5) * dx

    eta_initial = 0.8 * np.tanh((x - L / 2) / 0.8)
    M = 1.0
    W = 1.0
    kappa = 0.1
    dt = 0.005

    energy_history = np.zeros(100)

    energy_history[0] = total_free_energy(eta_initial, dx, kappa, W)

    for step in range(1, 100):
        if step == 1:
            eta_new = forward_euler_step(eta_initial, dx, dt, M, kappa, W)
        else:
            eta_new = forward_euler_step(eta_old, dx, dt, M, kappa, W)
        eta_old = eta_new
        energy_history[step] = total_free_energy(eta_new, dx, kappa, W)

    assert np.all(np.diff(energy_history) <= 1e-10)
