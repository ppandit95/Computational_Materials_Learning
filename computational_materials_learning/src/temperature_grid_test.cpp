#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "temperature_grid.hpp"

int main()
{
    int failures = 0;

    std::cout
        << "Case 1: valid grid constructs successfully\n";

    try {
        Temperature_Grid grid({800.0, 900.0, 1000.0});
    }
    catch (const std::invalid_argument& e) {
        std::cerr
            << "FAIL: Valid grid threw: "
            << e.what() << '\n';
        ++failures;
    }

    std::cout << "Case 2: empty grid throws\n";

    try {
        Temperature_Grid grid({});
        std::cerr
            << "FAIL: Empty grid did not throw.\n";
        ++failures;
    }
    catch (const std::invalid_argument&) {
    }

    std::cout << "Case 3: T = 0 throws\n";

    try {
        Temperature_Grid grid({0.0});
        std::cerr
            << "FAIL: T = 0 did not throw.\n";
        ++failures;
    }
    catch (const std::invalid_argument&) {
    }

    std::cout << "Case 4: negative T throws\n";

    try {
        Temperature_Grid grid({-800.0, -900.0, -1000.0});
        std::cerr
            << "FAIL: Negative temperatures did not throw.\n";
        ++failures;
    }
    catch (const std::invalid_argument&) {
    }

    std::cout
        << "Case 5: duplicate adjacent temperatures throw\n";

    try {
        Temperature_Grid grid({800.0, 800.0, 1000.0});
        std::cerr
            << "FAIL: Duplicate temperatures did not throw.\n";
        ++failures;
    }
    catch (const std::invalid_argument&) {
    }

    std::cout
        << "Case 6: decreasing temperatures throw\n";

    try {
        Temperature_Grid grid({800.0, 700.0, 1000.0});
        std::cerr
            << "FAIL: Decreasing temperatures did not throw.\n";
        ++failures;
    }
    catch (const std::invalid_argument&) {
    }

    std::cout << "Case 7: size() reports correctly\n";

    Temperature_Grid grid7({800.0, 900.0, 1000.0});

    if (grid7.size() != 3) {
        std::cerr << "FAIL: size() returned incorrect value.\n";
        ++failures;
    }

    std::cout
        << "Case 8: values() preserves supplied values\n";

    const std::vector<double> temperatures{
        800.0,
        900.0,
        1000.0
    };

    Temperature_Grid grid8(temperatures);

    const std::vector<double>& stored = grid8.values();

    for (std::size_t i = 0; i < temperatures.size(); ++i) {
        if (stored[i] != temperatures[i]) {
            std::cerr
                << "FAIL: value mismatch at index "
                << i << '\n';
            ++failures;
        }
    }

    // Also verify that construction from an lvalue did not modify
    // the caller's vector.
    if (temperatures[0] != 800.0 ||
        temperatures[1] != 900.0 ||
        temperatures[2] != 1000.0) {
        std::cerr
            << "FAIL: original vector was unexpectedly modified.\n";
        ++failures;
    }

    std::cout
        << "Case 9: construction using std::move\n";

    std::vector<double> movable_temperatures{
        800.0,
        900.0,
        1000.0
    };

    Temperature_Grid grid9(
        std::move(movable_temperatures)
    );

    if (grid9.size() != 3) {
        std::cerr
            << "FAIL: moved grid has incorrect size.\n";
        ++failures;
    }

    if (grid9.values()[0] != 800.0 ||
        grid9.values()[1] != 900.0 ||
        grid9.values()[2] != 1000.0) {
        std::cerr
            << "FAIL: moved grid did not preserve values.\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout
            << "Verdict: All test cases passed successfully.\n";
    }
    else {
        std::cout
            << "Verdict: "
            << failures
            << " test cases failed.\n";
    }

    return failures == 0 ? 0 : 1;
}
