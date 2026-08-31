#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "scientific_workspace.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

///////////////////////////////////Test Case 1 : Constructor Invariant////////////////////////////////
TEST_CASE("Scientific_Workspace rejects zero size")
{
    CHECK_THROWS_AS(
        Scientific_Workspace(0),
        std::invalid_argument
    );
}
/////////////////////////////Test Case 2 : Ordinary Storage//////////////////////////////////////////
TEST_CASE("Scientific_Workspace stores independent values")
{
    Scientific_Workspace workspace(3);

    workspace[0] = 10.0;
    workspace[1] = 20.0;
    workspace[2] = 30.0;

    CHECK(workspace.size() == 3);
    CHECK(workspace[0] == 10.0);
    CHECK(workspace[1] == 20.0);
    CHECK(workspace[2] == 30.0);
}
/////////////////////////////////Test Case 3: Deep Copy Test/////////////////////////////////////////
TEST_CASE("Copy construction performs a deep copy")
{
    Scientific_Workspace original(3);

    original[0] = 10.0;
    original[1] = 20.0;
    original[2] = 30.0;

    Scientific_Workspace copy = original;

    REQUIRE(copy.size() == original.size());

    CHECK(copy[0] == original[0]);
    CHECK(copy[1] == original[1]);
    CHECK(copy[2] == original[2]);

    copy[0] = 999.0;

    CHECK(copy[0] == 999.0);
    CHECK(original[0] == 10.0);
}
//////////////////////////////////Test Case 4 :Test Copy Assignment///////////////////////////////////
TEST_CASE("Copy assignment replaces storage with an independent copy")
{
    Scientific_Workspace source(3);

    source[0] = 10.0;
    source[1] = 20.0;
    source[2] = 30.0;

    Scientific_Workspace destination(1);
    destination[0] = -1.0;

    destination = source;

    REQUIRE(destination.size() == 3);

    CHECK(destination[0] == 10.0);
    CHECK(destination[1] == 20.0);
    CHECK(destination[2] == 30.0);

    destination[1] = 999.0;

    CHECK(source[1] == 20.0);
}
//////////////////////////////////Test Case 4 :Test Self Assignment///////////////////////////////////
TEST_CASE("Scientific_Workspace tolerates self assignment")
{
    Scientific_Workspace workspace(2);

    workspace[0] = 4.0;
    workspace[1] = 8.0;

    workspace = workspace;

    REQUIRE(workspace.size() == 2);
    CHECK(workspace[0] == 4.0);
    CHECK(workspace[1] == 8.0);
}
