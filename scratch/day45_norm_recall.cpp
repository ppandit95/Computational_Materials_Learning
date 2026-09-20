#include<iostream>
#include<vector>
#include<stdexcept>
#include<algorithm>
double linf_error(const std::vector<double>& numerical,
                  const std::vector<double>& reference);
int main(){
    const std::vector<double> numerical={1.1,1.95,3.3,4.4};
    const std::vector<double> reference={1,2,3,4};
    try{
        std::cout<<"L infinity Error of 2 Vectors is : " << linf_error(numerical,reference)<<std::endl;
    }
    catch(std::invalid_argument& e){
        std::cout <<"Error : "<<e.what()<<std::endl;
    }
    const double eA = 0.1;
    const double eB = 0.1;
    const double refA = 100;
    const double refB = 0.2;
    std::cout <<"Relative L2 Norm of Simulation A is : "<<eA/refA<<std::endl;
    std::cout<<"Relative L2 Norm of Simulation B is : "<<eB/refB<<std::endl;
    /*Reporting L2 error as 0.1 can be misleading as both simulations lead to solution of different magnitudes and error should be reported on a relative scale otherwise it will loose its decisive value
    */
    return 0;
}
double linf_error(const std::vector<double>& numerical,const std::vector<double>& reference){
    double max_error = 0.0;
    if(numerical.size() != reference.size())
        throw std::invalid_argument("Both vectors have different sizes");
    else if (numerical.size() == 0 && reference.size() == 0)
    {
        return 0;
    }
    else{
        for(std::size_t i=0;i<numerical.size();++i){
            double error = std::abs(numerical[i]-reference[i]);
            max_error = std::max(max_error,error);
        }
        return max_error;
    }
                    
}