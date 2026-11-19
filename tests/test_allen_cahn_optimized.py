import pytest
import numpy as np
from python.allen_cahn_solver import forward_euler_step as original_step
from python.allen_cahn_solver_optimized import forward_euler_step as optimized_step


@pytest.mark.parametrize("N", [100, 200, 400])
def test_single_step_equivalence(N):
    dx = 10.0 / N
    rng = np.random.default_rng(42)
    eta = rng.uniform(-1.0, 1.0, N)
    expected = original_step(eta, dx, 0.001, 1.0, 0.1, 1.0)
    actual = optimized_step(eta, dx, 0.001, 1.0, 0.1, 1.0)

    np.testing.assert_allclose(actual, expected, rtol=1e-12, atol=1e-14)


def test_long_time_numerical_equivalence():
    N = 200
    dx = 0.05
    dt = 0.005
    M = 1.0
    kappa = 0.1
    W = 1.0

    rng = np.random.default_rng(42)
    eta = rng.uniform(-1.0, 1.0, N)

    for step in range(1, 1000):
        expected = original_step(eta, dx, dt, M, kappa, W)
        actual = optimized_step(eta, dx, dt, M, kappa, W)

    np.testing.assert_allclose(actual, expected, rtol=1e-12, atol=1e-14)


def test_input_immutability():
    N = 200
    dx = 10.0 / N
    rng = np.random.default_rng(42)
    eta = rng.uniform(-1.0, 1.0, N)
    eta_old = eta.copy()
    eta_new = optimized_step(eta, dx, 0.001, 1.0, 0.1, 1.0)
    np.testing.assert_allclose(eta_old, eta, rtol=1e-12, atol=1e-10)
    assert not np.shares_memory(eta, eta_new)


@pytest.mark.parametrize("invalid_value", [0.0, -1.0, np.inf, -np.inf, np.nan])
@pytest.mark.parametrize("parameter", ["dx", "dt", "M", "kappa", "W"])
def test_invalid_scalar_parameters(parameter, invalid_value):

    N = 200
    rng = np.random.default_rng(42)

    params = {
        "eta": rng.uniform(-1.0, 1.0, N),
        "dx": 10.0 / N,
        "dt": 0.001,
        "M": 1.0,
        "kappa": 0.1,
        "W": 1.0,
    }

    params[parameter] = invalid_value

    with pytest.raises(ValueError):
        optimized_step(**params)


@pytest.mark.parametrize("invalid_value", [np.nan, np.inf, -np.inf])
def test_nonfinite_field(invalid_value):

    N = 200
    dx = 10.0 / N

    rng = np.random.default_rng(42)
    eta = rng.uniform(-1.0, 1.0, N)

    eta[N // 2] = invalid_value

    with pytest.raises(ValueError):
        optimized_step(eta, dx, 0.001, 1.0, 0.1, 1.0)
