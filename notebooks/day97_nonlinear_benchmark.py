import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    import matplotlib.pyplot as plt
    import pandas as pd

    return np, pd, plt


@app.cell
def _(np, pd, plt):
    file = pd.read_csv('/home/pushkar/computational_materials_learning/data/day97_nonlinear_benchmark.csv')
    plt.figure()
    plt.loglog(file['N'].to_numpy(),file['runtime_median_s'].to_numpy(),'-*')
    plt.title('Runtime Scaling(log-Log Plot)')
    plt.xlabel('Spatial Resolution')
    plt.ylabel('Runtime of nonlinear Solver')
    plt.legend()
    plt.show()
    slope, intercept = np.polyfit(np.log(file['N'].to_numpy()),np.log(file['runtime_median_s'].to_numpy()), 1)
    print(slope)
    return (file,)


@app.cell
def _(file, plt):
    plt.figure()
    plt.loglog(file['dx_m'].to_numpy()[:-1],file['reference_Linf_error'].to_numpy()[:-1],'-*')
    plt.title('Fine Grid Comparison Error (Log Log Plot)')
    plt.xlabel('Spatial Spacing')
    plt.ylabel('Numerical Convergence')
    plt.legend()
    plt.show()

    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
