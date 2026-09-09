#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "scientific_workspace.hpp"
#include "workspace_algorithms.hpp"

#include <stdexcept>
#include <vector>

TEST_CASE("sum computes the total of a Scientific_Workspace")
{
    Scientific_Workspace values(4);
    values[0] = 1.0;
    values[1] = 2.0;
    values[2] = 3.0;
    values[3] = 4.0;

    CHECK(sum(values) == doctest::Approx(10.0));
}

TEST_CASE("sum works with std::vector")
{
    const std::vector<double> values{0.25, 0.5, 0.75, 1.0};

    CHECK(sum(values) == doctest::Approx(2.5));
}

TEST_CASE("sum handles positive and negative values")
{
    const std::vector<double> values{5.0, -2.0, -3.0, 7.5};

    CHECK(sum(values) == doctest::Approx(7.5));
}

TEST_CASE("sum of an empty compatible container is zero")
{
    const std::vector<double> values{};

    CHECK(sum(values) == doctest::Approx(0.0));
}

TEST_CASE("sum handles floating-point arithmetic with tolerance")
{
    const std::vector<double> values{0.1, 0.2, 0.3};

    CHECK(sum(values) == doctest::Approx(0.6));
}

TEST_CASE("mean computes the average of a Scientific_Workspace")
{
    Scientific_Workspace values(4);
    values[0] = 2.0;
    values[1] = 4.0;
    values[2] = 6.0;
    values[3] = 8.0;

    CHECK(mean(values) == doctest::Approx(5.0));
}

TEST_CASE("mean works with std::vector")
{
    const std::vector<double> values{1.5, 2.5, 3.5};

    CHECK(mean(values) == doctest::Approx(2.5));
}

TEST_CASE("mean rejects an empty container")
{
    const std::vector<double> values{};

    CHECK_THROWS_WITH_AS(
        mean(values),
        "mean requires at least one value",
        std::invalid_argument);
}

TEST_CASE("mean of one value equals that value")
{
    const std::vector<double> values{42.0};

    CHECK(mean(values) == doctest::Approx(42.0));
}

TEST_CASE("transform_in_place scales vector values")
{
    std::vector<double> values{1.0, 2.0, 3.0};
    const double scale_factor = 2.5;

    transform_in_place(
        values,
        [scale_factor](double value)
        {
            return value * scale_factor;
        });

    CHECK(values[0] == doctest::Approx(2.5));
    CHECK(values[1] == doctest::Approx(5.0));
    CHECK(values[2] == doctest::Approx(7.5));
}

TEST_CASE("transform_in_place works with Scientific_Workspace")
{
    Scientific_Workspace values(3);
    values[0] = 25.0;
    values[1] = 100.0;
    values[2] = 500.0;

    transform_in_place(
        values,
        [](double temperature_celsius)
        {
            return temperature_celsius + 273.15;
        });

    CHECK(values[0] == doctest::Approx(298.15));
    CHECK(values[1] == doctest::Approx(373.15));
    CHECK(values[2] == doctest::Approx(773.15));
}

TEST_CASE("count_if counts positive values in std::vector")
{
    const std::vector<double> values{-1.0, 2.0, -3.0, 4.0};

    const auto count_positive =
        count_if(
            values,
            [](double value)
            {
                return value > 0.0;
            });

    CHECK(count_positive == 2);
}

TEST_CASE("count_if works with Scientific_Workspace and captured threshold")
{
    Scientific_Workspace values(4);
    values[0] = 900.0;
    values[1] = 1050.0;
    values[2] = 1200.0;
    values[3] = 750.0;

    const double critical_temperature_kelvin = 1000.0;

    const auto count_high_temperature =
        count_if(
            values,
            [critical_temperature_kelvin](double temperature)
            {
                return temperature > critical_temperature_kelvin;
            });

    CHECK(count_high_temperature == 2);
}

TEST_CASE("count_if returns zero for an empty container")
{
    const std::vector<double> values{};

    const auto count_positive =
        count_if(
            values,
            [](double value)
            {
                return value > 0.0;
            });

    CHECK(count_positive == 0);
}

TEST_CASE("count_if counts all matching elements")
{
    const std::vector<double> values{1.0, 2.0, 3.0, 4.0};

    const auto count_positive =
        count_if(
            values,
            [](double value)
            {
                return value > 0.0;
            });

    CHECK(count_positive == values.size());
}

TEST_CASE("count_if does not modify its input")
{
    std::vector<double> temperatures{
        900.0,
        1050.0,
        1200.0,
        750.0
    };

    const auto original = temperatures;
    const double threshold = 1000.0;

    const auto count_above_threshold =
        count_if(
            temperatures,
            [threshold](double temperature)
            {
                return temperature > threshold;
            });

    CHECK(count_above_threshold == 2);
    CHECK(temperatures == original);
}

TEST_CASE("count_if excludes values equal to a strict threshold")
{
    const std::vector<double> values{
        1.0,
        2.5,
        3.0,
        4.0
    };

    const double threshold = 2.5;

    const auto count_above_threshold =
        count_if(
            values,
            [threshold](double value)
            {
                return value > threshold;
            });

    CHECK(count_above_threshold == 2);
}

TEST_CASE("count_if evaluates a stateful predicate once per element")
{
    const std::vector<double> temperatures{
        900.0,
        1050.0,
        1200.0,
        750.0
    };

    const double threshold = 1000.0;
    std::size_t evaluations = 0;

    const auto predicate =
        [&evaluations, threshold](double temperature)
        {
            ++evaluations;
            return temperature > threshold;
        };

    const auto count_above_threshold =
        count_if(temperatures, predicate);

    CHECK(count_above_threshold == 2);
    CHECK(evaluations == temperatures.size());
}