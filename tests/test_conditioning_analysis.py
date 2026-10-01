import numpy as np
import pytest

from python.analyze_conditioning import asymptotic_slope


def test_asymptotic_slope_for_inverse_epsilon_scaling():
    """Verify that 1/epsilon scaling produces a log-log slope of -1."""
    epsilon = np.array([
        1e-2,
        1e-3,
        1e-4,
        1e-5,
    ])

    condition = 4.0 / epsilon

    slope = asymptotic_slope(epsilon, condition)

    assert slope == pytest.approx(-1.0)


def test_asymptotic_slope_rejects_nonpositive_epsilon():
    """Verify that nonpositive epsilon values are rejected."""
    epsilon = np.array([
        1e-2,
        0.0,
        1e-4,
    ])

    condition = np.array([
        4e2,
        4e3,
        4e4,
    ])

    with pytest.raises(ValueError, match="must be positive"):
        asymptotic_slope(epsilon, condition)