from scipy.sparse import diags
from scipy.sparse.linalg import spsolve

import numpy as np


def diagnose_poisson(num_interior: int) -> dict:

    if num_interior < 1:
        raise ValueError("num_interior must be positive")

    length = 1.0
    u_left = 1.0
    u_right = 3.0

    h = length / (num_interior + 1)

    x = np.linspace(0.0, length, num_interior + 2)
    x_interior = x[1:-1]

    forcing = np.pi**2 * np.sin(np.pi * x_interior)

    # Construct corrected RHS
    rhs = forcing.copy()

    rhs[0] += u_left / h**2
    rhs[-1] += u_right / h**2

    # Construct the sparse finite difference matrix
    main_diag = 2.0 * np.ones(num_interior)
    off_diag = -1.0 * np.ones(num_interior - 1)

    # FIX: Corrected parenthesis layout for scaling by / dx**2
    A = diags(
        diagonals=[off_diag, main_diag, off_diag],
        offsets=[-1, 0, 1],
        shape=(num_interior, num_interior),
        format="csr",
    ) / (h**2)

    # Solve for the interior nodes
    u_interior = spsolve(A, rhs)

    u_approx = np.pad(
        u_interior, (1, 1), mode="constant", constant_values=(u_left, u_right)
    )

    u_exact = 1.0 + 2.0 * x_interior + np.sin(np.pi * x_interior)

    algebraic_residual = np.linalg.norm(rhs - A @ u_interior, ord=np.inf)
    discretization_error = np.linalg.norm(u_interior - u_exact, ord=np.inf)
    exact_solution_residual = np.linalg.norm(rhs - A @ u_exact, ord=np.inf)

    return {
        "N": num_interior,
        "h": h,
        "algebraic_residual_linf": algebraic_residual,
        "discretization_error_linf": discretization_error,
        "exact_solution_residual_linf": exact_solution_residual,
    }
