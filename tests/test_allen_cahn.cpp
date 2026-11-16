#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "allen_cahn.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

// ============================================================
// TEST 1: Constant-field Laplacian
// ============================================================

TEST_CASE("Check for constant field Laplacian")
{
    const std::vector<double> eta(5, 0.5);

    const auto laplacian =
        phasefield::laplacian_neumann(eta, 1.0);

    REQUIRE(laplacian.size() == eta.size());

    for (const double value : laplacian)
    {
        CHECK(value == doctest::Approx(0.0));
    }
}

// ============================================================
// TEST 2: Linear-field Neumann boundary stencil
// ============================================================

TEST_CASE("Check for linear field boundary stencil")
{
    const std::vector<double> eta{1.0, 2.0, 3.0};
    const std::vector<double> expected{1.0, 0.0, -1.0};

    const auto laplacian =
        phasefield::laplacian_neumann(eta, 1.0);

    REQUIRE(laplacian.size() == expected.size());

    for (std::size_t i = 0; i < expected.size(); ++i)
    {
        CAPTURE(i);

        CHECK(laplacian[i] == doctest::Approx(expected[i]));
    }
}

// ============================================================
// TEST 3: Stationarity of uniform pure phases
// ============================================================

TEST_CASE("Check for uniform pure phases")
{
    for (const double phase : {-1.0, 1.0})
    {
        CAPTURE(phase);

        const std::vector<double> eta(5, phase);

        const auto eta_new =
            phasefield::forward_euler_step(
                eta, 0.05, 0.005, 1.0, 0.1, 1.0);

        REQUIRE(eta_new.size() == eta.size());

        for (std::size_t i = 0; i < eta.size(); ++i)
        {
            CHECK(eta_new[i] == doctest::Approx(eta[i]));
        }
    }
}

// ============================================================
// TEST 4: Energy dissipation during time evolution
// ============================================================

TEST_CASE("Check for energy dissipation")
{
    constexpr std::size_t N = 200;
    constexpr std::size_t nsteps = 100;

    constexpr double L = 10.0;
    constexpr double dx = L / N;
    constexpr double M = 1.0;
    constexpr double W = 1.0;
    constexpr double kappa = 0.1;
    constexpr double dt = 0.005;
    constexpr double tolerance = 1e-10;

    std::vector<double> eta(N, 0.0);
    std::vector<double> energy_history(nsteps + 1, 0.0);

    // Generate cell-centred initial condition
    for (std::size_t i = 0; i < N; ++i)
    {
        const double x =
            (static_cast<double>(i) + 0.5) * dx;

        eta[i] =
            0.8 * std::tanh((x - L / 2.0) / 0.8);
    }

    // Calculate initial free energy
    energy_history[0] =
        phasefield::total_free_energy(
            eta, dx, kappa, W);

    // Forward Euler time integration
    for (std::size_t step = 1; step <= nsteps; ++step)
    {
        eta = phasefield::forward_euler_step(
            eta, dx, dt, M, kappa, W);

        energy_history[step] =
            phasefield::total_free_energy(
                eta, dx, kappa, W);
    }

    // Check that all energies are finite
    for (const double energy : energy_history)
    {
        REQUIRE(std::isfinite(energy));
    }

    // Check monotonic energy dissipation
    for (std::size_t step = 1; step <= nsteps; ++step)
    {
        CAPTURE(step);

        CHECK(
            energy_history[step] - energy_history[step - 1] <= tolerance);
    }

    // Check that physical relaxation occurred
    CHECK(
        energy_history.back() < energy_history.front());
}

// ============================================================
// TEST 5: Invalid grid spacing
// ============================================================

TEST_CASE("Reject invalid grid spacing")
{
    const std::vector<double> eta{0.0, 0.5, 1.0};

    CHECK_THROWS_AS(
        phasefield::laplacian_neumann(eta, 0.0),
        std::invalid_argument);

    CHECK_THROWS_AS(
        phasefield::laplacian_neumann(eta, -0.1),
        std::invalid_argument);
}

// ============================================================
// TEST 7: Mobility scaling
// ============================================================

TEST_CASE("Verify mobility scaling")
{
    const std::vector<double> eta{0.2, 0.5, 0.8};

    constexpr double dx = 0.1;
    constexpr double kappa = 0.1;
    constexpr double W = 1.0;

    const auto rhs1 =
        phasefield::allen_cahn_rhs(
            eta, dx, 1.0, kappa, W);

    const auto rhs2 =
        phasefield::allen_cahn_rhs(
            eta, dx, 2.0, kappa, W);

    REQUIRE(rhs1.size() == eta.size());
    REQUIRE(rhs2.size() == eta.size());

    for (std::size_t i = 0; i < eta.size(); ++i)
    {
        CAPTURE(i);

        CHECK(
            rhs2[i] == doctest::Approx(2.0 * rhs1[i]));
    }
}

// ============================================================
// TEST 8: Analytical discrete free-energy verification
// ============================================================

TEST_CASE("Verify discrete total free energy")
{
    const std::vector<double> eta{-1.0, 0.0, 1.0};

    constexpr double dx = 0.5;
    constexpr double kappa = 0.1;
    constexpr double W = 2.0;

    const double energy =
        phasefield::total_free_energy(
            eta, dx, kappa, W);

    CHECK(energy == doctest::Approx(0.45));
}

// ============================================================
// TEST 9: Forward Euler input preservation and accuracy
// ============================================================

TEST_CASE("Verify Forward Euler update and input preservation")
{
    const std::vector<double> eta{0.2, 0.5, 0.8};
    const std::vector<double> eta_original = eta;

    constexpr double dx = 0.1;
    constexpr double dt = 0.001;
    constexpr double M = 1.0;
    constexpr double kappa = 0.1;
    constexpr double W = 1.0;

    const std::vector<double> expected{
        0.203192,
        0.500375,
        0.797288};

    auto eta_new =
        phasefield::forward_euler_step(
            eta, dx, dt, M, kappa, W);

    REQUIRE(eta_new.size() == eta.size());

    // Verify numerical accuracy
    for (std::size_t i = 0; i < eta.size(); ++i)
    {
        CAPTURE(i);

        CHECK(
            std::abs(eta_new[i] - expected[i]) <= 1e-12);

        CHECK(eta[i] == eta_original[i]);
    }

    // Verify independent result storage
    eta_new[0] = 10.0;

    CHECK(eta[0] == eta_original[0]);
}