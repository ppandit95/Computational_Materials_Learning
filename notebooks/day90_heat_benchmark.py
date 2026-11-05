import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import matplotlib.pyplot as plt
    import pandas as pd

    return pd, plt


@app.cell
def _(pd):
    file = pd.read_csv('/home/pushkar/computational_materials_learning/data/heat_1d_benchmark.csv')
    x = file['n_interior'].to_numpy()
    y = file['runtime_seconds'].to_numpy()
    return file, x, y


@app.cell
def _(plt, x, y):
    plt.figure()
    plt.loglog(x,y,'o',label='FTCS')
    plt.loglog(x,x*x,'--',label='Reference')
    plt.xlabel('Spatial Resolution')
    plt.ylabel('Runtime')
    plt.legend()
    plt.show()
    return


@app.cell
def _(file, plt, y):
    error = file['error_linf'].to_numpy()
    plt.figure()
    plt.loglog(y,error,'o',label='FTCS')
    plt.loglog(y,y**(-2/3),'--',label='Reference')
    plt.xlabel('Runtime')
    plt.ylabel('Error')
    plt.legend()
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
