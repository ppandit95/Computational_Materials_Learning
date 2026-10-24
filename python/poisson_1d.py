import numpy as np
from scipy.sparse import diags
from scipy.sparse.linalg import spsolve


def solve_poisson_1d(source, length: float, num_interior: int) -> tuple:
    """
    Solve the one-dimensional Poisson equation with homogeneous Dirichlet boundary conditions.
    """
    # FIX: Changed 'and' to 'or' so it catches regular negative numbers
    if not np.isfinite(length) or length <= 0:
        raise ValueError("Length of the domain is incompatible")
    if num_interior < 1:
        raise ValueError("Specified number of interior points are incompatible")
    if not callable(source):
        raise ValueError("The provided source function is not callable")

    dx = length / (num_interior + 1)
    x = np.linspace(0, length, num_interior + 2)
    x_interior = x[1:-1]

    # Evaluate the source function at interior points
    b = np.asarray(source(x_interior), dtype=float)

    # Construct the sparse finite difference matrix
    main_diag = 2.0 * np.ones(num_interior)
    off_diag = -1.0 * np.ones(num_interior - 1)

    # FIX: Corrected parenthesis layout for scaling by / dx**2
    A = diags(
        diagonals=[off_diag, main_diag, off_diag],
        offsets=[-1, 0, 1],
        shape=(num_interior, num_interior),
        format="csr",
    ) / (dx**2)

    # Solve for the interior nodes
    u_interior = spsolve(A, b)

    # FIX: Properly pad the boundary conditions (u=0 at both ends) into a single array
    u = np.pad(u_interior, (1, 1), mode="constant", constant_values=0.0)

    return x, u
