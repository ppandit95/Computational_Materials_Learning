#include<cmath>
#include<iomanip>
#include<iostream>
#include<limits>
int main(){
    const double eps = std::numeric_limits<double>::epsilon();
    std::cout << std::setprecision(20) << eps << std::endl;
    std::cout<< "1+ eps = " << 1 + eps << std::endl;
    std::cout<< "1+ eps/2 = " << 1 + eps/2 << std::endl;
    std::cout<<"0.1+ eps = " << 0.1 + eps << std::endl;
    std::cout<<"0.3 = " << 0.3 << std::endl;
    const double values[] = {
    1.0,
    1.0e4,
    1.0e8,
    1.0e12
};
    for (const double& v : values) {
        std::cout << "Function1_Value: " << sqrt(v*v+1) - v << std::endl;
        std::cout<<"Function2_value:" << 1/(sqrt(v*v+1) + v) << std::endl;
    }
    return 0;
}