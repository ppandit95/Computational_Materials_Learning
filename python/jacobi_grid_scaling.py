import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import jacobi_grid_analysis
    import pandas as pd
    import numpy as np
    import matplotlib.pyplot as plt

    return np, pd, plt


@app.cell
def _(pd):
    file = pd.read_csv("/home/pushkar/computational_materials_learning/data/jacobi_grid_scaling.csv",sep=',')
    file.head()
    return (file,)


@app.cell
def _(file, np, plt):
    k_observed = np.array(file['k_observed'])
    n = np.array(file['n'])

    plt.loglog(n,k_observed)
    plt.xlabel("Matrix Size(n)")
    plt.ylabel("Observed No of Iterations too reach convergence")
    plt.show()
    return (n,)


@app.cell
def _(file, n, np, plt):
    k_predicted = np.array(file['k_predicted'])

    plt.loglog(n,k_predicted)
    plt.xlabel("Matrix Size(n)")
    plt.ylabel("Predicted No of Iterations too reach convergence")
    plt.show()
    return (k_predicted,)


@app.cell
def _(file, k_predicted, n, np, plt):
    spectral_radius = np.array(file['rho_theory'])

    plt.loglog(n**2,k_predicted)
    plt.xlabel("Matrix Size(n)")
    plt.ylabel("Theoretical Spectral Radius")
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
