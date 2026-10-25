#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "csr_matrix.hpp"

#include "tridiagonal.hpp"

#include "poisson_1d.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>
#include <limits>

TEST_CASE("Verify Matrix Structure of Possion_1D Function")
{
  const CSRMatrix A = build_poisson_1d(3, 1.0);
  REQUIRE(A.rows() == 3);
  REQUIRE(A.cols() == 3);

  CHECK(A.nnz() == 7);

  const std::vector<double> v{1.0, 2.0, 3.0};
  const std::vector<double> product = A.matvec(v);
  const std::vector<double> expected{0.0, 0.0, 64.0};

  CHECK(product[0] == doctest::Approx(expected[0]));
  CHECK(product[1] == doctest::Approx(expected[1]));
  CHECK(product[2] == doctest::Approx(expected[2]));
}

TEST_CASE("Verifying Poisson Builder with Analytical Solution")
{
  const std::vector<double> b{2.0, 2.0, 2.0};
  const CSRMatrix A = build_poisson_1d(3, 1.0);
  const std::vector<double> analytical_sol{0.1875, 0.25, 0.1875};
  const std::vector<double> product = A.matvec(analytical_sol);
  std::vector<double> residual{0.0, 0.0, 0.0};
  for (std::size_t i = 0; i < 3; i++)
  {
    residual[i] = b[i] - product[i];
  }
  CHECK(residual[0] == doctest::Approx(0.0));
  CHECK(residual[1] == doctest::Approx(0.0));
  CHECK(residual[2] == doctest::Approx(0.0));
}

TEST_CASE("Validating Poisson builder with single interior point")
{
  const CSRMatrix A = build_poisson_1d(1, 1.0);

  // Matrix dimensions
  CHECK(A.rows() == 1);
  CHECK(A.cols() == 1);

  // Number of nonzero entries
  CHECK(A.nnz() == 1);

  // Verify the discrete operator
  const std::vector<double> u{0.25};

  const std::vector<double> product = A.matvec(u);

  REQUIRE(product.size() == 1);

  CHECK(product[0] == doctest::Approx(2.0));
}

TEST_CASE("Validating Invalid Inputs to Poisson Builder Function")
{
  CHECK_THROWS_AS(build_poisson_1d(0, 1.0), std::invalid_argument);
  CHECK_THROWS_AS(build_poisson_1d(3, 0.0), std::invalid_argument);
  CHECK_THROWS_AS(build_poisson_1d(3, -1.0), std::invalid_argument);
}