import numpy as np

"""
Calculate the diffusivity for a nonlinear diffusion model.

Parameters:
c (array-like): Concentration values.
D0 (float): Base diffusivity.
beta (float): Nonlinearity parameter.

Returns:
array-like: Diffusivity values.
"""


def diffusivity(c, D0, beta):
    if np.any(c < 0) or np.any(c > 1):
        raise ValueError("Concentration values must be within 0 and 1.")
    if np.any(D0 < 0) or np.any(beta < 0):
        raise ValueError("D0 and beta must be non-negative.")
    if not (np.isfinite(D0) and np.isfinite(beta)):
        raise ValueError("D0 and beta must be finite numbers.")
    return D0 * (1 + beta * c)
