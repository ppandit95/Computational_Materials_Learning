#include<iostream>
#include<cmath>
int main()
{
	double D0 = 1.0e-4;
	double Q = 250000;
	double R = 8.314;
	double T[4] = {1000,1100,1200,1300};
	double postfactor[4];
	double D[4];
	for(int i=0;i<4;i++)
	{
		postfactor[i] = std::exp(-Q/(R*T[i]));
		D[i] = D0*postfactor[i];
		std::cout<<"The Temperature is "<<T[i]<<" K\n";
		std::cout<<"The Diffusivity is "<<D[i]<<" m^2/sec\n";
		std::cout<<"--------------------------------------------"<<std::endl;
	}
	return 0;
	}
