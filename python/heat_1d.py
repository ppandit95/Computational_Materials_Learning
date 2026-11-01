import numpy as np


def two_mode_initial_condition(x):
    return np.sin(np.pi * x) + 0.25 * np.sin(3 * np.pi * x)


def solve_heat_ftcs(
    n_interior: int,
    alpha: float,
    dt: float,
    n_steps: int,
    initial_condition=None,
):
    """
    Solve the 1D heat equation using explicit FTCS.

    Parameters
    ----------
    n_interior : int
        Number of interior spatial grid points.
    alpha : float
        Thermal diffusivity in m^2/s.
    dt : float
        Time-step size in seconds.
    n_steps : int
        Number of explicit time steps.
    initial_condition : callable, optional
        Function of x defining the initial temperature distribution.
        If None, defaults to u(x,0) = sin(pi*x).

    Returns
    -------
    x : ndarray
        Spatial grid including boundary nodes.
    u : ndarray
        Temperature field at the final time.
    t_final : float
        Final simulation time in seconds.

    Raises
    ------
    ValueError
        If parameters are invalid or FTCS is unstable.
    ValueError
        If the initial condition shape does not match the spatial grid.
    """
    if n_interior < 1:
        raise ValueError("n_interior must be at least 1.")
    if alpha <= 0:
        raise ValueError("alpha must be positive.")
    if dt <= 0:
        raise ValueError("dt must be positive.")
    if n_steps < 1:
        raise ValueError("n_steps must be at least 1.")

    # Spatial grid
    dx = 1.0 / (n_interior + 1)
    x = np.linspace(0, 1, n_interior + 2)

    # Stability condition for FTCS
    r = alpha * dt / dx**2
    if r > 0.5:
        raise ValueError(f"FTCS is unstable for r={r:.3f} > 0.5.")

    if initial_condition is not None:
        u = initial_condition(x)
    else:
        # Initial condition: u(x,0) = sin(pi*x)
        u = np.sin(np.pi * x)

    if u.shape != x.shape:
        raise ValueError("Initial condition shape must match the spatial grid")

    # Time-stepping loop
    for _ in range(n_steps):
        u_new = u.copy()
        u_new[1:-1] = u[1:-1] + r * (u[2:] - 2 * u[1:-1] + u[:-2])
        u = u_new

    t_final = n_steps * dt
    return x, u, t_final
