#pragma once

#include <vector>

namespace phasefield
{

    std::vector<double> laplacian_neumann(const std::vector<double> &eta, double dx);
    std::vector<double> allen_cahn_rhs(const std::vector<double> &eta, double dx, double M, double kappa, double W);
    double total_free_energy(const std::vector<double> &eta, double dx, double kappa, double W);
    std::vector<double> forward_euler_step(const std::vector<double> &eta, double dx, double dt, double M, double kappa, double W);
}