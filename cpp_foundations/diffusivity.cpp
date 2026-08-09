#include<iostream>
#include<cmath>
int main()
{
	double D0 = 1.0e-4;
	double Q = 250000;
	double R = 8.314;
	double T = 1200;
	double postfactor = std::exp(-Q/(R*T));
	double D = D0*postfactor;
	std::cout<<"The Temperature is "<<T<<" K\n";
	std::cout<<"The Diffusivity is "<<D<<" m^2/sec\n";
	return 0;
	}
