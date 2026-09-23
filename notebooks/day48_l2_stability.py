import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import pandas as pd

    return (pd,)


@app.cell
def _(pd):
    file = pd.read_csv("/home/pushkar/computational_materials_learning/data/l2_stability.csv")
    return (file,)


@app.cell
def _(file):
    file.head()
    return


@app.cell
def _(file):
    import numpy as np
    naive = np.array(file["naive"])
    return naive, np


@app.cell
def _(file, np):
    stable = np.array(file["stable"])
    ref = np.array(file["reference"])
    return ref, stable


@app.cell
def _(ref, stable):
    e_rel = abs(stable-ref)/abs(ref)
    return


@app.cell
def _(file, np):
    exponent = np.array(file["exponent"])
    return (exponent,)


@app.cell
def _(exponent, naive):
    import matplotlib.pyplot as plt
    plt.plot(exponent,naive)
    plt.yscale('log')
    plt.xlabel('Exponent p')
    plt.ylabel('Naive L2 Norm Implementation')
    plt.show()
    return (plt,)


@app.cell
def _(exponent, plt, stable):
    plt.figure()
    plt.plot(exponent,stable)
    plt.yscale('log')
    plt.xlabel('Exponent p')
    plt.ylabel('Stable L2 Norm Implementation')
    plt.title('L2 norm stability across floating-point magnitudes')
    plt.show()
    return


@app.cell
def _(file):
    import numpy as np

    print(file[["naive", "stable", "reference"]].describe())

    print("Naive inf:",
          np.isinf(file["naive"]).sum())

    print("Stable inf:",
          np.isinf(file["stable"]).sum())

    print("Reference inf:",
          np.isinf(file["reference"]).sum())

    print("Naive zero:",
          (file["naive"] == 0.0).sum())

    print("Stable zero:",
          (file["stable"] == 0.0).sum())
    return (np,)


@app.cell
def _(df, file, plt):
    fig, ax = plt.subplots()

    ax.plot(
        file["exponent"],
        file["log10_reference"],
        label="Analytical reference",
    )

    ax.plot(
        file["exponent"],
        df["log10_naive"],
        label="Naive L2",
    )

    ax.plot(
        df["exponent"],
        df["log10_stable"],
        label="Stable L2",
    )

    ax.set_xlabel("Exponent p")
    ax.set_ylabel("log10(L2 norm)")
    ax.set_title(
        "L2 norm stability across floating-point magnitudes"
    )
    ax.legend()

    fig
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
