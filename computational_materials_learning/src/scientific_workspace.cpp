#include<iostream>
#include<vector>
#include<stdexcept>
#include<utility>
#include"scientific_workspace.hpp"
#include<memory>
Scientific_Workspace::Scientific_Workspace(std::size_t size):size_(size){
	if(size_ == 0)
		throw std::invalid_argument("The size of Input array is zero.");
	else
		data_ = std::make_unique<double[]>(size);

}
std::size_t Scientific_Workspace::size() const{
	return size_;
}
double& Scientific_Workspace::operator[](std::size_t index){
	return data_[index];
}
const double& Scientific_Workspace::operator[](std::size_t index) const{
	return data_[index];
}



