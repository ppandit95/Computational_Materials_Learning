#include "workspace_io.hpp"
#include <chrono>
#include <iostream>
#include <vector>
/**
 * @brief Benchmarks the execution time of writing field data to a CSV file.
 *
 * Measures CSV serialization and file-output time for a preallocated
 * coordinate and field dataset. Data allocation and initialization are
 * excluded from the timed region.
 *
 * @param coordinates Spatial coordinates to write.
 * @param values Field values corresponding to each coordinate.
 * @param filename Path of the output CSV file.
 * @return Elapsed wall-clock time in seconds.
 *
 * @note The coordinate and field containers must have equal sizes.
 * @note This benchmark measures serialization and filesystem I/O together.
 */
const std::vector<std::size_t> sizes{10'000, 100'000, 1'000'000, 5'000'000};
std::vector<double> times;
const double dx = 0.25;
int main() {
  std::ofstream results("benchmarks/csv_io_scaling.csv");
  if (!results) {
    throw std::runtime_error("failed to open benchmark results file");
  }
  for (const auto size : sizes) {
    std::vector<double> coordinates(size);
    std::vector<double> concentration(size);
    for (std::size_t i = 0; i < size; ++i) {
      coordinates[i] = static_cast<double>(i) * dx;
      concentration[i] = static_cast<double>(0.25 + 1.0e-7 * (i));
    }
    const auto start = std::chrono::high_resolution_clock::now();
    write_field_csv(coordinates, concentration, "benchmark_output.csv");
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration =
        std::chrono::duration<double>(end - start)
            .count();
    times.push_back(duration);
    std::cout << "Size: " << size << ", Duration: " << duration << " s, "
              << "Rows per s: " << size / static_cast<double>(duration)
              << std::endl;
  }
  for (std::size_t i = 1; i < times.size(); ++i) {
    std::cout << "Scaling Ratio " << i << ": " << times[i] / times[i - 1]
              << '\n';
  }
  results << "Size,Duration(s),Rows per s" << std::endl;
  results << std::setprecision(std::numeric_limits<double>::max_digits10);
  for (std::size_t i = 0; i < sizes.size(); ++i) {
    results << sizes[i] << "," << times[i] << ","
            << sizes[i] / static_cast<double>(times[i]) << std::endl;
  }
  return 0;
}