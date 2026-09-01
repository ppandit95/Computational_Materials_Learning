#ifndef WORKSPACE_ALGORITHMS_HPP
#define WORKSPACE_ALGORITHMS_HPP

#include<cstddef>
/**
 * @brief Computes the sum of values stored in an indexable container
 *
 *The container must provide size() and read-only operator[] access
 *
 *@tparam Container Type of the input container
 *@param values Container containing the values to sum
 *@return Sum of all values in the container
 *
 *@note Accumulation is performed in double precision
 */
template<typename Container>
double sum(const Container& values){
	double result = 0.0;
	for(std::size_t i=0;i<values.size();++i){
		result += values[i];
	}
	return result;
}

#endif 
