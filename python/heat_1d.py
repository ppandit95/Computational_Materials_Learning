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
    Solve the one-dimensional heat equation on the unit interval using FTCS.

    The solver advances ``u_t = alpha * u_xx`` on ``0 <= x <= 1``. The
    reference problem uses homogeneous Dirichlet boundary values and the
    default initial condition ``sin(pi * x)``. A vectorized callable such as
    :func:`two_mode_initial_condition` may be supplied instead; it must
    accept the complete spatial grid and return one value at every grid node.

    Parameters
    ----------
    n_interior : int
        Number of interior spatial grid points (dimensionless count).
    alpha : float
        Thermal diffusivity in m^2/s.
    dt : float
        Time-step size in seconds.
    n_steps : int
        Number of explicit time steps (dimensionless non-negative count).
    initial_condition : callable, optional
        Vectorized function of the dimensionless coordinates ``x`` in
        ``[0, 1]`` defining ``u(x, 0)``. It must return an array with
        ``n_interior + 2`` entries, including the two boundary values. If
        omitted, the supported reference condition ``u(x, 0) = sin(pi*x)``
        is used. The callable's endpoint values are retained by the update;
        use zero endpoints for the homogeneous-Dirichlet reference problem.

    Notes
    -----
    With ``dx = 1 / (n_interior + 1)``, explicit FTCS is stable only when
    ``r = alpha * dt / dx**2 <= 0.5``. The method rejects larger values of
    ``r`` before stepping.

    Returns
    -------
    x : ndarray
        Dimensionless spatial grid on ``[0, 1]``, including boundary nodes.
    u : ndarray
        Temperature field at ``t_final`` in the same units as the initial
        condition.
    t_final : float
        Final simulation time in seconds.

    Raises
    ------
    ValueError
        If ``n_interior < 1``, ``alpha <= 0``, ``dt <= 0``,
        ``n_steps < 0``, or the FTCS stability restriction is violated.
    ValueError
        If the initial condition does not match the spatial-grid shape.
    """
    if n_interior < 1:
        raise ValueError("n_interior must be at least 1.")
    if alpha <= 0:
        raise ValueError("alpha must be positive.")
    if dt <= 0:
        raise ValueError("dt must be positive.")
    if n_steps < 0:
        raise ValueError("n_steps must be non-negative.")

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
