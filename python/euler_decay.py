import numpy as np
import math


def forward_euler_decay(u0: float, lam: float, dt: float, steps: int) -> np.ndarray:
    """
    Integrate du/dt = -lam*u using Forward Euler.

    Parameters
    ----------
    u0 : float
        Initial value.
    lam : float
        Positive decay coefficient, in inverse-time units.
    dt : float
        Positive time-step size.
    steps : int
        Number of time steps.

    Returns
    -------
    np.ndarray
        Solution including the initial condition, shape (steps + 1,).

    Raises
    ------
    ValueError
        If the numerical inputs are invalid.

    Notes
    -----
    Forward Euler is asymptotically stable for
    0 < lam*dt < 2.
    """
    # 1. Added 'raise' keyword to actually throw the error
    if dt <= 0 or lam <= 0 or steps < 0:
        raise ValueError("Provided inputs must be positive numbers.")

    # 2. Initialized the solution array
    sol = np.zeros(steps + 1)

    sol[0] = u0
    for i in range(1, steps + 1):
        sol[i] = (1 - lam * dt) * sol[i - 1]

    return sol


def exact_decay(u0: float, lam: float, time: np.ndarray) -> np.ndarray:
    """
    Evaluate the analytical solution of du/dt = -lam*u

    Parameters
    ----------
    u0: float
        Initial value
    lam: float
        Positive Decay coefficient in inverse-time units
    time: np.ndarray
        Non-negative physical time

    Returns
    -------
    np.ndarray
        Analytical solution evaluated at the supplied time

    Raises
    ------
    ValueError
        If the inputs voilate the physical or numerical constraints.
    """
    if (
        np.any(time < 0.0)
        or lam <= 0
        or math.isnan(u0)
        or not np.all(np.isfinite(time))
    ):
        raise ValueError("The inputs provided are in compatible.")
    return u0 * np.exp(-lam * time)
