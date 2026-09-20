#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "numerical_foundations.hpp"

#include <stdexcept>
#include <vector>
#include<algorithm>
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

TEST_CASE("identical vectors have zero L1, L2 and Linf error")
{
    const std::vector<double> numerical{1.0, 2.0, 3.0, 4.0};
    const std::vector<double> reference{1.0, 2.0, 3.0, 4.0};

    CHECK(l1_error(numerical, reference) == doctest::Approx(0.0));
    CHECK(l2_error(numerical, reference) == doctest::Approx(0.0));
    CHECK(linf_error(numerical, reference) == doctest::Approx(0.0));
}

TEST_CASE("known five-element example with L1, L2 and Linf norm")
{
    const std::vector<double> numerical{1.1, 2.2, 3.3, 4.4, 5.5};
    const std::vector<double> reference{1.0, 2.0, 3.0, 4.0, 5.0};

    CHECK(l1_error(numerical, reference) ==
          doctest::Approx(1.5));

    CHECK(l2_error(numerical, reference) ==
          doctest::Approx(std::sqrt(0.55)));

    CHECK(linf_error(numerical, reference) ==
          doctest::Approx(0.5));
}
TEST_CASE("single-element vectors")
{
    const std::vector<double> numerical{5.5};
    const std::vector<double> reference{5.0};

    CHECK(l1_error(numerical, reference) ==
          doctest::Approx(0.5));

    CHECK(l2_error(numerical, reference) ==
          doctest::Approx(0.5));

    CHECK(linf_error(numerical, reference) ==
          doctest::Approx(0.5));
}


TEST_CASE("empty vectors have zero error")
{
    const std::vector<double> numerical{};
    const std::vector<double> reference{};

    CHECK(l1_error(numerical, reference) ==
          doctest::Approx(0.0));

    CHECK(l2_error(numerical, reference) ==
          doctest::Approx(0.0));

    CHECK(linf_error(numerical, reference) ==
          doctest::Approx(0.0));
}


TEST_CASE("error norms reject mismatched vector sizes")
{
    const std::vector<double> numerical{1.0, 2.0, 3.0};
    const std::vector<double> reference{1.0, 2.0};

    CHECK_THROWS_AS(
        l1_error(numerical, reference),
        std::invalid_argument);

    CHECK_THROWS_AS(
        l2_error(numerical, reference),
        std::invalid_argument);

    CHECK_THROWS_AS(
        linf_error(numerical, reference),
        std::invalid_argument);
}


TEST_CASE("localized large error")
{
    const std::vector<double> numerical{
        0.0, 0.0, 10.0, 0.0, 0.0
    };

    const std::vector<double> reference{
        0.0, 0.0, 0.0, 0.0, 0.0
    };

    CHECK(l1_error(numerical, reference) ==
          doctest::Approx(10.0));

    CHECK(l2_error(numerical, reference) ==
          doctest::Approx(10.0));

    CHECK(linf_error(numerical, reference) ==
          doctest::Approx(10.0));
}

TEST_CASE("Checking Relative L2 Norm with identical nonzero vectors"){
    const std::vector<double> numerical{1,2,3,4};
    const std::vector<double> referece{1,2,3,4};
    CHECK(relative_l2_error(numerical,referece) == doctest::Approx(0.0));
}

TEST_CASE("Checking Relative L2 Norm with {101,202} vs {100,200}"){
    const std::vector<double> numerical{101,202};
    const std::vector<double> reference{100,200};
    CHECK(relative_l2_error(numerical,reference) == doctest::Approx(0.01));
}

TEST_CASE("Checking Relative L2 Norm with {101,202} vs {0.0,0.0}"){
    const std::vector<double> numerical{101,202};
    const std::vector<double> reference{0.0,0.0};
    CHECK_THROWS_AS(relative_l2_error(numerical,reference),std::invalid_argument);
}

TEST_CASE("Checking Relative L2 Norm with Empty Vectors"){
    const std::vector<double> numerical;
    const std::vector<double> reference;
    CHECK_THROWS_AS(relative_l2_error(numerical,reference),std::invalid_argument);
}

TEST_CASE("Checking Relative L2 Norm with Vectors of Mismatched Sizes"){
    const std::vector<double> numerical{101,202};
    const std::vector<double> reference{100,200,300};
    CHECK_THROWS_AS(relative_l2_error(numerical,reference),std::invalid_argument);
}