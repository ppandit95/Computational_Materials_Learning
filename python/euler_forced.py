import numpy as np


def forward_euler_forced(
    u0: float,
    lam: float,
    source: float,
    dt: float,
    steps: int,
) -> np.ndarray:
    """
    Integrate du/dt = -lam*u + source using Forward Euler.

    Parameters
    ----------
    u0 : float
        Initial value.
    lam : float
        Positive, finite decay coefficient.
    source : float
        Finite constant source term.
    dt : float
        Positive, finite time step.
    steps : int
        Nonnegative integer number of time steps.

    Returns
    -------
    np.ndarray
        Numerical solution including the initial value.

    Raises
    ------
    ValueError
        If any parameter violates its mathematical domain.
    """

    if not np.isfinite(u0) or not np.isfinite(source):
        raise ValueError("u0 and source must be finite")

    if not np.isfinite(lam) or lam <= 0:
        raise ValueError("lam must be positive and finite")

    if not np.isfinite(dt) or dt <= 0:
        raise ValueError("dt must be positive and finite")

    if (
        isinstance(steps, (bool, np.bool_))
        or not isinstance(steps, (int, np.integer))
        or steps < 0
    ):
        raise ValueError("steps must be a nonnegative integer")

    sol = np.zeros(steps + 1, dtype=float)
    sol[0] = u0

    for i in range(steps):
        sol[i + 1] = (1 - lam * dt) * sol[i] + source * dt

    return sol


def exact_forced_decay(
    u0: float,
    lam: float,
    source: float,
    times: np.ndarray,
) -> np.ndarray:
    """
    Calculate the analytical solution of forced linear decay.

    The governing equation is

        du/dt = -lam*u + source

    Parameters
    ----------
    u0 : float
        Initial value.
    lam : float
        Positive, finite decay coefficient.
    source : float
        Finite constant source term.
    times : np.ndarray
        One-dimensional array of finite, nonnegative times.

    Returns
    -------
    np.ndarray
        Analytical solution evaluated at the supplied times.

    Raises
    ------
    ValueError
        If inputs violate their mathematical domains.
    """

    if not np.isfinite(u0) or not np.isfinite(source):
        raise ValueError("u0 and source must be finite")

    if not np.isfinite(lam) or lam <= 0:
        raise ValueError("lam must be positive and finite")

    times = np.asarray(times, dtype=float)

    if times.ndim != 1:
        raise ValueError("times must be a one-dimensional array")

    if not np.all(np.isfinite(times)):
        raise ValueError("times must contain finite values")

    if np.any(times < 0):
        raise ValueError("times must be nonnegative")

    equilibrium = source / lam

    return equilibrium + (u0 - equilibrium) * np.exp(-lam * times)
