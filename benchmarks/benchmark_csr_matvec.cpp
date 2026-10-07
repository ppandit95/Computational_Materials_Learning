#include "tridiagonal.hpp"
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
int main()
{
    std::ofstream out("data/day62_csr_matvec.csv");
    if (!out)
        std::cerr << "Could not open benchmark output file.\n";
    out << "n,nnz,mean_seconds\n";
    const std::vector<std::size_t> sizes{1'000, 10'000, 100'000, 1'000'000};
    for (std::size_t size : sizes)
    {
        const auto A = make_tridiagonal_matrix(size, -1.0, 2.0, -1.0);
        const std::vector<double> x(size, 1.0);
        for (int i = 0; i < 5; i++)
        {
            auto y = A.matvec(x);
            (void)y;
        }
        /**
         * @brief Determines the number of benchmark repetitions based on matrix size.
         *
         * Scales the repetition count inversely with the input size to maintain
         * consistent benchmark execution time across different problem scales:
         * - Size ≤ 10,000:     1000 repetitions
         * - Size ≤ 100,000:    200 repetitions
         * - Size ≤ 1,000,000:  100 repetitions
         * - Size > 1,000,000:  30 repetitions
         *
         * This adaptive approach ensures smaller matrices are tested more thoroughly
         * while larger matrices complete in reasonable time.
         */
        const std::size_t repititions = size <= 10'000 ? 1000 : size <= 100'000 ? 200
                                                            : size <= 1'000'000 ? 100
                                                                                : 30;
        using clock = std::chrono::steady_clock;
        double checksum = 0.0;
        const auto start = clock::now();
        for (std::size_t j = 0; j < repititions; j++)
        {
            const auto y = A.matvec(x);
            checksum += y[size / 2];
        }
        const auto stop = clock::now();
        const std::chrono::duration<double> elasped = stop - start;
        std::cout << "Size = " << size << " checksum = " << checksum << "\n";
        out << size << "," << A.nnz() << "," << elasped.count() / repititions << "\n";
    }
    out.close();
    return 0;
}