#include<cmath>
#include<iostream>
#include<vector>
#include"arrhenius.hpp"
double arrhenius_diffusivity(double T,
                             double D0,
                             double Q,
                             double R){
                             double D;
                             D = D0*std::exp(-Q/(R*T));
                             return D;
}
std::vector<double> arrhenius_diffusivities(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R){
    std::vector<double>D(temperatures.size());
    for(std::size_t i=0;i<temperatures.size();++i){
    		D[i] = arrhenius_diffusivity(temperatures[i],D0,Q,R);
    }
    return D;	
}
std::vector<double> arrhenius_diffusivities_empty_pushback(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R){
    std::vector<double> output;
    double D;
    output.reserve(temperatures.size());
    for(std::size_t i=0;i<temperatures.size();++i){
    	D = arrhenius_diffusivity(temperatures[i],D0,Q,R);
    	output.push_back(D);	
    }
    return output;
    }
