#include <stdexcept>
#include <utility>
#include "temperature_grid.hpp"
#include "arrhenius.hpp"
Temperature_Grid::Temperature_Grid(std::vector<double> temperatures)
    : temperatures_(std::move(temperatures))
{
    if (temperatures_.empty()) {
        throw std::invalid_argument(
            "Temperature grid must not be empty.");
    }

    for (double T : temperatures_) {
        if (T < 0.0 || approximately_equal(T,0.0)) {
            throw std::invalid_argument(
                "All temperatures must be greater than 0 K.");
        }
    }

    for (std::size_t i = 0; i + 1 < temperatures_.size(); ++i) {
        if (temperatures_[i + 1] <= temperatures_[i]) {
            throw std::invalid_argument(
                "Temperatures must be strictly increasing.");
        }
    }
}

const std::vector<double>& Temperature_Grid::values() const
{
    return temperatures_;
}

std::size_t Temperature_Grid::size() const
{
    return temperatures_.size();
}
