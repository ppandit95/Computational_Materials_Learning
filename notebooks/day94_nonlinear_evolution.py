import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    import matplotlib.pyplot as plt
    from python.nonlinear_diffusion_solver import solve_nonlinear_diffusion
    from python.nonlinear_diffusion_fv import nonlinear_diffusion_rhs

    return np, plt, solve_nonlinear_diffusion


@app.cell
def _(np):
    L = 1.0e-3  # m
    N = 64
    dx = L / N
    D0 = 1.0e-10
    beta = 1.0
    dt = 0.4 * dx**2 / (D0 * (1 + beta))  # Stability condition for Forward Euler
    n_steps = np.array([25,50,100])
    c_initial = np.zeros(N + 2)
    for i in range(N + 1):
        c_initial[i] = 0.5 + 0.2 * np.cos(
            2 * np.pi * (i + 0.5) / N
        )  # Non-uniform initial condition

    return D0, L, N, beta, c_initial, dt, dx, n_steps


@app.cell
def _(D0, beta, c_initial, dt, dx, n_steps, solve_nonlinear_diffusion):
    rows = n_steps.size
    c = [[] for _ in range(rows)]
    # Output: [[], [], []]

    for j,n_step in enumerate(n_steps):
        c[j] = solve_nonlinear_diffusion(c_initial, dx, D0, beta, dt, n_step)
    print(c[1])

    return (c,)


@app.cell
def _(L, N, np):
    pos = (1/L)*np.linspace(0,1,N+2)
    return (pos,)


@app.cell
def _(c, plt, pos):
    plt.figure()
    plt.plot(pos,c[0],'-+',label='NSTEP=25')
    plt.plot(pos,c[1],'_',label='NSTEP=50')
    plt.plot(pos,c[2],'-*',label='NSTEP=100')
    plt.legend()
    plt.xlabel('Position ( x/L)')
    plt.ylabel('Concentration')
    plt.show()
    return


@app.cell
def _(c, c_initial, dx, np):
    mass_25 = dx*np.sum(c[0])
    mass_50 = dx*np.sum(c[1])
    mass_100 = dx*np.sum(c[2])
    mass_0 = dx*np.sum(c_initial)

    diff_25 = abs(mass_25 - mass_0)
    diff_50 = abs(mass_50 - mass_0)
    diff_100 = abs(mass_100 - mass_0)

    print(diff_25,diff_50,diff_100)
    return


@app.cell
def _():
    import marimo as mo

    return (mo,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    As the time advances a sinusoidally varying concentration moves towards constant concentration due to diffusion governing equation although due to varying diffusivity thes effect varies spatially but eventually at infinite timestamp one can expect to get a constant concentration profile as flow of mass is guided by concentration gradient so mass simply redistributes while the total mass of the system remains same
    """)
    return


if __name__ == "__main__":
    app.run()
