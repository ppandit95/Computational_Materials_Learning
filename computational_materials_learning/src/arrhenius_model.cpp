#include<iostream>
#include<cmath>
#include<stdexcept>
#include"arrhenius_model.hpp"
#include"arrhenius.hpp"
ArrheniusModel::ArrheniusModel(double D0,double Q,double R){
	if(D0 < 0.0 )
		throw std::invalid_argument("Pre-Exponential Factor is Negative.");
	if(Q < 0.0)
		throw std::invalid_argument("Activation Energy is Negative.");

	
	if(R<0.0)
		throw std::invalid_argument("Gas Constant can never be negative.");
	D0_ = D0 ;
	Q_ = Q;
	R_ = R;
}
double ArrheniusModel::diffusivity(double T) const{
	if(T< 0.0 || approximately_equal(T,0.0,1.0e-6)){
	 	throw std::invalid_argument("The provided temperature is Negative.");
	 	return 1;
	 }
	 return arrhenius_diffusivity(T,D0_,Q_,R_);
}
	 
	 
	
