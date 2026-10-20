import numpy as np

from python.euler_forced import (
    forward_euler_forced,
    exact_forced_decay,
)


def calculate_errors(
    u0: float,
    lam: float,
    source: float,
    T: float,
    dt_values: np.ndarray,
) -> np.ndarray:
    """
    Calculate Forward Euler final-time errors for multiple timesteps.

    Parameters
    ----------
    u0 : float
        Initial value.
    lam : float
        Positive decay coefficient.
    source : float
        Constant forcing term.
    T : float
        Final simulation time.
    dt_values : np.ndarray
        Positive timesteps that divide T into integer step counts.

    Returns
    -------
    np.ndarray
        Absolute final-time errors for each timestep.

    Raises
    ------
    ValueError
        If T is invalid, dt_values is invalid, or a timestep
        does not divide T into an integer number of steps.
    """

    dt_values = np.asarray(dt_values, dtype=float)

    if not np.isfinite(T) or T <= 0:
        raise ValueError("T must be finite and positive.")

    if (
        dt_values.ndim != 1
        or not np.all(np.isfinite(dt_values))
        or np.any(dt_values <= 0)
    ):
        raise ValueError("dt_values must be a 1D array of positive finite values.")

    error_values = []

    for dt in dt_values:

        steps = round(T / dt)

        if steps < 1 or not np.isclose(steps * dt, T):
            raise ValueError("T must be an integer multiple of dt.")

        approx_sol = forward_euler_forced(
            u0=u0,
            lam=lam,
            source=source,
            dt=dt,
            steps=steps,
        )

        exact_sol = exact_forced_decay(
            u0=u0,
            lam=lam,
            source=source,
            times=np.array([T]),
        )

        error = abs(approx_sol[-1] - exact_sol[-1])

        error_values.append(error)

    return np.asarray(error_values)


def observed_orders(
    dt_values: np.ndarray,
    error_values: np.ndarray,
) -> np.ndarray:
    """
    Estimate convergence orders between successive timestep sizes.

    Parameters
    ----------
    dt_values : np.ndarray
        Positive timestep sizes in decreasing order.
    error_values : np.ndarray
        Corresponding positive errors.

    Returns
    -------
    np.ndarray
        Observed convergence orders.
    """

    dt_values = np.asarray(dt_values, dtype=float)
    error_values = np.asarray(error_values, dtype=float)

    if (
        dt_values.ndim != 1
        or error_values.ndim != 1
        or dt_values.size != error_values.size
        or dt_values.size < 2
        or not np.all(np.isfinite(dt_values))
        or not np.all(np.isfinite(error_values))
        or np.any(dt_values <= 0)
        or np.any(error_values <= 0)
        or not np.all(np.diff(dt_values) < 0)
    ):
        raise ValueError("Invalid timestep or error arrays.")

    return np.log(error_values[:-1] / error_values[1:]) / np.log(
        dt_values[:-1] / dt_values[1:]
    )


def profile_workload():
    """Execute a representative Forward Euler profiling workload."""

    forward_euler_forced(
        u0=1.0,
        lam=2.0,
        source=1.0,
        dt=0.1,
        steps=200_000,
    )


if __name__ == "__main__":

    u0 = 1.0
    lam = 2.0
    source = 1.0
    T = 1.0

    dt_values = np.array([0.2, 0.1, 0.05, 0.025])

    errors = calculate_errors(
        u0=u0,
        lam=lam,
        source=source,
        T=T,
        dt_values=dt_values,
    )

    orders = observed_orders(dt_values, errors)

    print("dt       steps     absolute error")

    for dt, error in zip(dt_values, errors):
        steps = round(T / dt)
        print(f"{dt:<8.3f} {steps:<9d} {error:.10e}")

    print("\nObserved convergence orders:")

    for i, order in enumerate(orders):
        print(f"dt {dt_values[i]:.3f} -> " f"{dt_values[i+1]:.3f}: p = {order:.6f}")

    print("\nRunning profiling workload...")
    profile_workload()
