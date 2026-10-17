"""
Automated scientific tests for Forward Euler temporal accuracy.

Day 72/365 — Week 11, Day 2

Governing equation
------------------
    du/dt = -lambda * u

Initial condition
-----------------
    u(0) = u0

Analytical solution
-------------------
    u(t) = u0 * exp(-lambda * t)

Scientific objectives
---------------------
1. Verify the analytical solution.
2. Verify the Forward Euler final-time error.
3. Confirm that temporal refinement reduces the error.
4. Verify first-order temporal convergence.
5. Verify the observed-order calculation.
6. Test invalid inputs and numerical edge cases.
"""

import numpy as np
import pytest

from python.euler_decay import (
    forward_euler_decay,
    exact_decay,
)

from python.euler_accuracy import (
    final_time_error,
    observed_order,
)

# ============================================================
# Shared scientific parameters
# ============================================================

U0 = 1.0
LAMBDA = 2.0
FINAL_TIME = 1.0

TIME_STEPS = np.array(
    [
        0.2,
        0.1,
        0.05,
        0.025,
    ]
)


# ============================================================
# Test 1: Analytical solution
# ============================================================


def test_exact_decay_matches_analytical_solution():
    """
    Verify the analytical solution at selected times.
    """

    times = np.array([0.0, 0.5, 1.0])

    numerical = exact_decay(
        U0,
        LAMBDA,
        times,
    )

    expected = U0 * np.exp(-LAMBDA * times)

    np.testing.assert_allclose(
        numerical,
        expected,
        rtol=1e-12,
        atol=1e-14,
    )


# ============================================================
# Test 2: Initial condition
# ============================================================


def test_exact_decay_preserves_initial_condition():
    """
    At t = 0, the analytical solution must equal u0.
    """

    times = np.array([0.0])

    result = exact_decay(
        U0,
        LAMBDA,
        times,
    )

    assert result[0] == pytest.approx(U0)


# ============================================================
# Test 3: Forward Euler solution
# ============================================================


def test_forward_euler_known_solution():
    """
    Verify Forward Euler against a hand-calculated solution.

    For lambda = 2 and dt = 0.2:

        amplification factor = 1 - lambda*dt
                             = 0.6

    Therefore:

        u0 = 1.0
        u1 = 0.6
        u2 = 0.36
        u3 = 0.216
    """

    solution = forward_euler_decay(
        u0=1.0,
        lam=2.0,
        dt=0.2,
        steps=3,
    )

    expected = np.array(
        [
            1.0,
            0.6,
            0.36,
            0.216,
        ]
    )

    np.testing.assert_allclose(
        solution,
        expected,
        rtol=1e-12,
        atol=1e-14,
    )


# ============================================================
# Test 4: Final-time error
# ============================================================


def test_final_time_error_matches_manual_calculation():
    """
    Verify the final-time error using an independent formula.

    For dt = 0.2:

        N = T/dt = 5

        u_FE(T) = (1 - lambda*dt)^N
                = 0.6^5

        u_exact(T) = exp(-2)

        error = |u_FE(T) - u_exact(T)|
    """

    dt = 0.2
    steps = 5

    expected_numerical = U0 * (1.0 - LAMBDA * dt) ** steps

    expected_exact = U0 * np.exp(-LAMBDA * FINAL_TIME)

    expected_error = abs(expected_numerical - expected_exact)

    calculated_error = final_time_error(
        U0,
        LAMBDA,
        dt,
        FINAL_TIME,
    )

    assert calculated_error == pytest.approx(
        expected_error,
        rel=1e-12,
        abs=1e-14,
    )


# ============================================================
# Test 5: Error decreases under refinement
# ============================================================


def test_error_decreases_with_timestep_refinement():
    """
    Verify that smaller time steps produce smaller errors.

    This test uses the stable regime:

        lambda * dt < 1

    for positive, monotone decay.
    """

    errors = np.array(
        [
            final_time_error(
                U0,
                LAMBDA,
                dt,
                FINAL_TIME,
            )
            for dt in TIME_STEPS
        ]
    )

    assert np.all(np.isfinite(errors))

    assert np.all(errors > 0.0)

    assert np.all(np.diff(errors) < 0.0)


# ============================================================
# Test 6: First-order temporal convergence
# ============================================================


def test_forward_euler_first_order_convergence():
    """
    Verify that the observed convergence order approaches one.

    For first-order accuracy:

        E(dt) = C*dt + O(dt^2)

    The observed order is:

        p = log(E_coarse/E_fine) / log(2)

    Expected asymptotic behavior:

        p -> 1 as dt -> 0
    """

    errors = np.array(
        [
            final_time_error(
                U0,
                LAMBDA,
                dt,
                FINAL_TIME,
            )
            for dt in TIME_STEPS
        ]
    )

    orders = np.array(
        [
            observed_order(
                errors[i],
                errors[i + 1],
            )
            for i in range(len(errors) - 1)
        ]
    )

    assert np.all(np.isfinite(orders))

    # Verify that the finest-grid estimate is close to 1.
    assert 0.9 < orders[-1] < 1.1

    # Verify that successive estimates approach 1.
    assert abs(orders[-1] - 1.0) < abs(orders[0] - 1.0)


# ============================================================
# Test 7: Observed-order formula
# ============================================================


@pytest.mark.parametrize(
    "error_coarse,error_fine,expected_order",
    [
        (0.02, 0.01, 1.0),
        (0.04, 0.01, 2.0),
        (0.08, 0.01, 3.0),
    ],
)
def test_observed_order_known_values(
    error_coarse,
    error_fine,
    expected_order,
):
    """
    Verify the observed-order formula against known values.
    """

    result = observed_order(
        error_coarse,
        error_fine,
    )

    assert result == pytest.approx(
        expected_order,
        rel=1e-12,
    )


# ============================================================
# Test 8: Invalid time steps
# ============================================================


@pytest.mark.parametrize(
    "invalid_dt",
    [
        0.0,
        -0.1,
        np.nan,
        np.inf,
        -np.inf,
    ],
)
def test_final_time_error_rejects_invalid_dt(invalid_dt):
    """
    Time step must be positive and finite.
    """

    with pytest.raises(ValueError):
        final_time_error(
            U0,
            LAMBDA,
            invalid_dt,
            FINAL_TIME,
        )


# ============================================================
# Test 9: Invalid final times
# ============================================================


@pytest.mark.parametrize(
    "invalid_time",
    [
        0.0,
        -1.0,
        np.nan,
        np.inf,
        -np.inf,
    ],
)
def test_final_time_error_rejects_invalid_final_time(
    invalid_time,
):
    """
    Final time must be positive and finite.
    """

    with pytest.raises(ValueError):
        final_time_error(
            U0,
            LAMBDA,
            0.1,
            invalid_time,
        )


# ============================================================
# Test 10: Incompatible time step and final time
# ============================================================


def test_final_time_error_rejects_noninteger_steps():
    """
    Reject time steps that cannot reach the requested final time
    using an integer number of Forward Euler steps.

    Example:

        T = 1.0
        dt = 0.3

        T/dt = 3.333...

    Therefore, the final time cannot be reached exactly.
    """

    with pytest.raises(ValueError):
        final_time_error(
            U0,
            LAMBDA,
            0.3,
            FINAL_TIME,
        )


# ============================================================
# Test 11: Invalid observed-order inputs
# ============================================================


@pytest.mark.parametrize(
    "error_coarse,error_fine",
    [
        (0.0, 0.01),
        (0.01, 0.0),
        (-0.01, 0.01),
        (0.01, -0.01),
        (np.nan, 0.01),
        (0.01, np.nan),
        (np.inf, 0.01),
        (0.01, np.inf),
    ],
)
def test_observed_order_rejects_invalid_errors(
    error_coarse,
    error_fine,
):
    """
    Logarithmic convergence-order estimation requires
    positive, finite errors.
    """

    with pytest.raises(ValueError):
        observed_order(
            error_coarse,
            error_fine,
        )


# ============================================================
# Test 12: No refinement gives zero observed order
# ============================================================


def test_observed_order_equal_errors():
    """
    If errors are equal:

        E_coarse/E_fine = 1

    Therefore:

        p = log(1)/log(2) = 0
    """

    result = observed_order(
        0.01,
        0.01,
    )

    assert result == pytest.approx(0.0)


# ============================================================
# Test 13: Exact solution array shape
# ============================================================


def test_exact_decay_output_shape():
    """
    Verify that the analytical solution preserves
    the shape of the input time array.
    """

    times = np.linspace(
        0.0,
        FINAL_TIME,
        21,
    )

    result = exact_decay(
        U0,
        LAMBDA,
        times,
    )

    assert isinstance(result, np.ndarray)

    assert result.shape == times.shape


# ============================================================
# Test 14: Convergence errors match reference values
# ============================================================


def test_convergence_errors_match_reference():
    """
    Regression test against independently verified
    final-time errors for the selected decay problem.
    """

    expected_errors = np.array(
        [
            0.0575752832366127,
            0.0279611008366127,
            0.0137586282482399,
            0.006823129623,
        ]
    )

    calculated_errors = np.array(
        [
            final_time_error(
                U0,
                LAMBDA,
                dt,
                FINAL_TIME,
            )
            for dt in TIME_STEPS
        ]
    )

    np.testing.assert_allclose(
        calculated_errors,
        expected_errors,
        rtol=1e-6,
        atol=1e-9,
    )
