#include "csr_matrix.hpp"
#include "tridiagonal.hpp"
#include "jacobi_iteration.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

double findMedian(std::vector<double> values)
{
    if (values.empty())
    {
        throw std::invalid_argument(
            "Cannot compute median of empty vector");
    }

    std::sort(values.begin(), values.end());

    const std::size_t n = values.size();

    if (n % 2 == 1)
    {
        return values[n / 2];
    }

    return 0.5 * (values[n / 2 - 1] + values[n / 2]);
}

int main()
{
    using clock = std::chrono::steady_clock;

    std::ofstream out("data/jacobi_grid_scaling.csv");

    if (!out)
    {
        std::cerr << "Error: Unable to open CSV output file.\n";
        return 1;
    }

    out << "n,rho_theory,k_predicted,k_observed,median_ms\n";

    const std::vector<std::size_t> grid_sizes{
        8, 16, 32, 64};

    constexpr double diagonal = 2.0;
    constexpr double upper_lower = -1.0;
    constexpr double tolerance = 1.0e-4;

    constexpr std::size_t max_iter = 20000;
    constexpr std::size_t repetitions = 3;

    const double pi = std::acos(-1.0);

    out << std::setprecision(12);

    for (const std::size_t n : grid_sizes)
    {
        // Construct the problem once per grid size.
        const auto A = make_tridiagonal_matrix(
            n,
            upper_lower,
            diagonal,
            upper_lower);

        std::vector<double> b(n, 0.0);

        for (std::size_t i = 0; i < n; ++i)
        {
            b[i] = std::sin(
                pi * static_cast<double>(i + 1) /
                static_cast<double>(n + 1));
        }

        const std::vector<double> x0(n, 0.0);
        const std::vector<double> diagonal_values(n, diagonal);

        const double rho_theory = std::cos(
            pi / static_cast<double>(n + 1));

        const std::size_t k_predicted =
            static_cast<std::size_t>(
                std::ceil(
                    std::log(tolerance) /
                    std::log(rho_theory)));

        std::vector<double> times_ms;
        times_ms.reserve(repetitions);

        JacobiResult result;

        for (std::size_t r = 0; r < repetitions; ++r)
        {
            const auto start = clock::now();

            result = solve_jacobi(
                A,
                b,
                diagonal_values,
                x0,
                tolerance,
                max_iter);

            const auto stop = clock::now();

            const double elapsed_ms =
                std::chrono::duration<double, std::milli>(
                    stop - start)
                    .count();

            times_ms.push_back(elapsed_ms);

            if (!result.converged)
            {
                std::cerr
                    << "Jacobi failed to converge for n="
                    << n << '\n';

                return 1;
            }
        }

        const double median_ms = findMedian(times_ms);

        out << n << ","
            << rho_theory << ","
            << k_predicted << ","
            << result.iterations << ","
            << median_ms << '\n';

        std::cout
            << "n=" << n
            << " | predicted=" << k_predicted
            << " | observed=" << result.iterations
            << " | median=" << median_ms << " ms\n";
    }

    return 0;
}