import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    from python.heat_1d import solve_heat_ftcs
    import matplotlib.pyplot as plt



    return np, plt, solve_heat_ftcs


@app.cell
def _(np, solve_heat_ftcs):
    # 1. Define configurations to run sequentially
    n_interior_list = [15, 31, 63, 127]
    alpha = 0.1
    r = 0.4
    tf = 0.1

    error_val = []
    dx_val = []

    # 2. Collect errors across all resolutions
    for n_interior in n_interior_list:
        dx = 1.0 / (n_interior + 1)
        t_target = r * (dx**2) / alpha
        n_steps = int(np.ceil(tf / t_target))
        dt = tf / n_steps

        x, u_numerical, t_actual = solve_heat_ftcs(n_interior, alpha, dt, n_steps)

        # Calculate exact solution and store max-norm error
        u_exact = np.exp(-alpha * t_actual * np.pi**2) * np.sin(np.pi * x)
        error = np.linalg.norm(u_numerical - u_exact, np.inf)

        error_val.append(error)
        dx_val.append(dx)

        # 3. Calculate spatial convergence rates: log2(error_new / error_old) / log2(dx_new / dx_old)
        # Since dx cuts roughly in half each step, log2(error_old / error_new) should be close to 2.0
    error_val = np.array(error_val)
    dx_val = np.array(dx_val)
    return dx_val, error_val


@app.cell
def _(dx_val, error_val, plt):
    C = error_val[0] / dx_val[0]**2

    plt.figure()
    plt.loglog(dx_val,error_val,'o',label='FTCS')
    plt.loglog(dx_val,2.0*dx_val**2,'--',label='Reference')
    plt.xlabel('Grid Spacing')
    plt.ylabel('Error')
    plt.legend()
    plt.show()
    return


@app.cell
def _(dx_val, error_val, np):
    p = np.log2(error_val[1:] / error_val[:-1]) / np.log2(dx_val[1:] / dx_val[:-1])

    return (p,)


@app.cell
def _(p):
    p
    return


@app.cell
def _(dx_val):
    dx_val
    return


@app.cell
def _(dx_val, np, p, plt):
    plt.figure()
    plt.plot(dx_val[1:],p,'o',label='FTCS')
    plt.plot(dx_val[1:],2*np.ones(p.size),'--',label='Reference')
    plt.xlabel('Grid Spacing')
    plt.ylabel('Convergence Order')
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
