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


# ============================================================
# Test: Zero Time Steps
# ============================================================


def test_zero_steps():
    sol = forward_euler_forced(u0=1.5, lam=2.0, source=1.0, dt=0.1, steps=0)
    assert sol.shape == (1,)
    assert sol[0] == pytest.approx(1.5)


# ============================================================
# Test: Stable_Perturbation Decay
# ============================================================


def test_stable_perturbation_decay():
    sol = forward_euler_forced(u0=0.6, lam=2.0, source=1.0, dt=0.4, steps=12)
    assert sol[1] <= sol[0]


# ============================================================
# Test: Unstable Perturbation Growth
# ============================================================


def test_unstable_perturbation_growth():

    approx_sol = forward_euler_forced(
        u0=0.6,
        lam=2.0,
        source=1.0,
        dt=1.1,
        steps=6,
    )

    equilibrium = 1.0 / 2.0

    perturbation = approx_sol - equilibrium

    perturbation_magnitude = np.abs(perturbation)

    assert np.all(perturbation_magnitude[1:] > perturbation_magnitude[:-1])


# ============================================================
# Test: Stability Boundary
# ============================================================


def test_stability_boundary():

    lam = 2.0
    dt = 1.0
    source = 1.0

    approx_sol = forward_euler_forced(
        u0=0.6,
        lam=lam,
        source=source,
        dt=dt,
        steps=6,
    )

    equilibrium = source / lam

    perturbation = approx_sol - equilibrium

    perturbation_magnitude = np.abs(perturbation)

    # Verify constant perturbation magnitude.
    np.testing.assert_allclose(
        perturbation_magnitude[1:],
        perturbation_magnitude[:-1],
        rtol=1e-12,
        atol=1e-14,
    )

    # Verify alternating perturbation sign.
    np.testing.assert_allclose(
        perturbation[1:],
        -perturbation[:-1],
        rtol=1e-12,
        atol=1e-14,
    )


@pytest.mark.parametrize(
    "u0,lam,source,dt,steps",
    [
        # Invalid number of steps
        (0.6, 2.0, 1.0, 0.4, -1),
        (0.6, 2.0, 1.0, 0.4, 2.5),
        (0.6, 2.0, 1.0, 0.4, np.nan),
        (0.6, 2.0, 1.0, 0.4, True),
        # Invalid decay coefficient
        (0.6, np.inf, 1.0, 0.4, 12),
        # Invalid source
        (0.6, 2.0, np.nan, 0.4, 12),
        # Invalid time step
        (0.6, 2.0, 1.0, 0.0, 12),
    ],
)
def test_forward_euler_forced_invalid_inputs(
    u0,
    lam,
    source,
    dt,
    steps,
):
    """
    Verify that invalid solver inputs raise ValueError.
    """

    with pytest.raises(ValueError):
        forward_euler_forced(
            u0=u0,
            lam=lam,
            source=source,
            dt=dt,
            steps=steps,
        )
