#ifndef WORKSPACE_ALGORITHMS_HPP
#define WORKSPACE_ALGORITHMS_HPP
#include<stdexcept>
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
/**
 * @brief Computes the arithmetic mean of an indexable container.
 *
 * The container must provide size() and read-only operator[] access.
 *
 * @tparam Container Type of the input container.
 * @param values Container containing the values.
 * @return Arithmetic mean of the values.
 *
 * @throws std::invalid_argument if the container is empty.
 *
 * @note Accumulation is performed in double precision.
 */
template <typename Container>
double mean(const Container& values)
{
    if (values.size() == 0)
    {
        throw std::invalid_argument(
            "mean requires at least one value"
        );
    }

    return sum(values) /
           static_cast<double>(values.size());
}
#endif 
