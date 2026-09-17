#pragma once
#include <fstream>
#include <iomanip>
#include <limits>
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
 * @note Numeric values are written with sufficient decimal precision
 * for round-trip recovery of double-precision values.
 */
template <typename Container>
void write_csv(const std::string &filename, const Container &values) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }
  file << std::setprecision(std::numeric_limits<double>::max_digits10);

  file << "index,values\n";
  for (std::size_t i = 0; i < values.size(); ++i) {
    file << i << "," << values[i] << "\n";
  }
  file.close();
}
/*
 * @brief Writes a Scientific CSV Document from a Vector of Scientific Workspcae
 * Object The container must provide size() and operator[] access
 *
 * @tparam CoordinateContainer Type of the input coordinate container.
 * @tparam FieldContainer Type of the input field container.
 * @param coordinates Container containing the coordinate values in meters
 * @param values Container containing the field values.
 * @param filename Name of the file to write.
 * @throws std::runtime_error if the file cannot be opened for writing.
 * @note Container is not modified by the template function.Moreover, size of
 * Coordinate should be equal to size of field container, otherwise unphysical
 * scenario is detected and std::runtime_error is thrown.
 * @note Numeric values are written with sufficient decimal precision
 * for round-trip recovery of double-precision values.
 */
template <typename CoordinateContainer, typename FieldContainer>
void write_field_csv(const CoordinateContainer &coordinates,
                     const FieldContainer &values,
                     const std::string &filename) {
  if (coordinates.size() == values.size()) {
    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " + filename);
    }
    file << std::setprecision(std::numeric_limits<double>::max_digits10);
    file << "x,field_value\n";
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
      file << coordinates[i] << "," << values[i] << "\n";
    }
    file.close();
  } else {
    throw std::runtime_error(
        "Coordinate and Field container sizes do not match.");
  }
}
/*
 * @brief Writes a Scientific CSV Document from a 2 field Vectors of Scientific
 * Workspcae Object The container must provide size() and operator[] access
 *
 * @tparam CoordinateContainer Type of the input coordinate container.
 * @tparam Field1Container Type of the input field container.
 * @tparam Field2Container Type of the input field container.
 * @param coordinates Container containing the coordinate values in meters
 * @param field1 fieldContainer1 containing the field1 values.
 * @param field2 fieldContainer2 containing the field2 values.
 * @param filename Name of the file to write.
 * @throws std::runtime_error if the file cannot be opened for writing as well
 * as std::invalid_argument if the sizes of the containers do not match.
 * @note Container is not modified by the template function.Moreover, size of
 * Coordinate should be equal to size of field container, otherwise unphysical
 * scenario is detected and std::runtime_error is thrown.
 * @note Numeric values are written with sufficient decimal precision
 * for round-trip recovery of double-precision values.
 */
template <typename CoordinateContainer, typename FieldContainer1,
          typename FieldContainer2>
void write_two_fields_csv(const CoordinateContainer &coordinates,
                          const FieldContainer1 &field1,
                          const FieldContainer2 &field2,
                          const std::string &filename) {
  if (coordinates.size() == field1.size() &&
      coordinates.size() == field2.size()) {
    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " + filename);
    }
    file << std::setprecision(std::numeric_limits<double>::max_digits10);
    file << "x,Concentration,Phi\n";
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
      file << coordinates[i] << "," << field1[i] << "," << field2[i] << "\n";
    }
    file.close();
  } else {
    throw std::invalid_argument("coordinate and field sizes must match");
  }
}