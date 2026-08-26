#include <iostream>
#include <stdexcept>
#include <vector>

#include "arrhenius.hpp"
#include "arrhenius_model.hpp"

double diffusivity_at_1000K(
    const ArrheniusModel& model)
{
    return model.diffusivity(1000.0);
}
int main()
{
    const double D0 = 1.0e-4;
    const double Q  = 250000.0;
    const double R  = 8.314;

    const double relative_tolerance = 1.0e-12;

    int failures = 0;

    ArrheniusModel model(D0, Q, R);


    // -------------------------------------------------------------------------
    // Case 1: Valid model + valid temperature
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 1: Valid model + valid T ----------------\n";

    const double T = 1000.0;

    const double expected_diffusivity =
        arrhenius_diffusivity(T, D0, Q, R);

    const double actual_diffusivity =
        model.diffusivity(T);

    if (!approximately_equal(
            actual_diffusivity,
            expected_diffusivity,
            relative_tolerance))
    {
        std::cerr
            << "FAIL: ArrheniusModel diffusivity does not match "
            << "the scalar Arrhenius implementation.\n";

        ++failures;
    }
    else
    {
        std::cout
            << "PASS: diffusivity at "
            << T
            << " K = "
            << actual_diffusivity
            << " m^2/s\n";
    }


    // -------------------------------------------------------------------------
    // Case 2: Invalid scalar temperature
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 2: T <= 0 ----------------\n";

    try
    {
        model.diffusivity(-1000.0);

        std::cerr
            << "FAIL: negative temperature did not throw.\n";

        ++failures;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout
            << "PASS: negative temperature rejected: "
            << e.what()
            << '\n';
    }


    // -------------------------------------------------------------------------
    // Case 3: Invalid model parameter
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 3: Invalid model parameter ----------------\n";

    try
    {
        ArrheniusModel bad_model(
            1.0e-4,
            250000.0,
            -8.314
        );

        std::cerr
            << "FAIL: negative gas constant did not throw.\n";

        ++failures;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout
            << "PASS: negative gas constant rejected: "
            << e.what()
            << '\n';
    }


    // -------------------------------------------------------------------------
    // Case 4: Valid temperature vector
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 4: Valid temperature vector ----------------\n";

    const std::vector<double> valid_temperatures{
        800.0,
        1000.0,
        1200.0
    };

    const auto values =
        model.diffusivities(valid_temperatures);

    if (values.size() != valid_temperatures.size())
    {
        std::cerr
            << "FAIL: expected "
            << valid_temperatures.size()
            << " diffusivities but obtained "
            << values.size()
            << ".\n";

        ++failures;
    }
    else
    {
        for (std::size_t i = 0; i < values.size(); ++i)
        {
            const double expected =
                model.diffusivity(valid_temperatures[i]);

            if (!approximately_equal(
                    values[i],
                    expected,
                    relative_tolerance))
            {
                std::cerr
                    << "FAIL: batch diffusivity mismatch at index "
                    << i
                    << ".\n";

                ++failures;
            }
        }
    }


    // -------------------------------------------------------------------------
    // Case 5: Empty temperature vector
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 5: Empty vector ----------------\n";

    const auto empty_result =
        model.diffusivities({});

    if (!empty_result.empty())
    {
        std::cerr
            << "FAIL: empty temperature vector produced "
            << "a non-empty diffusivity vector.\n";

        ++failures;
    }


    // -------------------------------------------------------------------------
    // Case 6: Vector containing an invalid temperature
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 6: Invalid temperature in vector ----------------\n";

    try
    {
        model.diffusivities({
            800.0,
            -1000.0,
            1200.0
        });

        std::cerr
            << "FAIL: invalid temperature vector did not throw.\n";

        ++failures;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout
            << "PASS: negative temperature entry rejected: "
            << e.what()
            << '\n';
    }


    // -------------------------------------------------------------------------
    // Case 7: Public accessors
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 7: Public accessors ----------------\n";

    if (model.pre_exponential_factor() != D0)
    {
        std::cerr
            << "FAIL: incorrect pre-exponential factor.\n";

        ++failures;
    }

    if (model.activation_energy() != Q)
    {
        std::cerr
            << "FAIL: incorrect activation energy.\n";

        ++failures;
    }

    if (model.gas_constant() != R)
    {
        std::cerr
            << "FAIL: incorrect gas constant.\n";

        ++failures;
    }


    // -------------------------------------------------------------------------
    // Case 8: Physical monotonicity
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 8: Arrhenius behaviour ----------------\n";

    const std::vector<double> monotonic_temperatures{
        800.0,
        1000.0,
        1200.0,
        1400.0
    };

    std::vector<double> monotonic_diffusivities =
        model.diffusivities(monotonic_temperatures);

    if (!is_monotonically_non_decreasing(
            monotonic_diffusivities))
    {
        std::cerr
            << "FAIL: diffusivity is not monotonically "
            << "non-decreasing with increasing temperature.\n";

        ++failures;
    }


    // -------------------------------------------------------------------------
    // Case 9: Scalar constructor vs parameter-struct constructor
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 9: Struct constructor state ----------------\n";

    const ArrheniusParameters parameters{
        D0,
        Q,
        R
    };

    const ArrheniusModel model_scalar(
        D0,
        Q,
        R
    );

    const ArrheniusModel model_struct(
        parameters
    );

    if (model_scalar.pre_exponential_factor()
        != model_struct.pre_exponential_factor())
    {
        std::cerr
            << "FAIL: D0 mismatch between constructors.\n";

        ++failures;
    }

    if (model_scalar.activation_energy()
        != model_struct.activation_energy())
    {
        std::cerr
            << "FAIL: Q mismatch between constructors.\n";

        ++failures;
    }

    if (model_scalar.gas_constant()
        != model_struct.gas_constant())
    {
        std::cerr
            << "FAIL: R mismatch between constructors.\n";

        ++failures;
    }


    // -------------------------------------------------------------------------
    // Case 10: Equivalent numerical behaviour of both constructors
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 10: Equivalent diffusivities ----------------\n";

    const std::vector<double> comparison_temperatures{
        800.0,
        1000.0,
        1200.0,
        1400.0
    };

    const auto diff_scalar =
        model_scalar.diffusivities(comparison_temperatures);

    const auto diff_struct =
        model_struct.diffusivities(comparison_temperatures);

    if (diff_scalar.size() != diff_struct.size())
    {
        std::cerr
            << "FAIL: diffusivity-vector size mismatch.\n";

        ++failures;
    }
    else
    {
        for (std::size_t i = 0; i < diff_scalar.size(); ++i)
        {
            if (!approximately_equal(
                    diff_scalar[i],
                    diff_struct[i],
                    relative_tolerance))
            {
                std::cerr
                    << "FAIL: diffusivity mismatch at index "
                    << i
                    << ".\n";

                ++failures;
            }
        }
    }


    // -------------------------------------------------------------------------
    // Case 11: Invalid ArrheniusParameters must preserve constructor validation
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 11: Invalid parameter struct ----------------\n";

    try
    {
        const ArrheniusParameters bad_parameters{
            1.0e-4,
            250000.0,
            -8.314
        };

        const ArrheniusModel bad_model(
            bad_parameters
        );

        std::cerr
            << "FAIL: invalid parameter struct was accepted.\n";

        ++failures;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout
            << "PASS: invalid parameter struct rejected: "
            << e.what()
            << '\n';
    }
    // -------------------------------------------------------------------------
    // Case 12: Checking eplicit behaviour of helper function
    // -------------------------------------------------------------------------

    std::cout
        << "---------------- Case 12: Explicit behaviour of Helper Function ----------------\n";
        /*const ArrheniusParameters parameters{
    		1.0e-4,
    		250000.0,
    		8.314
	};*/
	ArrheniusModel model_12(parameters);
	const double value = diffusivity_at_1000K(model_12);
	if(!approximately_equal(value,model_12.diffusivity(1000.0))){
		std::cerr<<"FAIL:The values of helper function and model function are not same"<<std::endl;
		++failures;
	}

    // -------------------------------------------------------------------------
    // Final result
    // -------------------------------------------------------------------------

    std::cout
        << "------------------------------------------------------------------\n";

    if (failures == 0)
    {
        std::cout
            << "All ArrheniusModel tests passed.\n";

        return 0;
    }

    std::cerr
        << failures
        << " test(s) failed.\n";

    return 1;
}
