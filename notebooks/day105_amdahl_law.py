import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np
    import matplotlib.pyplot as plt

    return np, plt


@app.cell
def _(np):
    s = np.linspace(1.0,100.0,500)
    return (s,)


@app.cell
def _(plt, s):
    fig, ax = plt.subplots()

    for p in [0.1,0.3,0.6,0.9]:
        speedup = 1.0 / ((1.0 - p) + p/s)
        ax.plot(s,speedup,label=f"p={p}")
    ax.set_xlabel("Component Speedup")
    ax.set_ylabel("Overall Program Speedup")
    ax.legend()
    ax.grid(True)

    fig
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
