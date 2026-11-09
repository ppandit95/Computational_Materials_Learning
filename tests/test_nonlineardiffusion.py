from python.nonlinear_diffusion import diffusivity
import numpy as np
import pytest


def test_constant_diffusivity_case():
    c = np.array([0.0, 0.5, 1.0])
    D0 = 1.0
    beta = 0.0
    expected = np.array([1.0, 1.0, 1.0])
    result = diffusivity(c, D0, beta)
    assert np.allclose(result, expected), f"Expected {expected}, but got {result}"


def test_analytical_solution_case():
    c = np.array([0.0, 0.5, 1.0])
    D0 = 1.0e-14
    beta = 1.0
    expected = [1e-14, 2e-14]
    result = diffusivity(c, D0, beta)
    assert result[0] == pytest.approx(expected[0])
    assert result[2] == pytest.approx(expected[1])


def test_shape_and_type():
    c = np.array([0.0, 0.5, 1.0])
    D0 = 1.0
    beta = 1.0
    result = diffusivity(c, D0, beta)
    assert isinstance(result, np.ndarray), "Result should be a numpy array."
    assert result.shape == c.shape, f"Expected shape {c.shape}, but got {result.shape}"


def test_invalid_parameters():
    c = np.array([-0.1, 0.5, 1.1])  # Invalid concentration values
    D0 = -1.0  # Invalid D0
    beta = -1.0  # Invalid beta
    with pytest.raises(ValueError):
        diffusivity(c, D0, beta)
