import numpy as np


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
