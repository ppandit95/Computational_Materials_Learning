from python.poisson_1d import solve_poisson_1d
import numpy as np
import pytest


def test_poisson_FDM():
    L = 1.0
    N = 3
    f = 2.0

    source = lambda x: f * np.ones_like(x)

    pos, approx_sol = solve_poisson_1d(source, L, N)

    exact_sol = pos * (1.0 - pos)

    np.testing.assert_allclose(
        approx_sol,
        exact_sol,
        rtol=1e-12,
        atol=1e-12,
    )

    assert pos.shape == (N + 2,)
    assert approx_sol.shape == (N + 2,)

    assert approx_sol[0] == pytest.approx(0.0)
    assert approx_sol[-1] == pytest.approx(0.0)


def test_poisson_invalid_length():
    source = lambda x: 2.0 * np.ones_like(x)

    with pytest.raises(ValueError):
        solve_poisson_1d(source, 0.0, 3)
