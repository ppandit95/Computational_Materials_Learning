import numpy as np
from python.cahn_hilliard_energy import (
    bulk_energy_density,
    chemical_bulk_curvature,
    chemical_bulk_derivative,
    total_mass,
)
import pytest

A = 1.0
x_alpha = 0.2
x_beta = 0.8
dx = 0.005


def test_bulk_enegy_density():
    assert bulk_energy_density(x_alpha, A, x_alpha, x_beta) == pytest.approx(0.0)
    assert bulk_energy_density(x_beta, A, x_alpha, x_beta) == pytest.approx(0.0)


def test_chemical_energy_derivative():
    assert chemical_bulk_derivative(x_alpha, A, x_alpha, x_beta) == pytest.approx(0.0)
    assert chemical_bulk_derivative(x_beta, A, x_alpha, x_beta) == pytest.approx(0.0)
    assert chemical_bulk_derivative(0.5, A, x_alpha, x_beta) == pytest.approx(0.0)


def test_chemical_bulk_curvature():
    assert chemical_bulk_curvature(0.5, A, x_alpha, x_beta) < 0.0
    assert chemical_bulk_curvature(x_alpha, A, x_alpha, x_beta) > 0.0


def test_total_mass():
    x = 0.3 * np.ones(100)
    assert total_mass(x, dx) == pytest.approx(0.3 * 100 * dx)


def test_invalid_composition():
    x = 1.1 * np.ones(100)
    with pytest.raises(ValueError):
        bulk_energy_density(x, A, x_alpha, x_beta)


def test_invalid_x_alpha():
    x = 0.5 * np.ones(100)
    with pytest.raises(ValueError):
        bulk_energy_density(x, A, -x_alpha, x_beta)


def test_invalid_x_beta():
    x = 0.5 * np.ones(100)
    with pytest.raises(ValueError):
        bulk_energy_density(x, A, x_alpha, -x_beta)
