import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import marimo as mo
    import numpy as np
    import matplotlib.pyplot as plt

    from python.allen_cahn_solver import (
        forward_euler_step,
        total_free_energy,
        allen_cahn_rhs,
    )

    return allen_cahn_rhs, forward_euler_step, mo, np, plt, total_free_energy


@app.cell
def _(mo):
    mo.md(r"""
    #1D Allen-Cahn Simulation

    ##Governing Equation

    $$
    \frac{\partial\eta}{\partial t} = M\left[\kappa\nabla^2\eta-W(\eta^3-\eta)\right]
    $$

    ## Free-Energy Functional

    $$
    F[\eta] = \int_\Omega\left[\frac{W}{4}(\eta^2-1)^2+\frac{\kappa}{2}|\nabla\eta|^2
    \right]dx
    $$

    ##Numerical Method

    - Cell-centered finite differences
    - Homogeneous Neumann Boundary Conditions
    - Forward Euler Time Integration
    - Dimensionless Simulation Parameters
    """)
    return


@app.cell
def _(np):
    N = 200
    L = 10.0
    M = 1.0
    W = 1.0

    dt = 0.005
    nsteps = 2000

    dx = L / N

    x = (np.arange(N) + 0.5) * dx
    return L, M, W, dt, dx, nsteps, x


@app.cell
def _(mo):
    width_slider = mo.ui.slider(start=0.2,stop=2.0,step=0.05,value=0.8,label="Initial Profile Length",show_value=True,)
    kappa_slider = mo.ui.slider(start=0.02,stop=0.5,step=0.01,value=0.1,label="Gradient Coefficient",show_value=True,)
    mo.vstack([width_slider,kappa_slider])
    return kappa_slider, width_slider


@app.cell
def _(L, W, kappa_slider, np, width_slider, x):
    initial_width = width_slider.value
    kappa = kappa_slider.value
    eta_initial = 0.8*np.tanh((x-L/2)/initial_width)
    characteristic_length = np.sqrt(2*kappa/W)
    eta_equilibrium = np.tanh((x-L/2)/characteristic_length)
    return characteristic_length, eta_equilibrium, eta_initial, kappa


@app.cell
def _(eta_equilibrium, eta_initial, plt, x):
    fig_profile,ax_profile = plt.subplots(figsize=(9,5))
    ax_profile.plot(x,eta_initial,label="Initial Profile",linewidth=2,)
    ax_profile.plot(x,eta_equilibrium,linestyle="--",label="Analytical Equilibrium",linewidth=2,)

    ax_profile.set_xlabel("Position,x")
    ax_profile.set_ylabel("Order Parameter")
    ax_profile.set_title("Allen-Cahn Interface Profiles")

    ax_profile.legend()
    ax_profile.grid(alpha=0.3)
    fig_profile.tight_layout()
    fig_profile
    return


@app.cell
def _(W, characteristic_length, dx, kappa, mo, np):
    interface_width = (2*characteristic_length*np.arctanh(0.8))
    sigma_exact = (2*np.sqrt(2*kappa*W)/3)
    mo.md(f"""
    ### Analytical Interface Properties

    | Quantity | Value |
    | --- | ---: |
    | Grid Spacing | {dx:.6f} |
    | Gradient Coefficient | {kappa:.6f} | 
    | Characteristic Length | {characteristic_length:.6f} |
    | 10-90% interface width | {interface_width:.6f} | 
    |Interfacial Energy | {sigma_exact:.6f} |
    """)
    return (sigma_exact,)


@app.cell
def _(M, W, dt, dx, kappa, mo):
    dt_max = 2/(M*(4*kappa/dx**2 + 2*W))
    stable_dt = dt<= dt_max
    mo.md(f"""
    ###Timestep Stability

    - Prescribed Timestep: {dt:.6f}
    - Estimated Maximum timestep: {dt_max:.6f}
    - Timestep Ratio: {dt/dt_max:.4f}
    - Status: {"🟢 Within stability estimate" if stable_dt else "🔴 Exceeds stability estimate"}""")
    return (stable_dt,)


@app.cell
def _(
    M,
    W,
    dt,
    dx,
    eta_initial,
    forward_euler_step,
    kappa,
    mo,
    np,
    nsteps,
    stable_dt,
    total_free_energy,
):
    mo.stop(not stable_dt,mo.md("**Reduce the gradient coefficient or timestep before running.**"))
    eta_current = eta_initial.copy()

    energy_history = np.zeros(nsteps+1)
    time_history = np.arange(nsteps+1)*dt

    energy_history[0] = total_free_energy(eta_current,dx,kappa,W)

    for step in range(1,nsteps+1):
        eta_current = forward_euler_step(eta_current,dx,dt,M,kappa,W)
        energy_history[step] = total_free_energy(eta_current,dx,kappa,W)
        eta_final = eta_current.copy()

    return energy_history, eta_final, time_history


@app.cell
def _(dt, eta_equilibrium, eta_final, eta_initial, nsteps, plt, x):
    fig_relaxation, ax_relaxation = plt.subplots(
        figsize=(9, 5)
    )

    ax_relaxation.plot(
        x,
        eta_initial,
        label="Initial condition",
        linewidth=2,
    )

    ax_relaxation.plot(
        x,
        eta_final,
        label="Numerical final profile",
        linewidth=2,
    )

    ax_relaxation.plot(
        x,
        eta_equilibrium,
        linestyle="--",
        label="Analytical equilibrium",
        linewidth=2,
    )

    ax_relaxation.set_xlabel("Position, x")
    ax_relaxation.set_ylabel("Order parameter")
    ax_relaxation.set_title(
        f"Interface Relaxation at t = {nsteps * dt:.2f}"
    )

    ax_relaxation.legend()
    ax_relaxation.grid(alpha=0.3)

    fig_relaxation.tight_layout()

    fig_relaxation

    return


@app.cell
def _(energy_history, plt, sigma_exact, time_history):
    fig_energy,ax_energy = plt.subplots(figsize=(9,5))

    ax_energy.plot(time_history,energy_history,linewidth=2,label="Numerical Free Energy")

    ax_energy.axhline(sigma_exact,linestyle="--",label="Analytical interfacial energy")

    ax_energy.set_xlabel("Time")
    ax_energy.set_ylabel("Total Free Energy")
    ax_energy.set_title("Allen-Cahn Free Energy Dissipation")

    ax_energy.legend()
    ax_energy.grid(alpha=0.3)
    fig_energy.tight_layout()
    fig_energy
    return


@app.cell
def _(
    M,
    W,
    allen_cahn_rhs,
    dx,
    energy_history,
    eta_equilibrium,
    eta_final,
    kappa,
    mo,
    np,
    sigma_exact,
):
    energy_changes = np.diff(energy_history)

    energy_monotonic = np.all(energy_changes <= 1e-10)

    profile_error = np.max(np.abs(eta_final - eta_equilibrium))
    relative_energy_error = (abs(energy_history[-1] - sigma_exact) / sigma_exact)

    final_residual = np.max(np.abs(allen_cahn_rhs(eta_final,dx,M,kappa,W)))

    mo.md(f"""
    ## Scientific Verification

    | Metric | Value | 
    | --- | ---: |
    | initial Free Energy | {energy_history[0]:.8f} |
    | Final Free Energy | {energy_history[-1]:.8f} | 
    | Analytical Interfacial Energy | {sigma_exact:.8f} |
    | Monotonic Energy Decrease | {energy_monotonic} | 
    | Maximum Profile Error | {profile_error:.6e} |
    | Relative Energy Error | {relative_energy_error:.6e} |
    | Final Equilibrium Residual | {final_residual:.6e} |
    """)
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
