import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import pandas as pd
    import numpy as np
    import matplotlib.pyplot as plt

    return np, pd, plt


@app.cell
def _(np, pd, plt):
    #Plot A : Computational Scaling
    df = pd.read_csv('data/day76_euler_cost.csv',sep=',')
    N = np.array(df['steps'])
    t_compute = np.array(df['median_seconds'])

    plt.figure()
    plt.loglog(N,t_compute)
    plt.xlabel('Median Compute Time(secs)')
    plt.ylabel('Timesteps in Forward Euler Approxitation')
    plt.title('Scaling Characteristics(Log-Log Scale)')
    plt.savefig('data/day76_scaling_characteristics.png')
    plt.show()

    return df, t_compute


@app.cell
def _(df, np, plt, t_compute):
    #Plot B : Accuracy Vs Computational Cost
    Error = np.array(df['absolute_error'])
    plt.figure()
    plt.loglog(t_compute,Error)
    plt.xlabel('Median Compute Time(secs)')
    plt.ylabel('Absolute Error of Approximate Solution')
    plt.savefig('data/day76_accuracyVScomputational_cost.png')
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
