import numpy as np


def jacobi_spectral_radius(n: int) -> float:
    """
    Return the spectral radius for 1D Dirichlet Jacobian Iteration
    """
    if n < 1:
        ValueError("Incompatible Matrix input size.")
    return 1 - (np.pi / (n + 1)) ** 2


def predicted_iteration(n: int, tolerance: float) -> int:
    if n < 1:
        ValueError("Input Matrix size is incompatible.")
    if tolerance < 0.0 or tolerance > 1 or tolerance == np.nan:
        ValueError("Supplied tolerance is incompatible.")
    return np.log(tolerance) / np.log(jacobi_spectral_radius(n))
