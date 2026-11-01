import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    import matplotlib.pyplot as plt
    from python.heat_1d import solve_heat_ftcs , two_mode_initial_condition

    return np, plt, solve_heat_ftcs, two_mode_initial_condition


@app.cell
def _(np, solve_heat_ftcs, two_mode_initial_condition):
    n_interior = 31
    alpha = 0.01

    # Calculate a stable dt dynamically (r = 0.4)
    dx = 1.0 / (n_interior + 1)
    r = 0.4
    dt = r * (dx**2) / alpha
    n_steps = 5
    t_initial = 0

    # Call the function with the two-mode initial condition
    x, u, t_final = solve_heat_ftcs(
        n_interior, alpha, dt, n_steps, initial_condition=two_mode_initial_condition
    )

    u_exact = np.exp(-0.1 * t_final * np.pi) * np.sin(np.pi * x) + 0.25 * np.exp(
            -0.1 * t_final * 3 * np.pi
        ) * np.sin(3 * np.pi * x)

    u_initial = np.exp(-0.1 * t_initial * np.pi) * np.sin(np.pi * x) + 0.25 * np.exp(
            -0.1 * t_initial * 3 * np.pi
        ) * np.sin(3 * np.pi * x)

    linf_norm = np.linalg.norm(np.abs(u - u_exact), ord=np.inf)


    return alpha, t_final, u, u_exact, u_initial, x


@app.cell
def _(plt, u, u_exact, u_initial, x):
    plt.figure()
    plt.plot(x,u_initial,'-*',label='Initial')
    plt.plot(x,u,'-o',label='Final_Approx')
    plt.plot(x,u_exact,'-+',label='Final_Exact')
    plt.legend()
    plt.xlabel('Position')
    plt.ylabel('Temperature')
    plt.show()
    return


@app.cell
def _(alpha, np, plt, t_final):
    t_values = np.linspace(0,t_final,200)
    A1 = np.exp(-alpha*np.pi**2*t_values)
    A3 = 0.25*np.exp(-9*alpha*np.pi**2*t_values)
    plt.figure()
    plt.plot(t_values,A1,'-+',label='A1')
    plt.plot(t_values,A3,'-*',label='A3')
    plt.xlabel('Time')
    plt.ylabel('Fourier Modes')
    plt.legend()
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
