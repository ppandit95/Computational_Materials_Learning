import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np

    from python.euler_accuracy import (
        final_time_error,
        observed_order,
    )

    return final_time_error, np, observed_order


@app.cell
def _(final_time_error, np, observed_order):
    dts = np.array([0.2, 0.1, 0.05, 0.025])

    errors = np.array([
        final_time_error(1.0, 2.0, dt, 1.0)
        for dt in dts
    ])

    orders = np.array([
        observed_order(errors[i], errors[i + 1])
        for i in range(len(errors) - 1)
    ])

    print("Time steps:", dts)
    print("Errors:", errors)
    print("Observed orders:", orders)
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
