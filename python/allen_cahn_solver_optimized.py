import numpy as np

from .allen_cahn_energy import (
    double_well_energy,
    double_well_derivative,
)


def _laplacian_neumann_unchecked(eta, dx):
    padded = np.pad(
        eta, (1, 1), mode="edge"
    )  # Pad an array with one repeated edge value before and after each axis.

    return (padded[2:] - 2.0 * padded[1:-1] + padded[:-2]) / dx**2


def _validate_field(eta, dx):
    """Validate a one-dimensional cell-centred field."""

    eta = np.asarray(eta, dtype=np.float64)

    if eta.ndim != 1:
        raise ValueError("eta must be one-dimensional.")

    if eta.size < 2:
        raise ValueError("eta must contain at least two cells.")

    if not np.all(np.isfinite(eta)):
        raise ValueError("eta must contain only finite values.")

    if not np.isfinite(dx) or dx <= 0.0:
        raise ValueError("dx must be positive and finite.")

    return eta


def _validate_positive_parameter(value, name):
    """Validate a positive finite scalar parameter."""

    if not np.isfinite(value) or value <= 0.0:
        raise ValueError(f"{name} must be positive and finite.")


def laplacian_neumann(eta, dx):
    """
    Calculate the discrete Laplacian with zero-flux boundaries.

    Parameters
    ----------
    eta : array_like
        One-dimensional cell-centred order parameter.
    dx : float
        Positive spatial grid spacing.

    Returns
    -------
    numpy.ndarray
        Discrete Laplacian at each physical cell.

    Raises
    ------
    ValueError
        If the field or grid spacing is invalid.
    """

    eta = _validate_field(eta, dx)

    # return (eta_padded[2:] - 2.0 * eta_padded[1:-1] + eta_padded[:-2]) / dx**2
    return _laplacian_neumann_unchecked(eta, dx)


def _allen_cahn_rhs_unchecked(eta, dx, M, kappa, W):

    laplacian = _laplacian_neumann_unchecked(eta, dx)
    derivative = double_well_derivative(eta, W)
    return M * (kappa * laplacian - derivative)


def allen_cahn_rhs(eta, dx, M, kappa, W):
    """
    Calculate the Allen-Cahn time derivative.

    Parameters
    ----------
    eta : array_like
        Cell-centred order parameter.
    dx : float
        Spatial grid spacing.
    M : float
        Positive kinetic mobility.
    kappa : float
        Positive gradient-energy coefficient.
    W : float
        Positive double-well energy coefficient.

    Returns
    -------
    numpy.ndarray
        Time derivative of the order parameter.

    Raises
    ------
    ValueError
        If any input is invalid.
    """

    eta = _validate_field(eta, dx)

    _validate_positive_parameter(M, "M")
    _validate_positive_parameter(kappa, "kappa")
    _validate_positive_parameter(W, "W")

    return _allen_cahn_rhs_unchecked(eta, dx, M, kappa, W)


def forward_euler_step(eta, dx, dt, M, kappa, W):
    """
    Advance the Allen-Cahn equation by one explicit Euler step.

    Parameters
    ----------
    eta : array_like
        Current order-parameter field.
    dx : float
        Spatial grid spacing.
    dt : float
        Positive timestep.
    M : float
        Kinetic mobility.
    kappa : float
        Gradient-energy coefficient.
    W : float
        Double-well energy coefficient.

    Returns
    -------
    numpy.ndarray
        Updated order parameter in independent storage.

    Raises
    ------
    ValueError
        If any input is invalid.

    Notes
    -----
    The caller is responsible for choosing a stable timestep.
    The original input field is not modified.
    """

    _validate_positive_parameter(dt, "dt")

    eta = _validate_field(eta, dx)

    _validate_positive_parameter(M, "M")
    _validate_positive_parameter(kappa, "kappa")
    _validate_positive_parameter(W, "W")

    rhs = _allen_cahn_rhs_unchecked(eta, dx, M, kappa, W)

    return eta + dt * rhs


def total_free_energy(eta, dx, kappa, W):
    """
    Calculate the discrete Allen-Cahn free energy.

    Parameters
    ----------
    eta : array_like
        Cell-centred order parameter.
    dx : float
        Spatial grid spacing.
    kappa : float
        Gradient-energy coefficient.
    W : float
        Double-well energy coefficient.

    Returns
    -------
    float
        Sum of discrete bulk and gradient energies.

    Raises
    ------
    ValueError
        If any input is invalid.

    Notes
    -----
    The discrete gradient energy is consistent with the
    zero-flux finite-difference Laplacian.
    """

    eta = _validate_field(eta, dx)

    _validate_positive_parameter(kappa, "kappa")
    _validate_positive_parameter(W, "W")

    bulk_energy = dx * np.sum(double_well_energy(eta, W))

    gradient_energy = (kappa / (2.0 * dx)) * np.sum(np.diff(eta) ** 2)

    return float(bulk_energy + gradient_energy)
