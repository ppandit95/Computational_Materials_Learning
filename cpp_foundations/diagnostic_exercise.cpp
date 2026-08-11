	#include<iostream>
	#include<cmath>
	int main()
	{
		double D0 = 1.0e-4;
		double Q = 250000;
		double R = 8.314;
		int T[4] = {1000,1100,1200,1300};
		double D[4];
		for(int i=0;i<4;i++)
		{
			D[i] = D0*std::exp(-Q/(R*T[i]));
			std::cout<<"The temperature is "<<T[i]<<" K"<<std::endl;
			std::cout<<"The diffusivity at this temperature is "<<D[i]<<" m^2/sec"<<std::endl;
			if(i>0)
			{
				if((D[i]-D[i-1])>0)
				std::cout<<"The Diffusivity seems to follow the trend at position "<< i <<std::endl;
				else
				std::cout<<"There seems to be issues physics that needs to be looked into."<<std::endl;
				std::cout<<"--------------------------------------------------------------"<<std::endl;
			}
		}
		return 0;
	}
