import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    from python.euler_forced import forward_euler_forced,exact_forced_decay
    import matplotlib.pyplot as plt

    return exact_forced_decay, forward_euler_forced, np, plt


@app.cell
def _(np):
    u0,lam,s,final_time = 1,2,1,1
    dts = np.array([0.1,0.05])
    steps = np.ceil(final_time*np.ones(dts.size)/dts).astype(int)
    return dts, final_time, lam, s, steps, u0


@app.cell
def _(
    dts,
    exact_forced_decay,
    final_time,
    forward_euler_forced,
    lam,
    np,
    plt,
    s,
    steps,
    u0,
):
    for index, dt in enumerate(dts):
        # Calculate total number of points (steps + 1) for the grid
        num_points = steps[index] + 1
        t_grid = np.linspace(0, final_time, num_points)
    
        analytical_sol = exact_forced_decay(u0, lam, s, t_grid)
    
        approx_sol = forward_euler_forced(u0, lam, s, dt, steps[index])

        plt.figure()

        plt.plot(t_grid,analytical_sol,label=f"Exact-{index}")
        plt.plot(t_grid,approx_sol,label=f"Approx-{index}")
        plt.legend()
        plt.show()



    

    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
