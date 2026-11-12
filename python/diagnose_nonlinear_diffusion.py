import numpy as np
import pandas as pd

from pathlib import Path

from nonlinear_diffusion_solver import solve_nonlinear_diffusion


def diagnose_nonlinear_diffusion_exp1():
    """
    Diagnose mass conservation and concentration bounds for
    different explicit time-step factors.

    Uses 64 finite-volume cells and zero-flux boundaries.
    """

    # Material parameters
    D0 = 1.0e-10
    L = 1.0e-3
    N = 64
    beta = 1.0

    dx = L / N
    n_steps = 100

    # Cell-centred initial concentration
    i = np.arange(N)

    c_initial = 0.5 + 0.2 * np.cos(2 * np.pi * (i + 0.5) / N)

    # Conservative stability-limit reference
    dt_ref = dx**2 / (2 * D0 * (1 + beta))

    factors = np.array([0.25, 0.5, 0.9, 1.1])

    initial_mass = np.sum(c_initial) * dx

    results = []

    for factor in factors:

        dt = factor * dt_ref

        try:
            c_final = solve_nonlinear_diffusion(
                c_initial,
                dx,
                D0,
                beta,
                dt,
                n_steps,
            )

            final_mass = np.sum(c_final) * dx

            mass_error = abs(final_mass - initial_mass)

            min_c = np.min(c_final)
            max_c = np.max(c_final)

            finite = bool(np.all(np.isfinite(c_final)))

            status = "completed"

        except ValueError:

            mass_error = np.nan
            min_c = np.nan
            max_c = np.nan

            finite = pd.NA

            status = "rejected"

        results.append(
            {
                "factor": factor,
                "dt": dt,
                "mass_error": mass_error,
                "min_c": min_c,
                "max_c": max_c,
                "finite": finite,
                "status": status,
            }
        )

    df = pd.DataFrame(results)

    output_dir = Path("data")
    output_dir.mkdir(parents=True, exist_ok=True)

    df.to_csv(
        output_dir / "day96_stability.csv",
        index=False,
    )

    print("\nExperiment 1: Stability diagnostics")
    print(df.to_string(index=False))

    return df


def diagnose_nonlinear_diffusion_exp2():
    """
    Compare nonlinear diffusion solutions at a common physical
    final time using different stable time-step factors.
    """

    D0 = 1.0e-10
    L = 1.0e-3
    beta = 1.0
    N = 64

    dx = L / N

    i = np.arange(N)

    c_initial = 0.5 + 0.2 * np.cos(2 * np.pi * (i + 0.5) / N)

    factors = np.array([0.25, 0.5, 0.9])

    dt_ref = dx**2 / (2 * D0 * (1 + beta))

    t_final = 100 * dt_ref

    solutions = []

    results = []

    for factor in factors:

        dt_target = factor * dt_ref

        n_steps = int(np.ceil(t_final / dt_target))

        dt_actual = t_final / n_steps

        c_final = solve_nonlinear_diffusion(
            c_initial,
            dx,
            D0,
            beta,
            dt_actual,
            n_steps,
        )

        solutions.append(c_final)

        results.append(
            {
                "factor": factor,
                "dt_actual": dt_actual,
                "n_steps": n_steps,
                "final_time": n_steps * dt_actual,
            }
        )

    # Finest temporal resolution as reference
    c_reference = solutions[0]

    errors = [np.max(np.abs(c - c_reference)) for c in solutions]

    for result, error in zip(results, errors):
        result["Linf_difference"] = error

    df = pd.DataFrame(results)

    print("\nExperiment 2: Common-final-time comparison")
    print(df.to_string(index=False))

    return df


if __name__ == "__main__":

    diagnose_nonlinear_diffusion_exp1()

    diagnose_nonlinear_diffusion_exp2()
