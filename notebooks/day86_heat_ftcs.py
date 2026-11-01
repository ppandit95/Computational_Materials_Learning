import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    import matplotlib.pyplot as plt
    from python.heat_1d import solve_heat_ftcs

    return np, plt, solve_heat_ftcs


@app.cell
def _(np, solve_heat_ftcs):
    N = 31
    alpha = 0.1
    r = 0.4
    t_steps = 32
    dx = 1/(N+1)
    dt = r*dx**2 / alpha
    x , u , t_final = solve_heat_ftcs(N,alpha,dt,t_steps)
    u_exact = np.exp(-0.1*t_final*np.pi)*np.sin(np.pi*x)
    linf_norm = np.linalg.norm(np.abs(u-u_exact), ord=np.inf)
    return linf_norm, t_final, u, u_exact, x


@app.cell
def _(linf_norm, t_final):
    print(t_final,linf_norm)
    return


@app.cell
def _(plt, u, u_exact, x):
    plt.plot(x,u,label='FTCS solution at t_final')
    plt.plot(x,u_exact,label='Exact Solution at t_final')
    plt.xlabel('Position')
    plt.ylabel('Temperature Field')
    plt.legend()
    plt.show()
    return


@app.cell
def _(plt, u, u_exact, x):
    plt.figure()
    plt.plot(x,u,'-o',label='FTCS solution at t_final')
    plt.plot(x,u_exact,'-+',label='Exact Solution at t_final')
    plt.xlabel('Position')
    plt.ylabel('Temperature Field')
    plt.legend()
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
