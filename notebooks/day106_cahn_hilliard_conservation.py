import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import numpy as np 
    import matplotlib.pyplot as plt
    from python.cahn_hilliard_energy import bulk_energy_density,chemical_bulk_curvature,chemical_bulk_derivative

    return (
        bulk_energy_density,
        chemical_bulk_curvature,
        chemical_bulk_derivative,
        np,
        plt,
    )


@app.cell
def _(
    bulk_energy_density,
    chemical_bulk_curvature,
    chemical_bulk_derivative,
    np,
    plt,
):
    x = np.linspace(0,1,100)
    A = 1
    x_alpha = 0.2
    x_beta = 0.8

    spinodal_indices = np.where(np.diff(np.sign(chemical_bulk_curvature(x,A,x_alpha,x_beta))))
    x_spinodal = x[spinodal_indices]

    fig,ax = plt.subplots(1,3,figsize=(15,4))

    ax[0].plot(x,bulk_energy_density(x,A,x_alpha,x_beta),'--',label="Bulk Enegy Density")
    ax[0].set_xlabel("Composition")
    ax[0].set_ylabel("Gibbs Energy")
    ax[0].legend()

    ax[1].plot(x,chemical_bulk_derivative(x,A,x_alpha,x_beta),'--',label="Bulk Energy Derivative")
    ax[1].axhline(0,color='grey',linestyle=':',linewidth=1)
    ax[1].set_xlabel("Composition")
    ax[1].set_ylabel("Gibbs Energy Derivative")
    ax[1].legend()

    ax[2].plot(x,chemical_bulk_curvature(x,A,x_alpha,x_beta),'--',label="Bulk Energy Curvature")
    ax[2].axhline(0,color='grey',linestyle=':',linewidth=1)
    ax[2].axvline(x_spinodal[0],color='green',linestyle=':',linewidth=1)
    ax[2].axvline(x_spinodal[1],color='green',linestyle=':',linewidth=1)
    ax[2].set_xlabel("Composition")
    ax[2].set_ylabel("Gibbs Energy Curvature")
    ax[2].legend()
    fig.tight_layout()
    fig
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
