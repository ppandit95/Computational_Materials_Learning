#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "numerical_foundations.hpp"
#include "workspace_algorithms.hpp"
#include "csr_matrix.hpp"
#include "diffusion_1d.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>
TEST_CASE("nearly_equal")
{
  CHECK(nearly_equal(1.0, 1.0, 1e-9, 1e-12));
  CHECK(!nearly_equal(1.0, 2.0, 1e-9, 1e-12));
}
TEST_CASE("nearby O(1) values")
{
  CHECK(nearly_equal(1.0, 1.0 + 1e-10, 1e-9, 1e-12));
  CHECK(!nearly_equal(1.0, 1.0 + 1e-8, 1e-9, 1e-12));
}

TEST_CASE("large Magnitude Values")
{
  CHECK(nearly_equal(1e10, 1e10 + 1e-5, 1e-9, 1e-12));
  CHECK(!nearly_equal(1e10, 1e10 + 1e-3, 1e-9, 1e-12));
}

TEST_CASE("Comparison near zero")
{
  CHECK(nearly_equal(1e-10, 1e-10 + 1e-12, 1e-9, 1e-12));
  CHECK(!nearly_equal(1e-10, 1e-10 + 1e-8, 1e-9, 1e-12));
}

TEST_CASE("clearly unequal values")
{
  CHECK(!nearly_equal(1.0, 2.0, 1e-9, 1e-12));
  CHECK(!nearly_equal(-1.0, 1.0, 1e-9, 1e-12));
}

TEST_CASE("negative rtol or atol")
{
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

  CHECK(l1_error(numerical, reference) == doctest::Approx(1.5));

  CHECK(l2_error(numerical, reference) == doctest::Approx(std::sqrt(0.55)));

  CHECK(linf_error(numerical, reference) == doctest::Approx(0.5));
}
TEST_CASE("single-element vectors")
{
  const std::vector<double> numerical{5.5};
  const std::vector<double> reference{5.0};

  CHECK(l1_error(numerical, reference) == doctest::Approx(0.5));

  CHECK(l2_error(numerical, reference) == doctest::Approx(0.5));

  CHECK(linf_error(numerical, reference) == doctest::Approx(0.5));
}

TEST_CASE("empty vectors have zero error")
{
  const std::vector<double> numerical{};
  const std::vector<double> reference{};

  CHECK(l1_error(numerical, reference) == doctest::Approx(0.0));

  CHECK(l2_error(numerical, reference) == doctest::Approx(0.0));

  CHECK(linf_error(numerical, reference) == doctest::Approx(0.0));
}

TEST_CASE("error norms reject mismatched vector sizes")
{
  const std::vector<double> numerical{1.0, 2.0, 3.0};
  const std::vector<double> reference{1.0, 2.0};

  CHECK_THROWS_AS(l1_error(numerical, reference), std::invalid_argument);

  CHECK_THROWS_AS(l2_error(numerical, reference), std::invalid_argument);

  CHECK_THROWS_AS(linf_error(numerical, reference), std::invalid_argument);
}

TEST_CASE("localized large error")
{
  const std::vector<double> numerical{0.0, 0.0, 10.0, 0.0, 0.0};

  const std::vector<double> reference{0.0, 0.0, 0.0, 0.0, 0.0};

  CHECK(l1_error(numerical, reference) == doctest::Approx(10.0));

  CHECK(l2_error(numerical, reference) == doctest::Approx(10.0));

  CHECK(linf_error(numerical, reference) == doctest::Approx(10.0));
}

TEST_CASE("Checking Relative L2 Norm with identical nonzero vectors")
{
  const std::vector<double> numerical{1, 2, 3, 4};
  const std::vector<double> referece{1, 2, 3, 4};
  CHECK(relative_l2_error(numerical, referece) == doctest::Approx(0.0));
}

TEST_CASE("Checking Relative L2 Norm with {101,202} vs {100,200}")
{
  const std::vector<double> numerical{101, 202};
  const std::vector<double> reference{100, 200};
  CHECK(relative_l2_error(numerical, reference) == doctest::Approx(0.01));
}

TEST_CASE("Checking Relative L2 Norm with {101,202} vs {0.0,0.0}")
{
  const std::vector<double> numerical{101, 202};
  const std::vector<double> reference{0.0, 0.0};
  CHECK_THROWS_AS(relative_l2_error(numerical, reference),
                  std::invalid_argument);
}

TEST_CASE("Checking Relative L2 Norm with Empty Vectors")
{
  const std::vector<double> numerical;
  const std::vector<double> reference;
  CHECK_THROWS_AS(relative_l2_error(numerical, reference),
                  std::invalid_argument);
}

TEST_CASE("Checking Relative L2 Norm with Vectors of Mismatched Sizes")
{
  const std::vector<double> numerical{101, 202};
  const std::vector<double> reference{100, 200, 300};
  CHECK_THROWS_AS(relative_l2_error(numerical, reference),
                  std::invalid_argument);
}
/* ||e|| >= 0,||-e|| = ||e||,|alpha * e| = |alpha| * ||e||
 * Erel,2(alpha.u_num,alpha.u_ref) = Erel,2(u_num,u_ref) provided reference norm
 * is non zero
 */
TEST_CASE("Checking Sign-Symmetry if L1,L2 and L infinity Norm")
{
  const std::vector<double> numerical{2.0, -4.0, 8.0};
  const std::vector<double> reference{1.0, -2.0, 5.0};
  CHECK(l1_error(numerical, reference) ==
        doctest::Approx(l1_error(reference, numerical)));
  CHECK(l2_error(numerical, reference) ==
        doctest::Approx(l2_error(reference, numerical)));
  CHECK(linf_error(numerical, reference) ==
        doctest::Approx(linf_error(reference, numerical)));
}

TEST_CASE("Checking Scaling Behaviour of Error Norms")
{
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
                     [scale](double error)
                     { return scale * error; });
  transform_in_place(scaled_reference,
                     [scale](double error)
                     { return scale * error; });
  CHECK(l1_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(scale * l1_error_original));
  CHECK(l2_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(scale * l2_error_original));
  CHECK(linf_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(scale * linf_error_original));
  CHECK(relative_l2_error(scaled_numerical, scaled_reference) ==
        doctest::Approx(rel_l2_error_original));
}

TEST_CASE("diagnose L2 overflow for very large values")
{
  const std::vector<double> numerical{1.0e200, 1.0e200};

  const std::vector<double> reference{0.0, 0.0};

  const double result = l2_error(numerical, reference);

  CHECK(std::isfinite(result));

  CHECK(result == doctest::Approx(std::sqrt(2.0) * 1.0e200));
}

TEST_CASE("Capturing NaN behavior in Error Norms")
{
  const double nan = std::numeric_limits<double>::quiet_NaN();
  const std::vector<double> numerical{1.0, nan, 3.0};
  const std::vector<double> reference{1.0, 2.0, 3.0};
  CHECK(std::isnan(l1_error(numerical, reference)));
  CHECK(std::isnan(l2_error(numerical, reference)));
}

TEST_CASE("Checking L2 Norm for Extreme Situation")
{
  const std::vector<double> numerical = {1.0e-200, 1.0e-200};
  const std::vector<double> reference = {0.0, 0.0};
  auto result = l2_error(numerical, reference);
  CHECK(result > 0.0);
  CHECK(result == doctest::Approx(std::sqrt(2.0) * 1.0e-200));
}

TEST_CASE("Checking Stable L2 Norm  at Extreme Case")
{
  const std::vector<double> numerical = {1.0e200, 1.0e200};
  const std::vector<double> reference = {0.0, 0.0};
  auto result = l2_error(numerical, reference);
  CHECK(result > 0.0);
  CHECK(result == doctest::Approx(std::sqrt(2.0) * 1.0e200));
}

TEST_CASE("Checking Condition Number for various x")
{
  const double x1 = 1.0;
  const double x2 = 100.0;
  const double x3 = 1.0e-12;
  const double x4 = 1.0e12;
  CHECK(sqrt_relative_condition_number(x1) == doctest::Approx(0.5));
  CHECK(sqrt_relative_condition_number(x2) == doctest::Approx(0.5));
  CHECK(sqrt_relative_condition_number(x3) == doctest::Approx(0.5));
  CHECK(sqrt_relative_condition_number(x4) == doctest::Approx(0.5));
  CHECK_THROWS_AS(sqrt_relative_condition_number(0.0), std::invalid_argument);
  CHECK_THROWS_AS(sqrt_relative_condition_number(-1.0), std::invalid_argument);
}

TEST_CASE("Verify Condition Number of Identity Matrix")
{
  Matrix2x2 I;
  I.a11 = 1.0;
  I.a12 = 0.0;
  I.a21 = 0.0;
  I.a22 = 1.0;
  CHECK(condition_number_inf(I) == doctest::Approx(1));
}

TEST_CASE("Verify Inverse Calculation throws invalid argument")
{
  Matrix2x2 S;
  S.a11 = 1.0;
  S.a12 = 2.0;
  S.a21 = 2.0;
  S.a22 = 4.0;
  CHECK_THROWS_AS(inverse(S), std::invalid_argument);
}

TEST_CASE("Verifying Scientific Trend Near Singularity")
{
  Matrix2x2 A1;
  A1.a11 = 1.0;
  A1.a12 = 1.0;
  A1.a21 = 1.0;
  A1.a22 = 1.0 + 1.0e-2;
  Matrix2x2 A2;
  A2.a11 = 1.0;
  A2.a12 = 1.0;
  A2.a21 = 1.0;
  A2.a22 = 1.0 + 1.0e-4;
  CHECK(condition_number_inf(A2) > condition_number_inf(A1));
  CHECK(condition_number_inf(A2) / condition_number_inf(A1) == doctest::Approx(99.0174));
}

TEST_CASE("Matrix conditioning near binary64 resolution")
{
  Matrix2x2 A{1.0, 1.0,
              1.0, 1.0 + 1e-10};
  CHECK(determinant(A) != 0.0);
  CHECK(condition_number_inf(A) > 1e10);
  Matrix2x2 B{1.0, 1.0,
              1.0, 1.0 + 1e-18};
  CHECK(determinant(B) == 0.0);
  CHECK_THROWS_AS(inverse(B), std::invalid_argument);
  Matrix2x2 A_singular{
      1.0, 1.0,
      1.0, 1.0};
  CHECK_THROWS_AS(inverse(A_singular), std::invalid_argument);
}

TEST_CASE("Condition number bounds RHS perturbation sensitivity")
{
  const Matrix2x2 A{1.0, 1.0, 1.0, 1.0001};
  std::cout << std::setprecision(6);
  const Vector2 x_exact{1.0, 1.0};
  const Vector2 b = matvec(A, x_exact);
  const double delta = 1e-8;
  const Vector2 b_perturbed{b.x1, b.x2 + delta};
  const Vector2 x_calc = solve(A, b);
  const Vector2 x_perturbed = solve(A, b_perturbed);
  const Vector2 db = {b_perturbed.x1 - b.x1, b_perturbed.x2 - b.x2};
  const Vector2 dx = {x_perturbed.x1 - x_calc.x1, x_perturbed.x2 - x_calc.x2};
  const double r_b = infinity_norm(db) / infinity_norm(b);
  const double r_x = infinity_norm(dx) / infinity_norm(x_calc);
  const double B = condition_number_inf(A) * r_b;
  const double R = r_x / r_b;
  CHECK(r_x <=
        B);
  CHECK(r_x > r_b);
}

TEST_CASE("matrix multiplied by its inverse gives identity")
{
  Matrix2x2 A{1.0, 2.0, 3.0, 4.0};
  Vector2 b{5.0, 6.0};
  const Matrix2x2 A_inv = inverse(A);
  const Vector2 product = matvec(A_inv, b);
  const Vector2 prod_A = matvec(A, product);
  CHECK(prod_A.x1 == doctest::Approx(b.x1));
  CHECK(prod_A.x2 == doctest::Approx(b.x2));
}

TEST_CASE("Infinity Norm handles Negative Signs")
{
  const Vector2 v{-3.5, 2.0};
  CHECK(infinity_norm(v) == doctest::Approx(3.5));
}

TEST_CASE("solving reproduces the RHS")
{
  Matrix2x2 A{1.0, 2.0, 3.0, 4.0};
  Vector2 b{5.0, 6.0};
  const Vector2 x = solve(A, b);
  const Vector2 prod = matvec(A, x);
  CHECK(prod.x1 == doctest::Approx(b.x1));
  CHECK(prod.x2 == doctest::Approx(b.x2));
}
TEST_CASE("CSR matvec reproduces tridiagonal operator")
{
  CSRMatrix A(3, 3, {2.0, -1.0, -1.0, 2.0, -1.0, -1.0, 2.0}, {0, 1, 0, 1, 2, 1, 2}, {0, 2, 5, 7});
  const std::vector<double> x{1.0, 2.0, 3.0};
  const auto y = A.matvec(x);
  REQUIRE(y.size() == 3);
  CHECK(y[0] == doctest::Approx(0.0));
  CHECK(y[1] == doctest::Approx(0.0));
  CHECK(y[2] == doctest::Approx(4.0));
  CHECK(A.nnz() == 7);
  CHECK_THROWS_AS(
      A.matvec({1.0, 2.0}),
      std::invalid_argument);
}

TEST_CASE("1D Diffusion Assembly Produces Expected Operator")
{
  const auto A = make_1d_diffusion_matrix(4, 0.5);
  CHECK(A.rows() == 4);
  CHECK(A.cols() == 4);
  CHECK(A.nnz() == 10);

  const std::vector<double> x{1.0, 2.0, 3.0, 4.0};
  const auto y = A.matvec(x);

  REQUIRE(y.size() == 4);

  CHECK(y[0] == doctest::Approx(0.0));
  CHECK(y[1] == doctest::Approx(0.0));
  CHECK(y[2] == doctest::Approx(0.0));
  CHECK(y[3] == doctest::Approx(20.0));
  CHECK_THROWS_AS(make_1d_diffusion_matrix(0, 0.5), std::invalid_argument);
  CHECK_THROWS_AS(make_1d_diffusion_matrix(4, 0.0), std::invalid_argument);
}