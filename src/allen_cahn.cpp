#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "allen_cahn.hpp"

namespace phasefield
{
    std::vector<double> laplacian_neumann(const std::vector<double> &eta, double dx)
    {
        if (dx <= 0.0 || !std::isfinite(dx))
            throw std::invalid_argument("Grid spacing dx must be positive and finite.");
        if (eta.size() < 2)
            throw std::invalid_argument("Eta vector has less than 2 elements wherein laplacian cannot be computed.");
        bool has_nonfinite = std::any_of(eta.begin(), eta.end(), [](double val)
                                         { return !std::isfinite(val); });

        if (has_nonfinite)
        {
            throw std::invalid_argument("Order parameter vector eta contains non-finite values.");
        }
        std::vector<double> laplacian(eta.size(), 0.0);
        for (std::size_t i = 0; i < eta.size(); i++)
        {
            if (i == 0)
                laplacian[i] = (eta[i + 1] - eta[i]) / (dx * dx);
            else if (i == eta.size() - 1)
                laplacian[i] = (eta[i - 1] - eta[i]) / (dx * dx);
            else
                laplacian[i] = (eta[i + 1] - 2 * eta[i] + eta[i - 1]) / (dx * dx);
        }
        return laplacian;
    }
    std::vector<double> allen_cahn_rhs(const std::vector<double> &eta, double dx, double M, double kappa, double W)
    {
        if (!std::isfinite(M) || M < 0.0)
            throw std::invalid_argument("Mobility parameter is either negative or non-finite");
        if (!std::isfinite(kappa) || kappa < 0.0)
            throw std::invalid_argument("Gradient coefficient is either negative or non-finite");
        if (!std::isfinite(W) || W < 0.0)
            throw std::invalid_argument("Barrier Height is either negative or non-finite.");
        std::vector<double> rhs(eta.size(), 0.0);
        std::vector<double> lap(eta.size(), 0.0);
        lap = laplacian_neumann(eta, dx);
        for (std::size_t i = 0; i < eta.size(); i++)
        {
            rhs[i] = M * (kappa * lap[i] - W * eta[i] * (eta[i] * eta[i] - 1));
        }
        return rhs;
    }
    double total_free_energy(const std::vector<double> &eta, double dx, double kappa, double W)
    {
        if (dx < 0.0 || !std::isfinite(dx))
            throw std::invalid_argument("Grid spacing dx must be positive and finite.");
        if (eta.size() < 2)
            throw std::invalid_argument("Eta vector has less than 2 elements wherein laplacian cannot be computed.");
        bool has_nonfinite = std::any_of(eta.begin(), eta.end(), [](double val)
                                         { return !std::isfinite(val); });

        if (has_nonfinite)
        {
            throw std::invalid_argument("Order parameter vector eta contains non-finite values.");
        }
        if (!std::isfinite(kappa) || kappa < 0.0)
            throw std::invalid_argument("Gradient coefficient is either negative or non-finite");
        if (!std::isfinite(W) || W < 0.0)
            throw std::invalid_argument("Barrier Height is either negative or non-finite.");
        double bulk_energy = 0.0;
        double interface_energy = 0.0;
        for (std::size_t i = 0; i < eta.size() - 1; i++)
        {
            bulk_energy += (W / 4) * (eta[i] * eta[i] - 1) * (eta[i] * eta[i] - 1);
            interface_energy += (eta[i + 1] - eta[i]) * (eta[i + 1] - eta[i]);
        }
        bulk_energy = bulk_energy * dx;
        interface_energy = interface_energy * (kappa / (2 * dx));
        return bulk_energy + interface_energy;
    }
    std::vector<double> forward_euler_step(const std::vector<double> &eta, double dx, double dt, double M, double kappa, double W)
    {
        if (!std::isfinite(dt) || dt <= 0.0)
            throw std::invalid_argument("dt Parameter is either negative or non-finite.");
        std::vector<double> eta_evolved(eta.size(), 0.0);
        std::vector<double> rhs = allen_cahn_rhs(eta, dx, M, kappa, W);
        for (std::size_t i = 0; i < eta.size(); i++)
        {
            eta_evolved[i] = eta[i] + dt * rhs[i];
        }
        return eta_evolved;
    }

}