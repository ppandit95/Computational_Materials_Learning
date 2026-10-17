import numpy as np

from .euler_decay import forward_euler_decay, exact_decay


def final_time_error(u0: float, lam: float, dt: float, final_time: float) -> float:
    """
    Calculate the final-time error of Forward Euler.

    Parameters
    ----------
    u0 : float
        Initial value.
    lam : float
        Positive decay coefficient, in inverse-time units.
    dt : float
        Positive time-step size.
    final_time : float
        Positive final physical time.

    Returns
    -------
    float
        Absolute error between the numerical and analytical
        solutions at final_time.

    Raises
    ------
    ValueError
        If dt or final_time is invalid, or if final_time
        is not divisible by dt.
    """

    if not np.isfinite(dt) or dt <= 0:
        raise ValueError("dt must be positive and finite")

    if not np.isfinite(final_time) or final_time <= 0:
        raise ValueError("final_time must be positive and finite")

    steps = round(final_time / dt)

    if not np.isclose(steps * dt, final_time):
        raise ValueError("Final time must be divisible by dt")

    u_approx = forward_euler_decay(u0, lam, dt, steps)

    u_exact = exact_decay(u0, lam, np.array([final_time]))

    return float(abs(u_approx[-1] - u_exact[-1]))


def observed_order(error_coarse: float, error_fine: float) -> float:
    """
    Estimate temporal convergence order for factor-two refinement.

    Parameters
    ----------
    error_coarse : float
        Error calculated using the coarser time step.
    error_fine : float
        Error calculated using half the coarse time step.

    Returns
    -------
    float
        Observed temporal convergence order.

    Raises
    ------
    ValueError
        If either error is nonfinite or nonpositive.

    Notes
    -----
    Assumes dt_fine = dt_coarse / 2.
    """

    if (
        not np.isfinite(error_coarse)
        or not np.isfinite(error_fine)
        or error_coarse <= 0
        or error_fine <= 0
    ):
        raise ValueError("Errors must be positive and finite")

    return float(np.log(error_coarse / error_fine) / np.log(2.0))
