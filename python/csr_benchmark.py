import numpy as np


def scaling_exponent(n, time_seconds):
    """Estimate the exponent in time ~n**p.

    Parameters
    ----------
    n : array_like
        Positive problem sizes.
    time_seconds : array_like
        Positive measured execution times

    Returns
    -------
    float
        Least-squares slope of log(time) versus log(n).

    Raises
    ------
    ValueError
        If arrays differ in shape,contain fewer than two values,or contain non-positive values
    """
    if (
        n.size != time_seconds.size
        or n.size < 2
        or time_seconds.size < 2
        or np.any(n < 0)
        or np.any(time_seconds < 0)
    ):
        raise ValueError("The input arguments are incompatible.")
    p, _ = np.polyfit(np.log(n), np.log(time_seconds), 1)
    return p
