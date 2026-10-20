import numpy as np
import pytest

from python.euler_diagnostics import (
    calculate_errors,
    observed_orders,
)

# Common scientific parameters
u0 = 1.0
lam = 2.0
source = 1.0
T = 1.0

dt_values = np.array([0.2, 0.1, 0.05, 0.025])


def test_error_decreases_with_refinement():
    """
    Verify that Forward Euler global error decreases
    as the timestep is refined.
    """

    errors = calculate_errors(
        u0,
        lam,
        source,
        T,
        dt_values,
    )

    assert np.all(errors[1:] < errors[:-1])


def test_observed_convergence_order():
    """
    Verify first-order convergence of Forward Euler.
    """

    errors = calculate_errors(
        u0,
        lam,
        source,
        T,
        dt_values,
    )

    convergence_orders = observed_orders(
        dt_values,
        errors,
    )

    assert np.all(np.abs(convergence_orders - 1.0) < 0.1)
