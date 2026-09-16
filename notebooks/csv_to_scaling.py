import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import pandas as pd

    return (pd,)


@app.cell
def _():
    import matplotlib.pyplot as plt

    return (plt,)


@app.cell
def _():
    import numpy as np

    return (np,)


@app.cell
def _(pd):
    csv_io_data = pd.read_csv("/home/pushkar/computational_materials_learning/benchmarks/csv_io_scaling.csv")
    return (csv_io_data,)


@app.cell
def _(csv_io_data):
    #part 1 : Show the Measured Table
    csv_io_data
    return


@app.cell
def _(csv_io_data):
    from python.benchmark_analysis import compute_scaling_ratios
    size_ratios, time_ratios = compute_scaling_ratios(csv_io_data["Size"],csv_io_data["Duration(s)"])

    size_ratios, time_ratios
    return


@app.cell
def _(csv_io_data, np):
    Size = np.log(csv_io_data["Size"])
    return (Size,)


@app.cell
def _(csv_io_data, np):
    Duration = np.log(csv_io_data["Duration(s)"])
    return (Duration,)


@app.cell
def _(Duration, Size, plt):
    plt.figure()
    plt.plot(Duration,Size)
    plt.xlabel("Elapsed Time(s)")
    plt.ylabel("No. of CSV Rows")
    plt.title("CSV output scaling in Log-Log Space")
    plt.show()
    return


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    "\"\"#Log Log Scaling Interpretation"\"\"
    """)
    return


@app.cell
def _():
    import marimo as mo

    return (mo,)


if __name__ == "__main__":
    app.run()
