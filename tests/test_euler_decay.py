import numpy as np
import pytest

from python.euler_decay import forward_euler_decay

# -------------------------------------------------------
# Test A: Correct numerical update
# -------------------------------------------------------


def test_correct_numerical_update():

    sol = forward_euler_decay(u0=1.0, lam=2.0, dt=0.2, steps=3)

    expected = np.array([1.0, 0.6, 0.36, 0.216])

    np.testing.assert_allclose(sol, expected, rtol=1e-12, atol=1e-14)

    assert sol.shape == (4,)


# -------------------------------------------------------
# Test B: Numerical stability behavior
# -------------------------------------------------------


def test_stability_behavior():

    u0 = 1.0
    lam = 2.0
    steps = 12

    # Asymptotically stable cases
    for dt in [0.2, 0.8]:

        sol = forward_euler_decay(u0, lam, dt, steps)

        assert abs(sol[-1]) < abs(sol[0])

    # Marginal stability
    sol = forward_euler_decay(u0, lam, 1.0, steps)

    assert np.isclose(abs(sol[-1]), abs(sol[0]))

    # Unstable case
    sol = forward_euler_decay(u0, lam, 1.1, steps)

    assert abs(sol[-1]) > abs(sol[0])


# -------------------------------------------------------
# Test C: Invalid input handling
# -------------------------------------------------------


def test_invalid_inputs():

    # Negative time step
    with pytest.raises(ValueError):
        forward_euler_decay(1.0, 2.0, -0.25, 12)

    # Zero decay coefficient
    with pytest.raises(ValueError):
        forward_euler_decay(1.0, 0.0, 0.5, 12)

    # Negative number of steps
    with pytest.raises(ValueError):
        forward_euler_decay(1.0, 2.0, 0.25, -5)
