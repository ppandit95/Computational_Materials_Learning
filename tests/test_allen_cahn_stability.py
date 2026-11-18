import numpy as np
import pytest
from python.allen_cahn_solver import forward_euler_step
from python.allen_cahn_stability import critical_timestep


def test_ref_timestep():

    dx = 0.05
    M = 1.0
    kappa = 0.1
    W = 1.0

    expected = 2.0 / 162.0

    actual = critical_timestep(dx, M, kappa, W)

    assert actual == pytest.approx(expected, rel=1e-12, abs=0.0)


def test_spatial_refinement():

    M = 1.0
    kappa = 0.1
    W = 1.0

    dt_coarse = critical_timestep(0.1, M, kappa, W)

    dt_medium = critical_timestep(0.05, M, kappa, W)

    dt_fine = critical_timestep(0.025, M, kappa, W)

    assert dt_fine < dt_medium < dt_coarse


def test_invalid_grid_spacing():

    with pytest.raises(ValueError):
        critical_timestep(0.0, 1.0, 0.1, 1.0)

    with pytest.raises(ValueError):
        critical_timestep(-1.0, 1.0, 0.1, 1.0)

    with pytest.raises(ValueError):
        critical_timestep(np.nan, 1.0, 0.1, 1.0)
