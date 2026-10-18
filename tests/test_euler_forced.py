"""
Scientific verification tests for forced Forward Euler.

Governing equation
------------------
du/dt = -lam*u + source

Day 73/365
"""

import numpy as np
import pytest

from python.euler_forced import (
    forward_euler_forced,
    exact_forced_decay,
)

from python.euler_decay import forward_euler_decay

# ============================================================
# Test A: Known numerical solution
# ============================================================


def test_known_numerical_solution():
    """
    Verify the Forward Euler solution against a known value.
    """

    sol = forward_euler_forced(
        u0=1.0,
        lam=2.0,
        source=1.0,
        dt=0.1,
        steps=10,
    )

    assert len(sol) == 11

    assert sol[-1] == pytest.approx(
        0.5536870912,
        rel=1e-10,
    )


# ============================================================
# Test B: Equilibrium preservation
# ============================================================


def test_equilibrium_preservation():
    """
    Verify that the numerical solution preserves equilibrium.

    Equilibrium:

        u_eq = source / lam
    """

    equilibrium = 0.5

    sol = forward_euler_forced(
        u0=equilibrium,
        lam=2.0,
        source=1.0,
        dt=0.1,
        steps=100,
    )

    expected = np.full(
        sol.shape,
        equilibrium,
    )

    np.testing.assert_allclose(
        sol,
        expected,
        rtol=1e-12,
        atol=1e-14,
    )


# ============================================================
# Test C: Backward compatibility
# ============================================================


def test_backward_compatibility():
    """
    Setting source=0 must recover the original decay solver.
    """

    sol_forced = forward_euler_forced(
        u0=1.0,
        lam=2.0,
        source=0.0,
        dt=0.1,
        steps=10,
    )

    sol_original = forward_euler_decay(
        u0=1.0,
        lam=2.0,
        dt=0.1,
        steps=10,
    )

    np.testing.assert_allclose(
        sol_forced,
        sol_original,
        rtol=1e-12,
        atol=1e-14,
    )


# ============================================================
# Test D: Input validation
# ============================================================


@pytest.mark.parametrize(
    "lam,dt,steps",
    [
        (0.0, 0.1, 10),
        (-2.0, 0.1, 10),
        (np.nan, 0.1, 10),
        (np.inf, 0.1, 10),
        (2.0, 0.0, 10),
        (2.0, -0.1, 10),
        (2.0, np.nan, 10),
        (2.0, np.inf, 10),
        (2.0, 0.1, -1),
        (2.0, 0.1, 2.5),
    ],
)
def test_invalid_inputs(lam, dt, steps):
    """
    Invalid parameters must raise ValueError.
    """

    with pytest.raises(ValueError):
        forward_euler_forced(
            u0=1.0,
            lam=lam,
            source=1.0,
            dt=dt,
            steps=steps,
        )


# ============================================================
# Additional test: Analytical solution
# ============================================================


def test_analytical_solution():
    """
    Verify the analytical solution at t=1.
    """

    times = np.array([0.0, 1.0])

    exact = exact_forced_decay(
        u0=1.0,
        lam=2.0,
        source=1.0,
        times=times,
    )

    assert exact[0] == pytest.approx(1.0)

    assert exact[-1] == pytest.approx(
        0.5676676416183064,
        rel=1e-12,
    )
