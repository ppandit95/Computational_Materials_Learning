#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "allen_cahn.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <limits>
#include <algorithm>

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
// ============================================================
// TEST 10: Invalid Vector Size
// ============================================================
TEST_CASE("Checking for Invalid Input Vector")
{
    std::vector<double> eta{};
    CHECK_THROWS_AS(phasefield::laplacian_neumann(eta, 0.01), std::invalid_argument);
    eta.push_back(1.0);
    CHECK_THROWS_AS(phasefield::laplacian_neumann(eta, 0.01), std::invalid_argument);
}
// ============================================================
// TEST 11: NaN/Inifinity Input
// ============================================================
TEST_CASE("Reject non-finite inputs")
{
    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const double inf_val = std::numeric_limits<double>::infinity();
    const std::vector<double> valid_eta{0.1, 0.2, 0.3};

    // Non-finite dx
    CHECK_THROWS_AS(phasefield::laplacian_neumann(valid_eta, nan_val), std::invalid_argument);
    CHECK_THROWS_AS(phasefield::laplacian_neumann(valid_eta, inf_val), std::invalid_argument);

    // Non-finite element inside eta
    const std::vector<double> bad_eta1{0.1, nan_val, 0.3};
    const std::vector<double> bad_eta2{0.1, inf_val, 0.3};
    CHECK_THROWS_AS(phasefield::laplacian_neumann(bad_eta1, 0.1), std::invalid_argument);
    CHECK_THROWS_AS(phasefield::laplacian_neumann(bad_eta2, 0.1), std::invalid_argument);
}
// ============================================================
// TEST 12: Invalid Timestep
// ============================================================
TEST_CASE("Reject Negative/Non-Finite Timestep")
{
    const double inf_val = std::numeric_limits<double>::infinity();

    const std::vector<double> eta{0.2, 0.5, 0.8};

    constexpr double dx = 0.1;
    constexpr double dt1 = -0.005;
    constexpr double dt2 = 0.0;

    constexpr double M = 1.0;
    constexpr double kappa = 0.1;
    constexpr double W = 1.0;

    CHECK_THROWS_AS(phasefield::forward_euler_step(
                        eta, dx, inf_val, M, kappa, W),
                    std::invalid_argument);

    CHECK_THROWS_AS(phasefield::forward_euler_step(
                        eta, dx, dt1, M, kappa, W),
                    std::invalid_argument);
    CHECK_THROWS_AS(phasefield::forward_euler_step(
                        eta, dx, dt2, M, kappa, W),
                    std::invalid_argument);
}
// ============================================================
// TEST 11: Invalid and Zero Physical Parameters (M, kappa, W)
// ============================================================

TEST_CASE("Validate physical parameters M, kappa, and W")
{
    const std::vector<double> eta{0.2, 0.5, 0.8};
    constexpr double dx = 0.1;

    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const double inf_val = std::numeric_limits<double>::infinity();

    // 1. Negative coefficients must throw std::invalid_argument
    CHECK_THROWS_AS(phasefield::allen_cahn_rhs(eta, dx, -1.0, 0.1, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(phasefield::allen_cahn_rhs(eta, dx, 1.0, -0.1, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(phasefield::allen_cahn_rhs(eta, dx, 1.0, 0.1, -1.0), std::invalid_argument);

    CHECK_THROWS_AS(phasefield::total_free_energy(eta, dx, -0.1, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(phasefield::total_free_energy(eta, dx, 0.1, -1.0), std::invalid_argument);

    // 2. Non-finite coefficients (NaN / Inf) must throw std::invalid_argument
    for (const double bad_val : {nan_val, inf_val, -inf_val})
    {
        CAPTURE(bad_val);

        CHECK_THROWS_AS(phasefield::allen_cahn_rhs(eta, dx, bad_val, 0.1, 1.0), std::invalid_argument);
        CHECK_THROWS_AS(phasefield::allen_cahn_rhs(eta, dx, 1.0, bad_val, 1.0), std::invalid_argument);
        CHECK_THROWS_AS(phasefield::allen_cahn_rhs(eta, dx, 1.0, 0.1, bad_val), std::invalid_argument);

        CHECK_THROWS_AS(phasefield::total_free_energy(eta, dx, bad_val, 1.0), std::invalid_argument);
        CHECK_THROWS_AS(phasefield::total_free_energy(eta, dx, 0.1, bad_val), std::invalid_argument);
    }

    // 3. Zero physical parameters are valid boundary cases
    CHECK_NOTHROW([&]()
                  {
        // M = 0.0 -> zero mobility produces all-zero RHS
        const auto rhs_zero_M = phasefield::allen_cahn_rhs(eta, dx, 0.0, 0.1, 1.0);
        for (const double val : rhs_zero_M)
        {
            CHECK(val == doctest::Approx(0.0));
        }

        // kappa = 0.0 and W = 0.0 -> zero total free energy
        const double zero_energy = phasefield::total_free_energy(eta, dx, 0.0, 0.0);
        CHECK(zero_energy == doctest::Approx(0.0)); }());
}

// ============================================================
// TEST 12: Minimum Valid Grid Size (N = 2)
// ============================================================

TEST_CASE("Verify minimum valid grid size N = 2")
{
    const std::vector<double> eta{0.0, 1.0};
    constexpr double dx = 0.5;
    constexpr double dt = 0.01;
    constexpr double M = 1.0;
    constexpr double kappa = 0.1;
    constexpr double W = 1.0;

    CHECK_NOTHROW([&]()
                  {
        const auto lap = phasefield::laplacian_neumann(eta, dx);
        REQUIRE(lap.size() == 2);
        // Left: (1.0 - 0.0) / 0.25 = 4.0; Right: (0.0 - 1.0) / 0.25 = -4.0
        CHECK(lap[0] == doctest::Approx(4.0));
        CHECK(lap[1] == doctest::Approx(-4.0));

        const auto rhs = phasefield::allen_cahn_rhs(eta, dx, M, kappa, W);
        REQUIRE(rhs.size() == 2);
        CHECK(std::isfinite(rhs[0]));
        CHECK(std::isfinite(rhs[1]));

        const double energy = phasefield::total_free_energy(eta, dx, kappa, W);
        CHECK(std::isfinite(energy));
        CHECK(energy > 0.0);

        const auto eta_next = phasefield::forward_euler_step(eta, dx, dt, M, kappa, W);
        REQUIRE(eta_next.size() == 2);
        CHECK(std::isfinite(eta_next[0]));
        CHECK(std::isfinite(eta_next[1])); }());
}

// ============================================================
// TEST 13: Equilibrium Interface Grid Refinement Convergence
// ============================================================

TEST_CASE("Verify equilibrium interface refinement convergence")
{
    constexpr double L = 10.0;
    constexpr double M = 1.0;
    constexpr double kappa = 0.1;
    constexpr double W = 1.0;
    const double interface_width = std::sqrt(2.0 * kappa / W);

    std::vector<double> residuals;

    for (const std::size_t N : {100ul, 200ul, 400ul})
    {
        const double dx = L / static_cast<double>(N);
        std::vector<double> eta(N, 0.0);

        for (std::size_t i = 0; i < N; ++i)
        {
            const double x = (static_cast<double>(i) + 0.5) * dx;
            eta[i] = std::tanh((x - L / 2.0) / interface_width);
        }

        const auto rhs = phasefield::allen_cahn_rhs(eta, dx, M, kappa, W);

        // Compute max interior residual excluding boundary cells [1 .. N-2]
        double max_residual = 0.0;
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            max_residual = std::max(max_residual, std::abs(rhs[i]));
        }

        residuals.push_back(max_residual);
    }

    REQUIRE(residuals.size() == 3);
    for (std::size_t i = 1; i < residuals.size(); ++i)
    {
        CAPTURE(i);
        CHECK(residuals[i] < residuals[i - 1]);
    }
}

// ============================================================
// TEST 14: Analytical Interfacial Energy Accuracy
// ============================================================

TEST_CASE("Verify analytical interfacial energy accuracy")
{
    constexpr std::size_t N = 400;
    constexpr double L = 10.0;
    constexpr double kappa = 0.1;
    constexpr double W = 1.0;

    const double sigma_exact = (2.0 * std::sqrt(2.0 * kappa * W)) / 3.0;
    const double interface_width = std::sqrt(2.0 * kappa / W);
    const double dx = L / static_cast<double>(N);

    std::vector<double> eta(N, 0.0);
    for (std::size_t i = 0; i < N; ++i)
    {
        const double x = (static_cast<double>(i) + 0.5) * dx;
        eta[i] = std::tanh((x - L / 2.0) / interface_width);
    }

    const double sigma_num = phasefield::total_free_energy(eta, dx, kappa, W);
    const double relative_error = std::abs(sigma_num - sigma_exact) / sigma_exact;

    CHECK(relative_error < 1.0e-3);
}
