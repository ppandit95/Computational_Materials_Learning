#include<iostream>
#include<vector>
#include"arrhenius.hpp"
#include"scientific_workspace.hpp"
#include<utility>
#include<memory>
int main(){
	int failures = 0;
	std::cout<<"----------------------------------------Case 1:size 3 constructs successfully----------------------------------\n";
	try{
		Scientific_Workspace a(3);
	}
	catch(std::invalid_argument& e){
		std::cerr<<"FAIL:size 3 object is not being created-"<<e.what()<<std::endl;
		++failures;
	}
	std::cout<<"----------------------------------------Case 2:size 0 throws std::invalid_argument----------------------------------\n";
	try{
		Scientific_Workspace b(0);
		++failures;
	}
	catch(std::invalid_argument& e){
		std::cerr<<"PASS:size 0 object is not being created-"<<e.what()<<std::endl;
	}
	std::cout<<"----------------------------------------Case 3:size() returns 3----------------------------------\n";
	Scientific_Workspace c(3);
	if(c.size() != 3)
		++failures;
	std::cout<<"----------------------------------------Case 4:writing through operator[] is preserved----------------------------------\n";
	Scientific_Workspace d(3);
	d[0] = 1.0;
	d[1] = 2.0;
	d[2] = 3.0;
	if(approximately_equal(d[0],1.0)&&approximately_equal(d[1],2.0)&&approximately_equal(d[2],3.0))
		std::cout<<"PASS:The writing of object preserves order"<<std::endl;
	else
		++failures;
	std::cout<<"----------------------------------------Case 4:writing through operator[] is preserved----------------------------------\n";
	Scientific_Workspace f(3);
	f[0] = 1.0;
	f[1] = 2.0;
	f[2] = 3.0;
	if(approximately_equal(f[0],1.0)&&approximately_equal(f[1],2.0)&&approximately_equal(f[2],3.0))
		std::cout<<"PASS:The writing of object preserves order"<<std::endl;
	else
		++failures;
	std::cout << "----------------------------------------Case 5: const Scientific_Workspace permits read access and moving a Scientific_Workspace transfers ownership----------------------------------\n";
	Scientific_Workspace temp(3);
	temp[0] = 1.0;
	temp[1] = 2.0;
	temp[2] = 3.0;

	const Scientific_Workspace g(std::move(temp));

	if (approximately_equal(g[0], 1.0) &&
    		approximately_equal(g[1], 2.0) &&
    		approximately_equal(g[2], 3.0))
	{
    		std::cout << "PASS: const object permits read access\n";
	}
	else{
    		++failures;
	}
	if(failures == 0){
		std::cout<<"Verdict::All tests passed."<<std::endl;
		return 0;
	}
	else{
		std::cout<<"Verdict:: "<<failures<<" Test Cases failed."<<std::endl;
		return 1;
	}
	}

