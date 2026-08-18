#include<iostream>
#include<vector>
#include"arrhenius.hpp"
int main(){
	std::vector<double> increasing_case1 = {};
	std::vector<double> increasing_case2 = {5.0};
	std::vector<double> increasing_case3 = {1.0, 2.0, 3.0};
	std::vector<double> increasing_case4 = {1.0, 1.0, 2.0};
	std::vector<double> increasing_case5 = {1.0, 3.0, 2.0};
	bool actual =
    		is_monotonically_non_decreasing(increasing_case1);
	int failures = 0;
	bool expected = true;
	if(actual != expected){
		++failures;
		std::cout<< "Monotonicity trend for Case1 : FAIL"<<std::endl;}
	else
		std::cout<<"Monotonicity Test for Case1 : PASS"<<std::endl;
	std::cout<<"----------------------------------------------------------------------------------------"<<std::endl;
	actual =
    		is_monotonically_non_decreasing(increasing_case2);
    	if(actual != expected){
		++failures;
		std::cout<< "Monotonicity trend for Case2: FAIL"<<std::endl;}
	else
		std::cout<<"Monotonicity Test for Case2 : PASS"<<std::endl;
	std::cout<<"----------------------------------------------------------------------------------------"<<std::endl;
		actual =
    		is_monotonically_non_decreasing(increasing_case3);
    	if(actual != expected){
		++failures;
		std::cout<< "Monotonicity trend for Case3: FAIL"<<std::endl;}
	else
		std::cout<<"Monotonicity Test for Case3 : PASS"<<std::endl;
	std::cout<<"----------------------------------------------------------------------------------------"<<std::endl;
		actual =
    		is_monotonically_non_decreasing(increasing_case4);
    	if(actual != expected){
		++failures;
		std::cout<< "Monotonicity trend for Case4: FAIL"<<std::endl;}
	else
		std::cout<<"Monotonicity Test for Case4 : PASS"<<std::endl;
	std::cout<<"----------------------------------------------------------------------------------------"<<std::endl;
		actual =
    		is_monotonically_non_decreasing(increasing_case5);
    	if(actual != expected){
		++failures;
		std::cout<< "Monotonicity trend for Case5: FAIL"<<std::endl;}
	else
		std::cout<<"Monotonicity Test for Case5 : PASS"<<std::endl;
	std::cout<<"----------------------------------------------------------------------------------------"<<std::endl;
	if(failures != 0){
		std::cout<<"The monotonicity test failed "<<failures<<" times."<<std::endl;
		return 1;
		}
	else
		return 0;
	}
	


