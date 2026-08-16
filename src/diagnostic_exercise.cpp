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
	for(std::size_t i=0;i+1<Diffusivities.size();++i){
		if(Diffusivities[i+1]>=Diffusivities[i])
			std::cout<<"The monotonicity trend seems to be followed at position "<<i<<std::endl;
		else
			std::cout<<"There seems to be issue with pysics of arrhenius equation at "<<i<<std::endl;
	}
	std::cout<<"--------------------------------------------------------"<<std::endl;
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
	return 0;
	
}
