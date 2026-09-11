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
/*
 * @brief Writes a Scientific CSV Document from a Vector of Scientific Workspcae Object
 * The container must provide size() and operator[] access
 *
 * @tparam CoordinateContainer Type of the input coordinate container.
 * @tparam FieldContainer Type of the input field container.
 * @param coordinates Container containing the coordinate values.
 * @param values Container containing the field values.
 * @param filename Name of the file to write.
 * @throws std::runtime_error if the file cannot be opened for writing.
 * @note Container is not modified by the template function.Moreover, size of Coordinate should be equal to size of field container, otherwise unphysical scenario is detected and std::runtime_error is thrown.
 */
template <typename CoordinateContainer, typename FieldContainer>
void write_field_csv(
    const CoordinateContainer& coordinates,
    const FieldContainer& values,
    const std::string& filename){
      if(coordinates.size() == values.size()){
        std::ofstream file(filename);
        if (!file.is_open()) {
          throw std::runtime_error("Could not open file for writing: " + filename);
        }
        file << "x,field_value\n";
        for (std::size_t i = 0; i < coordinates.size(); ++i) {
          file << coordinates[i] << "," << values[i] << "\n";
        }
        file.close();
      }
      else{
        throw std::runtime_error("Coordinate and Field container sizes do not match.");
      }

    }