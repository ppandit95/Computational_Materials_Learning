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
		data_ = std::make_unique<double[]>(size_);

}
Scientific_Workspace::Scientific_Workspace(const Scientific_Workspace& other):size_(other.size()){
	if(size_ == 0)
		throw std::invalid_argument("The size of Input array is zero.");
	else{
		data_ = std::make_unique<double[]>(size_);
		for(std::size_t i=0;i<size_;++i)
			data_[i] = other.data_[i];
	}
}
Scientific_Workspace& Scientific_Workspace::operator=(const Scientific_Workspace& other){
	if (this == &other)
	        return *this;

	auto new_data = std::make_unique<double[]>(other.size_);

	for (std::size_t i = 0; i < other.size_; ++i)
	        new_data[i] = other.data_[i];

	size_ = other.size_;
	data_ = std::move(new_data);
    	return *this;
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



