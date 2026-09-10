#pragma once
#include <fstream>
#include <stdexcept>
#include <string>
/*
 * @brief Writes a CSV Document from a Vector of Scientific Workspcae Object
 *
 * The container must provide size() and operator[] access.
 *
 * @tparam Container Type of the input container.
 * @param filename Name of the file to write.
 * @param values Container containing the values to write.
 * @throws std::runtime_error if the file cannot be opened for writing.
 * @note Container is not modified by the template function.
 */
template <typename Container>
void write_csv(const std::string &filename, const Container &values) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }
  file << "index,values\n";
  for (std::size_t i = 0; i < values.size(); ++i) {
    file << i << "," << values[i] << "\n";
  }
  file.close();
}