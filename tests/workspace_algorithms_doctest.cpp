#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include<doctest/doctest.h>

#include"scientific_workspace.hpp"
#include"workspace_algorithms.hpp"

#include<vector>

TEST_CASE("sum computes the total of a Scientific Workspace"){
	Scientific_Workspace values(4);
	values[0] = 1.0;
	values[1] = 2.0;
	values[2] = 3.0;
	values[3] = 4.0;

	CHECK(sum(values) == doctest::Approx(10.0));
}

TEST_CASE("sum works with std::vector"){
	const std::vector<double> values{0.25,0.5,0.75,1.0};
	CHECK(sum(values) == doctest::Approx(2.50));
}

TEST_CASE("sum handles positive and negative values"){
	const std::vector<double> values{5.0,-2.0,-3.0,7.5};
	CHECK(sum(values)==doctest::Approx(7.5));
}

TEST_CASE("sum of an empty compatible container is zero"){
	const std::vector<double> values;
	CHECK(sum(values) == doctest::Approx(0.0));
}


