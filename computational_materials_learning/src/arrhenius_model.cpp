#include<iostream>
#include<cmath>
#include<stdexcept>
#include <vector>
#include"arrhenius_model.hpp"
#include"arrhenius.hpp"
ArrheniusModel::ArrheniusModel(double D0,double Q,double R):D0_(D0),Q_(Q),R_(R){
	if(D0 < 0.0 )
		throw std::invalid_argument("Pre-Exponential Factor is Negative.");
	if(Q < 0.0)
		throw std::invalid_argument("Activation Energy is Negative.");

	
	if(R<0.0)
		throw std::invalid_argument("Gas Constant can never be negative.");
}
ArrheniusModel::ArrheniusModel(
    const ArrheniusParameters& parameters)
    : ArrheniusModel(
          parameters.D0,
          parameters.Q,
          parameters.R)
{
}
double ArrheniusModel::diffusivity(double T) const{
	if(T< 0.0 || approximately_equal(T,0.0,1.0e-6)){
	 	throw std::invalid_argument("The provided temperature is Negative.");
	 }
	 return arrhenius_diffusivity(T,D0_,Q_,R_);
}
std::vector<double> ArrheniusModel::diffusivities(
    const std::vector<double>& temperatures) const{
    	std::vector<double> Diffusivities;
    	double D;
    	Diffusivities.reserve(temperatures.size());
    	/*
    	try{
    		bool = is_monotonically_non_decreasing(temperatures);
    		if(bool == false)
    			throw std::invalid_argument("The Temperature array is not increasing monotonically")<<std::endl;
    	}
    	catch(std::invalid_argument& e){
    		std:cerr<<"The input temperature array has issues : "<<e.what()<<std::endl;
    		return 1;
    	}
    	*/
    	for(std::size_t i=0;i<temperatures.size();++i){
    		try{
    			D = ArrheniusModel::diffusivity(temperatures[i]);
    		}
    		catch(std::invalid_argument& e){
    			std::cerr<<e.what()<<std::endl;
    		}
    		Diffusivities.push_back(D);
    	}
    	return Diffusivities;
    }
double ArrheniusModel::pre_exponential_factor() const{
	return D0_;
}
double ArrheniusModel::activation_energy() const{
	return Q_;
}
double ArrheniusModel::gas_constant() const{
	return R_;
}
bool ArrheniusModel::operator=(const ArrheniusModel& b){
	if(approximately_equal(this->pre_exponential_factor(),b.pre_exponential_factor()) && approximately_equal(this->activation_energy(),b.activation_energy()) && approximately_equal(this->gas_constant(),b.gas_constant()))
		return true;
	else
		return false;
} 
    	 
	 
	
