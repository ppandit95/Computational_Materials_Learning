import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    import matplotlib.pyplot as plt
    import marimo as mo

    from python.allen_cahn_solver import (
        forward_euler_step,
        total_free_energy,
    )

    from python.allen_cahn_stability import (
        critical_timestep,
    )

    return (
        critical_timestep,
        forward_euler_step,
        mo,
        np,
        plt,
        total_free_energy,
    )


@app.cell
def _(critical_timestep, np):
    N = 200
    L = 10.0
    dx = L / N

    M = 1.0
    kappa = 0.1
    W = 1.0

    x = (np.arange(N) + 0.5) * dx

    rng = np.random.default_rng(42)

    eta_initial = (
        0.8 * np.tanh((x - L / 2) / 0.8)
        + 1e-4 * rng.standard_normal(N)
    )

    dt_critical = critical_timestep(
        dx, M, kappa, W
    )

    return M, W, dt_critical, dx, eta_initial, kappa, x


@app.cell
def _(dt_critical):
    dt_critical
    return


@app.cell
def _(mo):
    ratio = mo.ui.slider(start=0.1,stop=1.5,step=0.05,value=0.5,label="Timestep/Critical Timestep") 
    ratio
    return (ratio,)


@app.cell
def _(dt_critical, ratio):
    dt = ratio.value*dt_critical
    dt
    return (dt,)


@app.cell
def _(
    M,
    W,
    dt,
    dx,
    eta_initial,
    forward_euler_step,
    kappa,
    np,
    total_free_energy,
):
    eta = eta_initial.copy()

    energy_history = [
        total_free_energy(eta, dx, kappa, W)
    ]

    max_eta_history = [
        np.max(np.abs(eta))
    ]

    time_history = [0.0]

    divergence_step = None

    for step in range(1, 201):

        eta = forward_euler_step(
            eta, dx, dt, M, kappa, W
        )

        # Detect numerical divergence
        if not np.all(np.isfinite(eta)):
            divergence_step = step
            break

        if np.max(np.abs(eta)) > 10.0:
            divergence_step = step
            break

        # Record diagnostics
        energy_history.append(
            total_free_energy(eta, dx, kappa, W)
        )

        max_eta_history.append(
            np.max(np.abs(eta))
        )

        time_history.append(step * dt)
    return divergence_step, energy_history, eta, max_eta_history, time_history


@app.cell
def _(dt, dt_critical, energy_history, plt, time_history):
    fig_energy, ax_energy = plt.subplots()

    ax_energy.plot(time_history, energy_history)

    ax_energy.set_xlabel("Physical time")
    ax_energy.set_ylabel("Total free energy")
    ax_energy.set_title(
        f"Energy evolution: dt/dt_critical = {dt/dt_critical:.2f}"
    )

    ax_energy.grid(True)

    fig_energy
    return


@app.cell
def _(divergence_step, dt):
    if divergence_step is None:
        print("No divergence detected within 200 steps.")
    else:
        print(f"Divergence detected at step {divergence_step}")
        print(f"Divergence time: {divergence_step * dt:.6f}")
    return


@app.cell
def _(dt, dt_critical, eta, eta_initial, plt, x):
    fig_profile, ax_profile = plt.subplots()

    ax_profile.plot(
        x,
        eta_initial,
        linestyle="--",
        label="Initial profile",
    )

    ax_profile.plot(
        x,
        eta,
        label="Final profile",
    )

    ax_profile.set_xlabel("Position")
    ax_profile.set_ylabel("Order parameter")

    ax_profile.set_title(
        f"Allen-Cahn evolution: dt/dt_critical = "
        f"{dt / dt_critical:.2f}"
    )

    ax_profile.legend()
    ax_profile.grid(True)

    fig_profile
    return


@app.cell
def _(max_eta_history, plt, time_history):
    fig_magnitude, ax_magnitude = plt.subplots()

    ax_magnitude.plot(
        time_history,
        max_eta_history,
        label="Maximum magnitude",
    )

    ax_magnitude.axhline(
        1.0,
        linestyle="--",
        label="Equilibrium bulk magnitude",
    )

    ax_magnitude.set_xlabel("Physical time")
    ax_magnitude.set_ylabel("Maximum absolute order parameter")
    ax_magnitude.set_title("Numerical stability monitoring")

    ax_magnitude.legend()
    ax_magnitude.grid(True)

    fig_magnitude
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
