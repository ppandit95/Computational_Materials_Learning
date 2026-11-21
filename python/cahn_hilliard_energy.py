import numpy as np


def _validate_composition(x):
    """
    Validate that a composition lies within the physical mole-fraction range.

    Parameters
    ----------
    x : float or array_like
        Solute mole fraction. Every value must be finite and satisfy
        0 <= x <= 1.

    Raises
    ------
    ValueError
        If any composition is nonfinite or lies outside [0, 1].

    Notes
    -----
    Mole fractions are dimensionless.

    This function validates physical bounds only. It does not check
    the ordering of equilibrium compositions.
    """
    x = np.asarray(x, dtype=np.float64)

    if np.any(~np.isfinite(x)) or np.any((x < 0.0) | (x > 1.0)):
        raise ValueError("Mole fractions must be finite and within [0, 1].")


def bulk_energy_density(x, A, x_alpha, x_beta):
    """
    Calculate the bulk free-energy density of a binary alloy.

    The free energy is represented by a quartic double-well potential:

        f(x) = A * (x - x_alpha)^2 * (x - x_beta)^2

    Parameters
    ----------
    x : float or array_like
        Solute mole fraction, constrained to [0, 1].
        Dimensionless.
    A : float
        Positive bulk free-energy coefficient.
        Units: J/m^3 for a volumetric free-energy density.
    x_alpha : float
        Equilibrium solute mole fraction of the alpha phase.
        Dimensionless.
    x_beta : float
        Equilibrium solute mole fraction of the beta phase.
        Dimensionless.

    Returns
    -------
    float or ndarray
        Bulk free-energy density evaluated at each composition.
        Units: J/m^3 when A is expressed in J/m^3.

    Raises
    ------
    ValueError
        If a composition is nonfinite or outside [0, 1], or if
        the energy coefficient is invalid.

    Notes
    -----
    The intended equilibrium compositions satisfy

        0 <= x_alpha < x_beta <= 1.

    Both equilibrium compositions are minima of the bulk free energy,
    with f(x_alpha) = f(x_beta) = 0.

    This is a phenomenological double-well model, not a
    CALPHAD-derived free-energy description.

    The function does not include gradient-energy contributions.
    """
    if not np.isfinite(A) or A <= 0.0:
        raise ValueError("A must be positive and finite.")
    _validate_composition(x_alpha)
    _validate_composition(x_beta)
    _validate_composition(x)
    if x_alpha >= x_beta:
        raise ValueError("Require x_alpha < x_beta.")
    return A * (x - x_alpha) ** 2 * (x - x_beta) ** 2


def chemical_bulk_derivative(x, A, x_alpha, x_beta):
    """
    Calculate the first composition derivative of bulk free energy.

    The analytical derivative is

        df/dx = 2*A*(x - x_alpha)*(x - x_beta)
                *(2*x - x_alpha - x_beta)

    Parameters
    ----------
    x : float or array_like
        Solute mole fraction in [0, 1]. Dimensionless.
    A : float
        Positive bulk free-energy coefficient.
        Units: J/m^3 for volumetric free energy.
    x_alpha : float
        Equilibrium solute mole fraction of the alpha phase.
    x_beta : float
        Equilibrium solute mole fraction of the beta phase.

    Returns
    -------
    float or ndarray
        First derivative of bulk free-energy density with respect
        to mole fraction.
        Units: J/m^3 when A is expressed in J/m^3.

    Raises
    ------
    ValueError
        If a composition is nonfinite or outside [0, 1], or if
        the energy coefficient is invalid.

    Notes
    -----
    Since mole fraction is dimensionless, differentiation with
    respect to x does not change the energy-density units.

    This derivative represents the bulk contribution to the
    variational chemical potential in the Cahn-Hilliard model.

    The complete variational derivative is

        mu = df/dx - kappa * laplacian(x).

    If a conventional molar chemical-potential difference is needed,
    an appropriate molar-volume conversion is required.
    """
    if A < 0.0:
        raise ValueError("The value of bulk energy coefficient is negative.")
    _validate_composition(x_alpha)
    _validate_composition(x_beta)
    _validate_composition(x)
    if x_alpha >= x_beta:
        raise ValueError("Require x_alpha < x_beta.")
    return 2.0 * A * (x - x_alpha) * (x - x_beta) * (2.0 * x - x_alpha - x_beta)


def chemical_bulk_curvature(x, A, x_alpha, x_beta):
    """
    Calculate the second composition derivative of bulk free energy.

    The analytical curvature is

        d2f/dx2 = 2*A*[
            (2*x - x_alpha - x_beta)^2
            + 2*(x - x_alpha)*(x - x_beta)
        ]

    Parameters
    ----------
    x : float or array_like
        Solute mole fraction in [0, 1]. Dimensionless.
    A : float
        Positive bulk free-energy coefficient.
        Units: J/m^3 for volumetric free energy.
    x_alpha : float
        Equilibrium solute mole fraction of the alpha phase.
    x_beta : float
        Equilibrium solute mole fraction of the beta phase.

    Returns
    -------
    float or ndarray
        Curvature of bulk free-energy density with respect to
        composition.
        Units: J/m^3 when A is expressed in J/m^3.

    Raises
    ------
    ValueError
        If a composition is nonfinite or outside [0, 1], or if
        the energy coefficient is invalid.

    Notes
    -----
    The sign of the curvature determines local thermodynamic
    stability against infinitesimal homogeneous-composition
    perturbations:

        d2f/dx2 > 0 : locally stable
        d2f/dx2 < 0 : spinodally unstable
        d2f/dx2 = 0 : spinodal boundary

    For constant positive mobility and gradient-energy coefficient,
    negative bulk curvature permits growth of sufficiently
    long-wavelength composition fluctuations.

    The curvature does not include gradient-energy contributions.
    """
    if A < 0.0:
        raise ValueError("The value of bulk energy coefficient is negative.")
    _validate_composition(x_alpha)
    _validate_composition(x_beta)
    _validate_composition(x)
    return (
        2.0
        * A
        * ((2.0 * x - x_alpha - x_beta) ** 2 + 2.0 * (x - x_alpha) * (x - x_beta))
    )


def total_mass(x, dx):
    """
    Calculate the discrete composition integral in one dimension.

    The integral is approximated using a cell-centred discretization:

        I_x = dx * sum(x_i)

    Parameters
    ----------
    x : array_like
        One-dimensional array of solute mole fractions.
        Every value must be finite and lie within [0, 1].
    dx : float
        Uniform grid-cell width.
        Units: m for a dimensional spatial grid.

    Returns
    -------
    float
        Discrete integral of solute mole fraction.
        Units: m when dx is expressed in metres.

    Raises
    ------
    ValueError
        If dx is nonfinite or nonpositive, or if any mole fraction
        is nonfinite or outside [0, 1].

    Notes
    -----
    Despite the function name, the returned quantity is a
    composition integral, not mass in kilograms.

    For constant molar volume Vm and cross-sectional area S,
    the total solute amount is

        n_B = (S / Vm) * I_x.

    Under constant molar volume and zero-flux boundary conditions,
    conservation of I_x corresponds to conservation of solute amount.

    This discrete integral is appropriate for uniformly spaced,
    cell-centred finite-volume or finite-difference grids.
    """
    if dx <= 0.0 or not np.isfinite(dx):
        raise ValueError("Grid Spacing in 1D domain is not appropriate.")
    _validate_composition(x)
    return dx * np.sum(x)
