import numpy as np


def double_well_energy(eta, W):
    """Return the local double-well energy density."""
    return (W / 4) * (eta**2 - 1) ** 2


def double_well_derivative(eta, W):
    """Return the derivative with respect to eta."""
    return W * (eta**3 - eta)
