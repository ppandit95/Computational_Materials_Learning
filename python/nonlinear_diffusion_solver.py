import numpy as np
from python.nonlinear_diffusion_fv import nonlinear_diffusion_rhs


def solve_nonlinear_diffusion(
    c0,
    dx,
    D0,
    beta,
    dt,
    n_steps,
):
    """
    Integrate nonlinear diffusion using Forward Euler.

    Parameters
    ----------
    c0 : ndarray
        Initial cell-average concentrations, dimensionless.
    dx : float
        Uniform cell width in metres.
    D0 : float
        Reference diffusivity in m^2/s.
    beta : float
        Dimensionless concentration-dependence parameter.
    dt : float
        Time step in seconds.
    n_steps : int
        Number of Forward Euler steps.

    Returns
    -------
    ndarray
        Final cell-average concentrations.
    """
    if dt <= 0:
        raise ValueError("Time step dt must be positive.")
    if np.all(c0) < 0:
        raise ValueError("Concentration values must be non-negative.")
    if dx <= 0:
        raise ValueError("Cell width dx must be positive.")
    if D0 <= 0:
        raise ValueError("Reference diffusivity D0 must be positive.")
    if n_steps <= 0:
        raise ValueError("Number of steps n_steps must be positive.")
    if beta < 0:
        raise ValueError("Diffusivity coefficient beta must be non-negative.")
    if dt > 0.5 * dx**2 / (D0 * (1 + beta)):
        raise ValueError(
            "Time step dt is too large for stability. " "Use dt <= 0.5 * dx^2 / D0."
        )
    c = c0.copy()
    for _ in range(n_steps):
        rhs = nonlinear_diffusion_rhs(c, dx, D0, beta)
        c += dt * rhs
    return c
