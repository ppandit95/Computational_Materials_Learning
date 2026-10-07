import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import pandas as pd

    return (pd,)


@app.cell
def _():
    from pathlib import Path
    import sys

    PROJECT_ROOT = Path.cwd()

    if str(PROJECT_ROOT) not in sys.path:
        sys.path.insert(0, str(PROJECT_ROOT))

    print("PROJECT_ROOT:", PROJECT_ROOT)
    print("project root on sys.path:", str(PROJECT_ROOT) in sys.path)
    return


@app.cell
def _():
    from python.csr_benchmark import scaling_exponent

    return (scaling_exponent,)


@app.cell
def _(pd):
    import numpy as np
    file = pd.read_csv("data/day62_csr_matvec.csv",sep=',')
    print(file.head())
    return file, np


@app.cell
def _(file, np):
    n = np.array(file["n"])
    t = np.array(file["mean_seconds"])
    return n, t


@app.cell
def _():
    import matplotlib.pyplot as plt

    return (plt,)


@app.cell
def _(file, plt):
    fig, ax = plt.subplots()

    ax.loglog(
        file["n"],
        file["mean_seconds"],
        marker="o",
    )

    ax.set_xlabel("Matrix dimension n")
    ax.set_ylabel("Mean matvec time [s]")
    ax.grid(True)

    fig
    return


@app.cell
def _(n, scaling_exponent, t):
    p = scaling_exponent(n,t)
    return (p,)


@app.cell
def _(p):
    print(p)
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
