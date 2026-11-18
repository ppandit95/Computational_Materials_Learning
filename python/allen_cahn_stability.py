import numpy as np


def critical_timestep(dx, M, kappa, W):
    """
    Estimate the bulk linear stability limit.

    Parameters
    ----------
    dx : float
        Positive spatial grid spacing.
    M : float
        Positive kinetic mobility.
    kappa : float
        Positive gradient-energy coefficient.
    W : float
        Positive double-well coefficient.

    Returns
    -------
    float
        Conservative linear stability timestep bound.

    Raises
    ------
    ValueError
        If any parameter is nonpositive or nonfinite.

    Notes
    -----
    Based on linearization around eta = +/-1.
    Does not guarantee nonlinear energy stability.
    """

    for name, param in {
        "Grid spacing": dx,
        "Gradient coefficient": kappa,
        "Kinetic mobility": M,
        "Double-well coefficient": W,
    }.items():

        if not np.isfinite(param) or param <= 0.0:
            raise ValueError(f"{name}={param} must be finite and strictly positive.")

    return 2.0 / (M * (4.0 * kappa / dx**2 + 2.0 * W))
