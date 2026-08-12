import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    # Arrhenius Diffusivity Calculation
    return


@app.cell
def _():
    # Inputs
    D0 = 1.0e-4
    Q = 250000.0
    R = 8.314
    import numpy as np
    import jax.numpy as jnp

    return D0, Q, R, jnp, np


@app.cell
def _():
    import marimo as mo
    T= mo.ui.slider(800,1600,10)
    T
    return


@app.cell
def _(np):
    #Temp = T.value
    #Temp = jnp.linspace(800,1600)
    Temp = np.array([1000,1100,1200,1300])
    return (Temp,)


@app.cell
def _(D0, Q, R, Temp, np):
    D_numpy = D0*np.exp(-Q/(R*Temp))

    return (D_numpy,)


@app.cell
def _(D0, Q, R, Temp, jnp):
    D_jax = D0*jnp.exp(-Q/(R*Temp))

    return (D_jax,)


@app.cell
def _(Temp):
    print(f"Temperature : {Temp}")
    return


@app.cell
def _(D_numpy):
    print(f"Diffusivity vector calculated with numpy : {D_numpy}")
    return


@app.cell
def _(D_jax):
    print(f"Diffusivity vector calculated with JAX  : {D_jax}")
    return


@app.cell
def _(D_jax, D_numpy):
    print(f"Error : {D_numpy-D_jax}")
    return


@app.cell
def _():
    import matplotlib.pyplot as plt

    return (plt,)


@app.cell
def _(D_numpy, Temp, np, plt):
    plt.figure()
    plt.plot(Temp,np.log(D_numpy))
    plt.xlabel("Temperature[K]")
    plt.ylabel("Diffusivity[m2/sec]")
    plt.show()
    return


@app.cell
def _(D_numpy, Temp, plt):
    plt.figure()
    plt.plot(Temp,D_numpy)
    plt.yscale('log')
    plt.xlabel("Temperature[K]")
    plt.ylabel("Diffusivity[m2/sec]")
    plt.show()
    return


@app.cell
def _(D_numpy, np):
    diff = D_numpy[1:] - D_numpy[:-1]
    if np.any(diff<=0):
       print(f"There's some issue at index list : {np.where(diff)} ") 
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
