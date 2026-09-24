#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "numerical_foundations.hpp"
#include "workspace_algorithms.hpp"
#include <algorithm>
#include <limits>
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

TEST_CASE("identical vectors have zero L1, L2 and Linf error") {
  const std::vector<double> numerical{1.0, 2.0, 3.0, 4.0};
  const std::vector<double> reference{1.0, 2.0, 3.0, 4.0};

  CHECK(l1_error(numerical, reference) == doctest::Approx(0.0));
  CHECK(l2_error(numerical, reference) == doctest::Approx(0.0));
  CHECK(linf_error(numerical, reference) == doctest::Approx(0.0));
}

TEST_CASE("known five-element example with L1, L2 and Linf norm") {
  const std::vector<double> numerical{1.1, 2.2, 3.3, 4.4, 5.5};
  const std::vector<double> reference{1.0, 2.0, 3.0, 4.0, 5.0};

  CHECK(l1_error(numerical, reference) == doctest::Approx(1.5));

  CHECK(l2_error(numerical, reference) == doctest::Approx(std::sqrt(0.55)));

  CHECK(linf_error(numerical, reference) == doctest::Approx(0.5));
}
TEST_CASE("single-element vectors") {
  const std::vector<double> numerical{5.5};
  const std::vector<double> reference{5.0};

  CHECK(l1_error(numerical, reference) == doctest::Approx(0.5));

  CHECK(l2_error(numerical, reference) == doctest::Approx(0.5));

  CHECK(linf_error(numerical, reference) == doctest::Approx(0.5));
}

TEST_CASE("empty vectors have zero error") {
  const std::vector<double> numerical{};
  const std::vector<double> reference{};

  CHECK(l1_error(numerical, reference) == doctest::Approx(0.0));

  CHECK(l2_error(numerical, reference) == doctest::Approx(0.0));

  CHECK(linf_error(numerical, reference) == doctest::Approx(0.0));
}

TEST_CASE("error norms reject mismatched vector sizes") {
  const std::vector<double> numerical{1.0, 2.0, 3.0};
  const std::vector<double> reference{1.0, 2.0};

  CHECK_THROWS_AS(l1_error(numerical, reference), std::invalid_argument);

  CHECK_THROWS_AS(l2_error(numerical, reference), std::invalid_argument);

  CHECK_THROWS_AS(linf_error(numerical, reference), std::invalid_argument);
}

TEST_CASE("localized large error") {
  const std::vector<double> numerical{0.0, 0.0, 10.0, 0.0, 0.0};

  const std::vector<double> reference{0.0, 0.0, 0.0, 0.0, 0.0};

  CHECK(l1_error(numerical, reference) == doctest::Approx(10.0));

  CHECK(l2_error(numerical, reference) == doctest::Approx(10.0));

  CHECK(linf_error(numerical, reference) == doctest::Approx(10.0));
}

TEST_CASE("Checking Relative L2 Norm with identical nonzero vectors") {
  const std::vector<double> numerical{1, 2, 3, 4};
  const std::vector<double> referece{1, 2, 3, 4};
  CHECK(relative_l2_error(numerical, referece) == doctest::Approx(0.0));
}

TEST_CASE("Checking Relative L2 Norm with {101,202} vs {100,200}") {
  const std::vector<double> numerical{101, 202};
  const std::vector<double> reference{100, 200};
  CHECK(relative_l2_error(numerical, reference) == doctest::Approx(0.01));
}

TEST_CASE("Checking Relative L2 Norm with {101,202} vs {0.0,0.0}") {
  const std::vector<double> numerical{101, 202};
  const std::vector<double> reference{0.0, 0.0};
  CHECK_THROWS_AS(relative_l2_error(numerical, reference),
                  std::invalid_argument);
}

TEST_CASE("Checking Relative L2 Norm with Empty Vectors") {
  const std::vector<double> numerical;
  const std::vector<double> reference;
  CHECK_THROWS_AS(relative_l2_error(numerical, reference),
                  std::invalid_argument);
}

TEST_CASE("Checking Relative L2 Norm with Vectors of Mismatched Sizes") {
  const std::vector<double> numerical{101, 202};
  const std::vector<double> reference{100, 200, 300};
  CHECK_THROWS_AS(relative_l2_error(numerical, reference),
                  std::invalid_argument);
}
/* ||e|| >= 0,||-e|| = ||e||,|alpha * e| = |alpha| * ||e||
 * Erel,2(alpha.u_num,alpha.u_ref) = Erel,2(u_num,u_ref) provided reference norm
 * is non zero
 */
TEST_CASE("Checking Sign-Symmetry if L1,L2 and L infinity Norm") {
  const std::vector<double> numerical{2.0, -4.0, 8.0};
  const std::vector<double> reference{1.0, -2.0, 5.0};
  CHECK(l1_error(numerical, reference) ==
        doctest::Approx(l1_error(reference, numerical)));
  CHECK(l2_error(numerical, reference) ==
        doctest::Approx(l2_error(reference, numerical)));
  CHECK(linf_error(numerical, reference) ==
        doctest::Approx(linf_error(reference, numerical)));
}

TEST_CASE("Checking Scaling Behaviour of Error Norms") {
  const std::vector<double> reference{1.0, 2.0, 4.0};
  const std::vector<double> numerical{1.1, 1.8, 4.4};
  const double scale = 1000.0;
  const double l1_error_original = l1_error(numerical, reference);
  const double l2_error_original = l2_error(numerical, reference);
  const double linf_error_original = linf_error(numerical, reference);
  const double rel_l2_error_original = relative_l2_error(numerical, reference);
  // Mutable copies for the scaling experiment.
  std::vector<double> scaled_numerical = numerical;
  std::vector<double> scaled_reference = reference;
  transform_in_place(scaled_numerical,
                     [scale](double error) { return scale * error; });
  transform_in_place(scaled_reference,
                     [scale](double error) { return scale * error; });
  CHECK(l1_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(scale * l1_error_original));
  CHECK(l2_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(scale * l2_error_original));
  CHECK(linf_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(scale * linf_error_original));
  CHECK(relative_l2_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(rel_l2_error_original));
}

TEST_CASE("diagnose L2 overflow for very large values") {
  const std::vector<double> numerical{1.0e200, 1.0e200};

  const std::vector<double> reference{0.0, 0.0};

  const double result = l2_error(numerical, reference);

  CHECK(std::isfinite(result));

  CHECK(result == doctest::Approx(std::sqrt(2.0) * 1.0e200));
}

TEST_CASE("Capturing NaN behavior in Error Norms") {
  const double nan = std::numeric_limits<double>::quiet_NaN();
  const std::vector<double> numerical{1.0, nan, 3.0};
  const std::vector<double> reference{1.0, 2.0, 3.0};
  CHECK(std::isnan(l1_error(numerical, reference)));
  CHECK(std::isnan(l2_error(numerical, reference)));
}

TEST_CASE("Checking L2 Norm for Extreme Situation") {
  const std::vector<double> numerical = {1.0e-200, 1.0e-200};
  const std::vector<double> reference = {0.0, 0.0};
  auto result = l2_error(numerical,reference);
  CHECK(result > 0.0);
  CHECK(result == doctest::Approx(std::sqrt(2.0) * 1.0e-200));
}

TEST_CASE("Checking Stable L2 Norm  at Extreme Case"){
    const std::vector<double> numerical = {1.0e200, 1.0e200};
  const std::vector<double> reference = {0.0, 0.0};
  auto result = l2_error(numerical,reference);
  CHECK(result > 0.0);
  CHECK(result == doctest::Approx(std::sqrt(2.0) * 1.0e200));
}