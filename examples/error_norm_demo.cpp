#include<iostream>
#include<vector>
#include"numerical_foundations.hpp"
#include<stdexcept>
int main(){
    const std::vector<double> reference{
    1.0, 2.0, 3.0, 4.0
};

const std::vector<double> numerical{
    1.1, 1.9, 3.2, 3.8
};
try{
    double l1 = l1_error(numerical,reference);
    double l2 = l2_error(numerical,reference);
    double linf = linf_error(numerical,reference);
    std::cout<<"L1 Norm : "<<l1<<" L2 Norm: "<<l2<<" Linfinity Norm: "<<linf<<std::endl;
}
catch(std::invalid_argument& e){
    std::cout<<"There is error: " << e.what()<<std::endl;
}

    return 0;
}