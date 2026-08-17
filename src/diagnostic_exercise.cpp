#include<iostream>
#include<vector>
#include<stdexcept>
#include"arrhenius.hpp"
int main(){
	const std::vector<double> Temp = {1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0};
	std::vector<double> Diffusivities;
	try{
		Diffusivities = arrhenius_diffusivities_empty_pushback(Temp,1.0e-4,250000.0,8.314);
	}
	catch(const std::invalid_argument& e)
	{
    		std::cerr << "Input error: " << e.what() << '\n';
		return 1;
	}
	std::cout << "The monotonicity trend for Diffusivity vector is : " << is_monotonically_non_decreasing(Diffusivities) << std::endl;
	std::cout<<"--------------------------------------------------------"<<std::endl;
	if (is_monotonically_non_decreasing(Diffusivities) == false)
		return 1;
	if (Temp.size() != Diffusivities.size())
	{
    		std::cerr << "Error: temperature and diffusivity sizes differ.\n";
    		return 1;
	}
	for (std::size_t i = 0; i < Temp.size(); ++i)
	{
    		std::cout
        		<< "T = " << Temp[i] << " K"
        		<< ", D = " << Diffusivities[i] << " m^2/s\n";
	}
	std::cout<<"--------------------------------------------------------"<<std::endl;
	//Checking the implementation of Monotonicity function
	std::vector<double> a={};
	std::vector<double> b={5.0};
	std::vector<double> c={1.0,2.0,3.0};
	std::vector<double> d={1.0,1.0,2.0};
	std::vector<double> e={1.0, 2.0, 0.5, 4.0};
	std::cout << "The monotonicity trend for a vector is : " << is_monotonically_non_decreasing(a) << std::endl;
	std::cout<<"--------------------------------------------------------"<<std::endl;
	std::cout << "The monotonicity trend for b vector is : " << is_monotonically_non_decreasing(b) << std::endl;
	std::cout<<"--------------------------------------------------------"<<std::endl;
	std::cout << "The monotonicity trend for c vector is : " << is_monotonically_non_decreasing(c) << std::endl;
	std::cout<<"--------------------------------------------------------"<<std::endl;
	std::cout << "The monotonicity trend for d vector is : " << is_monotonically_non_decreasing(d) << std::endl;
	std::cout<<"--------------------------------------------------------"<<std::endl;
	std::cout << "The monotonicity trend for e vector is : " << is_monotonically_non_decreasing(e) << std::endl;
	std::cout<<"--------------------------------------------------------"<<std::endl;
	return 0;
	
}
