import numpy as np

from python.allen_cahn_energy import (
    double_well_energy,
    double_well_derivative,
)


def laplacian_neumann(eta, dx):
    eta_padded = np.pad(eta, pad_width=1, mode="edge")
    return (eta_padded[2:] - 2 * eta_padded[1:-1] + eta_padded[:-2]) / dx**2


def allen_cahn_rhs(eta, dx, M, kappa, W):
    laplacian = laplacian_neumann(eta, dx)
    df_deta = double_well_derivative(eta, W)

    return M * (kappa * laplacian - df_deta)


def forward_euler_step(eta, dx, dt, M, kappa, W):
    rhs = allen_cahn_rhs(eta, dx, M, kappa, W)

    return eta + dt * rhs


def total_free_energy(eta, dx, kappa, W):
    bulk_energy = dx * np.sum(double_well_energy(eta, W))

    gradient_energy = (kappa / (2 * dx)) * np.sum(np.diff(eta) ** 2)

    return bulk_energy + gradient_energy
