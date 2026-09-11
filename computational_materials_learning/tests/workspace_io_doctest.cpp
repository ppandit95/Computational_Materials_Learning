#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "workspace_io.hpp"
#include <cstdio>
TEST_CASE("Nominal File Writing Test") {
  std::vector<double> values = {1.0, 2.0, 3.0};
  CHECK_NOTHROW(write_csv("test_output.csv", values));
  std::ifstream file("test_output.csv");
  CHECK(file.is_open());
  std::string line;
  std::getline(file, line);
  CHECK(line == "index,values");
  file.close();
  std::remove("test_output.csv");
}
TEST_CASE("Writing at Non Existant Address") {
  std::vector<double> values = {1.0, 2.0, 3.0};
  CHECK_THROWS_AS(write_csv("/non_existent_directory/test_output.csv", values), std::runtime_error);
}
TEST_CASE("Input Vector is not modified by write_csv") {
  std::vector<double> values = {1.0, 2.0, 3.0};
  std::vector<double> original_values = values; // Make a copy of the original values
  CHECK_NOTHROW(write_csv("test_output.csv", values));
  CHECK(values == original_values); // Check that the original values are unchanged
  std::remove("test_output.csv");
}
TEST_CASE("Successful Coordinate Filed Output with 3 Entries") {
  std::vector<double> coordinates = {0.0, 1.0, 2.0};
  std::vector<double> field_values = {10.0, 20.0, 30.0};
  CHECK_NOTHROW(write_field_csv(coordinates, field_values, "test_field_output.csv"));
  std::ifstream file("test_field_output.csv");
  CHECK(file.is_open());
  std::string line;
  std::getline(file, line);
  CHECK(line == "x,field_value");
  file.close();
  std::remove("test_field_output.csv");
}
TEST_CASE("Mismatched Field Size and Coordinate Size") {
  std::vector<double> coordinates = {0.0, 1.0};
  std::vector<double> field_values = {10.0, 20.0, 30.0};
  CHECK_THROWS_AS(write_field_csv(coordinates, field_values, "test_field_output.csv"),std::runtime_error);
}
TEST_CASE("Empty but Compatible Inpute to write_field_csv") {
  std::vector<double> coordinates;
  std::vector<double> field_values;
  write_field_csv(coordinates, field_values, "test_field_output.csv");
  std::ifstream file("test_field_output.csv");
  CHECK(file.is_open());
  std::string line;
  std::getline(file, line);
  CHECK(line == "x,field_value");
  file.close();
  std::remove("test_field_output.csv");
}