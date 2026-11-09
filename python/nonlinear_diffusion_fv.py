import numpy as np
from python.nonlinear_diffusion import diffusivity


def nonlinear_diffusion_rhs(c, dx, D0, beta):
    """
    Compute the conservative finite-volume spatial operator.

    Parameters
    ----------
    c : ndarray
        Cell-average concentrations, dimensionless.
    dx : float
        Uniform cell width in metres.
    D0 : float
        Reference diffusivity in m^2/s.
    beta : float
        Dimensionless diffusivity coefficient.

    Returns
    -------
    ndarray
        Rate of concentration change in s^-1.
    """
    # Diffusivity at cell centres
    D = diffusivity(c, D0, beta)

    # Diffusivity at interior faces (arithmetic mean of neighbouring cells)
    D_faces = 0.5 * (D[1:] + D[:-1])

    # Fluxes on all N+1 faces; the two end faces stay zero (zero-flux BCs)
    flux = np.zeros(c.size + 1)
    flux[1:-1] = -D_faces * (c[1:] - c[:-1]) / dx

    # Conservative update: dc/dt = -(F_{i+1/2} - F_{i-1/2}) / dx
    return -(flux[1:] - flux[:-1]) / dx
