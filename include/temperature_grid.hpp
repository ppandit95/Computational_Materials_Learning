#pragma once

#include <cstddef>
#include <vector>

/**
 * @brief Owns a strictly increasing grid of positive temperatures.
 *
 * Temperatures are expressed in kelvin.
 *
 * Construction Requirements:
 * - the grid is non-empty;
 * - every temperature is greater than 0 K;
 * - temperatures are strictly increasing.
 */
class Temperature_Grid
{
public:
    /**
     * @brief Construct a temperature grid.
     *
     * @param temperatures Temperatures in K.
     *
     * @throws std::invalid_argument if the vector is empty,
     *         contains T <= 0 K, or is not strictly increasing.
     */
    explicit Temperature_Grid(std::vector<double> temperatures);

    /**
     * @brief Return read-only access to the owned temperatures.
     *
     * @return Const reference to temperatures in K.
     */
    const std::vector<double>& values() const;

    /**
     * @brief Return the number of temperatures.
     */
    std::size_t size() const;

private:
    std::vector<double> temperatures_;
};
