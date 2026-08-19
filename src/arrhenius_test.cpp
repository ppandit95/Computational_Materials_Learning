#include<iostream>
#include<iomanip>
#include<stdexcept>
#include"arrhenius.hpp"
int main(){
	std::cout<<std::setprecision(25);
	//////////////////////////////////////////Case 1/////////////////////////////////////////////////////////
	bool is_equal;
	double a = 0.1;
	double b = 0.1;
	double rtel = 1.0e-12;
	std::cout<<"The value of a is :"<<a<<std::endl;
	std::cout<<"The value of b is :"<<b<<std::endl;
	try{
		is_equal =  approximately_equal(a,b,rtel);
		std::cout<<"Are the quantities equal :" <<is_equal<<std::endl;
		//return 0;
	}
	catch(std::invalid_argument& e){
		std::cerr<<"The relative tolerance is negative."<<std::endl;
		//return 1;
	}
	std::cout<<"-------------------------------------------------------------------------------------------"<<std::endl;
	//////////////////////////////////////////Case 2/////////////////////////////////////////////////////////
	a = 1.0;
	b = 1.0+1e-13;
	rtel = 1.0e-12;
	std::cout<<"The value of a is :"<<a<<std::endl;
	std::cout<<"The value of b is :"<<b<<std::endl;
	try{
		is_equal =  approximately_equal(a,b,rtel);
		std::cout<<"Are the quantities equal :" <<is_equal<<std::endl;
		//return 0;
	}
	catch(std::invalid_argument& e){
		std::cerr<<"The relative tolerance is negative."<<std::endl;
		//return 1;
	}
	std::cout<<"-------------------------------------------------------------------------------------------"<<std::endl;
	//////////////////////////////////////////Case 3/////////////////////////////////////////////////////////
	a = 1.0;
	b = 1.01;
	rtel = 1.0e-12;
	std::cout<<"The value of a is :"<<a<<std::endl;
	std::cout<<"The value of b is :"<<b<<std::endl;
	try{
		is_equal =  approximately_equal(a,b,rtel);
		std::cout<<"Are the quantities equal :" <<is_equal<<std::endl;
		//return 0;
	}
	catch(std::invalid_argument& e){
		std::cerr<<"The relative tolerance is negative."<<std::endl;
		//return 1;

	}
	std::cout<<"-------------------------------------------------------------------------------------------"<<std::endl;
	//////////////////////////////////////////Case 4/////////////////////////////////////////////////////////
	a = arrhenius_diffusivity(1000.0,1.0e-4,250000.0,8.314);
	b = 8.72707e-18;
	rtel = 1.0e-12;
	std::cout<<"The value of a is :"<<a<<std::endl;
	std::cout<<"The value of b is :"<<b<<std::endl;
	try{
		is_equal =  approximately_equal(a,b,rtel);
		std::cout<<"Are the quantities equal :" <<is_equal<<std::endl;
		//return 0;
	}
	catch(std::invalid_argument& e){
		std::cerr<<"The relative tolerance is negative."<<std::endl;
		//return 1;

	}
}
