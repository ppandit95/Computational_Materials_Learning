from python.poisson_diagnostics import diagnose_poisson

import pytest
import math
import numpy as np


def test_poisson_diagnostics():

    # Test 1: Finite and nonnegative diagnostics
    result = diagnose_poisson(15)

    assert all(math.isfinite(v) and v >= 0 for v in result.values())

    # Test 2: Algebraic residual
    assert result["algebraic_residual_linf"] < 1.0e-9

    # Test 3: Discretization error decreases with refinement
    N_values = [15, 31, 63]

    errors = np.zeros(len(N_values))

    for i, N in enumerate(N_values):
        result = diagnose_poisson(N)
        errors[i] = result["discretization_error_linf"]

    assert all(
        x > y for x, y in zip(errors, errors[1:])
    ), "Discretization error is not strictly decreasing"

    # Test 4: Second-order convergence
    ratio_1 = errors[0] / errors[1]
    ratio_2 = errors[1] / errors[2]

    assert 3.5 < ratio_1 < 4.5
    assert 3.5 < ratio_2 < 4.5

    # Observed convergence orders
    p1 = np.log(ratio_1) / np.log(2)
    p2 = np.log(ratio_2) / np.log(2)

    assert p1 == pytest.approx(2.0, abs=0.2)
    assert p2 == pytest.approx(2.0, abs=0.2)

    # Test 5: Invalid input
    with pytest.raises(ValueError):
        diagnose_poisson(0)
