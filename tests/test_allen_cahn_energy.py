import pytest

from python.allen_cahn_energy import (
    double_well_energy,
    double_well_derivative,
)


def test_double_well_energy():
    eta = 0.3
    W = 2.0

    assert double_well_energy(eta, W) == pytest.approx(0.41405)


def test_double_well_derivative():
    eta = 0.3
    W = 2.0
    h = 1.0e-6

    numerical_derivative = (
        double_well_energy(eta + h, W) - double_well_energy(eta - h, W)
    ) / (2.0 * h)

    analytical_derivative = double_well_derivative(eta, W)

    assert analytical_derivative == pytest.approx(
        numerical_derivative,
        rel=1e-6,
        abs=1e-10,
    )


def test_zero_eta_double_well():
    eta = 0.0
    W = 2.0

    assert double_well_energy(eta, W) == pytest.approx(0.5)
    assert double_well_derivative(eta, W) == pytest.approx(0.0)
