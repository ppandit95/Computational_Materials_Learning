#include<iostream>
/**
 * @brief Checks if two floating-point numbers are nearly equal within specified tolerances.
 *@param  a First floating-point number
 *@param  b Second floating-point number
 *@param  rtol Relative tolerance
 *@param  atol Absolute tolerance
 *@return true if the numbers are nearly equal, false otherwise
 *@throws std::invalid_argument if rtol or atol is negative
 */
bool nearly_equal(double a,
                  double b,
                  double rtol,
                  double atol){
    if (rtol<0 || atol<0){
        throw std::invalid_argument("Error: rtol and atol must be non-negative.");
    }
    return std::abs(a - b) <= (atol + rtol * std::abs(b));
}