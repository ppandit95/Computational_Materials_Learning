#include<iostream>
#include<vector>
#include<stdexcept>
#include"arrhenius.hpp"
#include"arrhenius_model.hpp"
int main(){
	const double D0 = 1.0e-4;
	const double Q  = 250000.0;
	const double R  = 8.314;
	int failures = 0;

	ArrheniusModel AModel(D0, Q, R);
	std::cout<<"---------------------------Case 1: Valid Model+Valid T----------------------------------------------"<<std::endl;
	std::cout<<"Corresponding Diffusivity at 1000K is : " <<AModel.diffusivity(1000.0)<<std::endl;
	std::cout<<"---------------------------Case 2: T<0 ----------------------------------------------"<<std::endl;
	try{
    		AModel.diffusivity(-1000.0);
    		std::cerr<< "FAIL: negative temperature did not throw.\n";
    		++failures;
	}
	catch (const std::invalid_argument& e)
	{
    		std::cout<< "PASS: negative temperature rejected: "<< e.what() << '\n';
	}
	std::cout<<"---------------------------Case 3: Invalid Model Parameter ----------------------------------------------"<<std::endl;
	try{
		ArrheniusModel bad_Model(1.0e-4,250000.0,-8.314);
		std::cerr<< "FAIL: negative Gas Constant did not throw.\n";
    		++failures;
	}
	catch(const std::invalid_argument& e)
	{
    		std::cout<< "PASS: negative gas constant rejected: "<< e.what() << '\n';
	}
	std::cout<<"---------------------------Case 4: Valid Temperature Vector ----------------------------------------------"<<std::endl;
	const auto values = AModel.diffusivities({800.0, 1000.0, 1200.0});
	if (values.size() != 3){
    		std::cerr<< "FAIL: expected 3 diffusivities, got "<< values.size() << '\n';
		++failures;
}
	std::cout<<"---------------------------Case 5: Empty Vector ----------------------------------------------"<<std::endl;
	const auto empty_result = AModel.diffusivities({});
	if (!empty_result.empty())
	{
    		std::cerr<< "FAIL: empty temperature vector produced "<< "non-empty diffusivity vector.\n";
    		++failures;
}
	std::cout<<"---------------------------Case 6: vector containing one invalid temperature ----------------------------------------------"<<std::endl;
	try{
		AModel.diffusivities({800, -1000, 1200});
		std::cerr<< "FAIL: Invalid Temperature Array did not throw.\n";
    		++failures;
	}
	catch(const std::invalid_argument& e)
	{
    		std::cout<< "PASS: negative temperature entry rejected: "<< e.what() << '\n';
	}
	std::cout<< "---------------- Case 7: Public accessors ----------------\n";
	if (AModel.pre_exponential_factor() != D0){
    		std::cerr << "FAIL: incorrect pre-exponential factor.\n";
    		++failures;
	}
	if (AModel.activation_energy() != Q){
    		std::cerr << "FAIL: incorrect activation energy.\n";
    		++failures;
	}
	if (AModel.gas_constant() != R){
    		std::cerr << "FAIL: incorrect gas constant.\n";
    		++failures;
	}
	std::cout<< "---------------- Case 8: Arrhenius behavior ----------------\n";

	const std::vector<double> Temperatures{
	    800.0,
	    1000.0,
	    1200.0,
	    1400.0
	};

	std::vector<double> Diffusivities =
    		AModel.diffusivities(Temperatures);

	if (!is_monotonically_non_decreasing(Diffusivities))
	{
    		std::cerr
        		<< "FAIL: diffusivity is not monotonically "
           		"non-decreasing with temperature.\n";

    		++failures;
	}
	if (failures == 0)
	{
    		std::cout << "All tests passed.\n";
    		return 0;
	}
	std::cerr << failures << " test(s) failed.\n";
	return 1;
}

	
