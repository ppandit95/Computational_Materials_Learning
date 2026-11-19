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
    data = np.genfromtxt("data/allen_cahn_performance.csv",delimiter=",",names=True)
    return (data,)


@app.cell
def _(data, plt):
    fig,ax = plt.subplots()

    ax.loglog(data["N"],data["Original_s"],'-o',label="Original")
    ax.loglog(data["N"],data["Optimized_s"],'--',label="Optimized")

    ax.set_xlabel("No. of Cells")
    ax.set_ylabel("Runtime for 1000 Steps")
    ax.legend()
    ax.grid(True,which="both")
    fig
    return


@app.cell
def _(data, plt):
    fig1,ax1 = plt.subplots()

    ax1.plot(data["N"],data["Speedup"],'-o')

    ax1.set_xlabel("No. of Cells")
    ax1.set_ylabel("Speedup")
    ax1.legend()
    ax1.grid(True,which="both")
    fig1
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
