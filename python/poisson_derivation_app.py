import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np

    L = 1.0
    N = 3

    dx = L / (N + 1)

    x = np.linspace(0.0, L, N + 2)

    u_exact = x * (1.0 - x)

    A = np.array([
        [ 2.0, -1.0,  0.0],
        [-1.0,  2.0, -1.0],
        [ 0.0, -1.0,  2.0],
    ])

    b = dx**2 * np.full(N, 2.0)

    u_interior = u_exact[1:-1]

    residual = b - A @ u_interior
    return L, np, u_exact, x


@app.cell
def _(L, np):
    x = np.linspace(0.0, L, 300000+ 2)
    u_exact = x * (1.0 - x)

    return u_exact, x


@app.cell
def _(u_exact, x):
    import matplotlib.pyplot as plt

    plt.figure()
    # Changed 'plt.plt' to 'plt.plot' and '-0-' to '-o-'
    plt.plot(x, u_exact, '-o', label='Exact') 
    plt.legend()
    plt.xlabel('Position')
    plt.ylabel('Exact Solution')
    plt.show()

    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
