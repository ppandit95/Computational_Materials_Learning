import numpy as np
from python.allen_cahn_solver import (
    forward_euler_step,
    total_free_energy,
    laplacian_neumann,
    allen_cahn_rhs,
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


def test_empty_input_array():
    eta = np.array([])
    with pytest.raises(ValueError):
        laplacian_neumann(eta, 0.05)


def test_single_cell_input():
    eta = np.array([0.1])
    with pytest.raises(ValueError):
        laplacian_neumann(eta, 0.05)


def test_zero_negative_grid_spacing():
    eta = np.array([1.0, 2.0, 3.0])
    dx = 0.0
    with pytest.raises(ValueError):
        laplacian_neumann(eta, dx)
    dx = -0.05
    with pytest.raises(ValueError):
        laplacian_neumann(eta, dx)
    dx = np.nan
    with pytest.raises(ValueError):
        laplacian_neumann(eta, dx)


def test_nonfinite_order_parameter():
    eta = np.array([1.0, np.nan, 3.0, 4.0])
    dx = 0.005
    with pytest.raises(ValueError):
        laplacian_neumann(eta, dx)


def test_nonpositive_timestep():
    N = 200
    L = 10.0
    M = 1.0
    kappa = 0.1
    W = 1.0
    dt = -0.005
    dx = L / N
    x = (np.arange(N) + 0.5) * dx
    eta_initial = np.tanh((x - L / 2) / np.sqrt(2 * kappa / W))
    with pytest.raises(ValueError):
        forward_euler_step(eta_initial, dx, dt, M, kappa, W)


def test_equilibrium_interface_refinement():

    L = 10.0
    M = 1.0
    kappa = 0.1
    W = 1.0
    residuals = []
    for N in [100, 200, 400]:
        dx = L / N
        x = (np.arange(N) + 0.5) * dx

        eta = np.tanh((x - L / 2) / np.sqrt(2 * kappa / W))

        rhs = allen_cahn_rhs(eta, dx, M, kappa, W)

        # Exclude the boundary cells for the
        # isolated-interface discretization study.
        residual = np.max(np.abs(rhs[1:-1]))
        residuals.append(residual)

    assert np.all(np.diff(residuals) <= 0)


def test_interfacial_energy_accuracy():
    L = 10.0
    M = 1.0
    kappa = 0.1
    W = 1.0
    N = 400
    sigma_exact = 2 * np.sqrt(2 * kappa * W) / 3
    dx = L / N
    x = (np.arange(N) + 0.5) * dx
    eta = np.tanh((x - L / 2) / np.sqrt(2 * kappa / W))
    rhs = allen_cahn_rhs(eta, dx, M, kappa, W)
    # Exclude the boundary cells for the
    # isolated-interface discretization study.
    residual = np.max(np.abs(rhs[1:-1]))
    sigma_num = total_free_energy(eta, dx, kappa, W)
    relative_error = abs(sigma_num - sigma_exact) / sigma_exact
    assert relative_error < 1.0e-3
