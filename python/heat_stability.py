import numpy as np


def amplification_factor(theta, r):
    """
    Compute the FTCS heat-equation amplification factor.

    Parameters
    ----------
    theta : float or ndarray
        Dimensionless wavenumber k*dx.
    r : float
        Dimensionless diffusion number alpha*dt/dx**2.
        Must be nonnegative.

    Returns
    -------
    float or ndarray
        Fourier-mode amplification factor.

    Raises
    ------
    ValueError
        If r is negative.
    """
    return 1 - 4 * r * np.sin(theta / 2) ** 2
