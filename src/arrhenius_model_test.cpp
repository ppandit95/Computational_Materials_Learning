#include<iostream>
#include<vector>
#include<stdexcept>
#include"arrhenius_model.hpp"
int main(){
	ArrheniusModel AModel(1.0e-4,250000.0,8.314);
	std::cout<<"---------------------------Case 1: Valid Model+Valid T----------------------------------------------"<<std::endl;
	std::cout<<"Corresponding Diffusivity at 1000K is : " <<AModel.diffusivity(1000.0)<<std::endl;
	std::cout<<"---------------------------Case 2: T<0 ----------------------------------------------"<<std::endl;
	try{
		AModel.diffusivity(-1000.0);
	}
	catch(const std::invalid_argument& e)
	{
    		std::cerr << "Input error: " << e.what() << '\n';
	}
	std::cout<<"---------------------------Case 3: Invalid Model Parameter ----------------------------------------------"<<std::endl;
	try{
		ArrheniusModel bad_Model(1.0e-4,250000.0,-8.314);
	}
	catch(const std::invalid_argument& e)
	{
    		std::cerr << "Object Parameter error: " << e.what() << '\n';
	}
	std::cout<<"---------------------------Case 4: Valid Temperature Vector ----------------------------------------------"<<std::endl;
	try{	
		AModel.diffusivities({800, 1000, 1200});
	}
	catch(const std::invalid_argument& e)
	{
    		std::cerr << "Object Parameter error: " << e.what() << '\n';
	}std::cout<<"---------------------------Case 5: Empty Vector ----------------------------------------------"<<std::endl;
	try{
		AModel.diffusivities({});
	}
	catch(const std::invalid_argument& e)
	{
    		std::cerr << "Object Parameter error: " << e.what() << '\n';
	}std::cout<<"---------------------------Case 6: vector containing one invalid temperature ----------------------------------------------"<<std::endl;
	try{
		AModel.diffusivities({800, -1000, 1200});
	}
	catch(const std::invalid_argument& e)
	{
    		std::cerr << "Object Parameter error: " << e.what() << '\n';
	}
	return 0;
}

	
