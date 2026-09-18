#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "numerical_foundations.hpp"

#include <stdexcept>
#include <vector>

TEST_CASE("nearly_equal") {
    CHECK(nearly_equal(1.0, 1.0, 1e-9, 1e-12));
    CHECK(!nearly_equal(1.0, 2.0, 1e-9, 1e-12));
}
TEST_CASE("nearby O(1) values") {
    CHECK(nearly_equal(1.0, 1.0 + 1e-10, 1e-9, 1e-12));
    CHECK(!nearly_equal(1.0, 1.0 + 1e-8, 1e-9, 1e-12));
}

TEST_CASE("large Magnitude Values") {
    CHECK(nearly_equal(1e10, 1e10 + 1e-5, 1e-9, 1e-12));
    CHECK(!nearly_equal(1e10, 1e10 + 1e-3, 1e-9, 1e-12));
}

TEST_CASE("Comparison near zero") {
    CHECK(nearly_equal(1e-10, 1e-10 + 1e-12, 1e-9, 1e-12));
    CHECK(!nearly_equal(1e-10, 1e-10 + 1e-8, 1e-9, 1e-12));
}

TEST_CASE("clearly unequal values") {
    CHECK(!nearly_equal(1.0, 2.0, 1e-9, 1e-12));
    CHECK(!nearly_equal(-1.0, 1.0, 1e-9, 1e-12));
}

TEST_CASE("negative rtol or atol") {
    CHECK_THROWS_AS(nearly_equal(1.0, 1.0, -1e-9, 1e-12), std::invalid_argument);
    CHECK_THROWS_AS(nearly_equal(1.0, 1.0, 1e-9, -1e-12), std::invalid_argument);
}