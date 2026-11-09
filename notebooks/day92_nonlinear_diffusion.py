import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import matplotlib.pyplot as plt
    import numpy as np
    from python.nonlinear_diffusion import diffusivity


    return diffusivity, np, plt


@app.cell
def _(np):
    D0 = 1e-14
    c = np.linspace(0,1,1000)
    beta = np.array([0,0.5,1,2])
    D = np.zeros((3,1000))
    return D, D0, beta, c


@app.cell
def _(D, D0, beta, c, diffusivity, np, plt):
    plt.figure()
    for i in range(beta.size):
        D[i] = diffusivity(c,D0,beta[i])
        plt.plot(c,np.log(D[i]),label=f'Beta={beta[i]}')
    plt.legend()
    plt.show()
    return


@app.cell
def _(D0, diffusivity, np, plt):
    def _():
        x = np.linspace(0,1,1000)
        c = 0.5 + 0.4 * np.sin(np.pi*x)
        dcdx = 0.4*np.pi*np.cos(np.pi*x)
        J = - diffusivity(c,D0,1) * dcdx
        plt.figure()
        plt.plot(x,J,'--')
        plt.xlabel('Location')
        plt.ylabel('Mass Flux')
        plt.legend()
        return plt.show()


    _()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
