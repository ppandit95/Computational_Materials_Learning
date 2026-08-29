#include<vector>
#include<utility>
#include<memory>
/**
 * @brief Scientific_Workspace class which owns its own variables and its elements could be forked when needed.
 * @param size_ stores the size of array to be constructed
 * @param data_ creates a unique pointer to array and owns it corresponding to size provided
 * @throws Invalid_Argument error when the size is set to zero and class object is created 
*/
class Scientific_Workspace
{
public:
    /**
     * @brief Constructs an Scientific Workspace object.
     * @param size - needs the size of unique array to be created upon construction
     * @throws std::invalid_argument If size = 0
     */
    explicit Scientific_Workspace(std::size_t size);
    /**
     * @brief Supplies size of the corresponding array in immutable fashion(Read-Only for user)
     * @return Size of the array
     */
    std::size_t size() const;
    /**
     * @brief Supplies index element  of the corresponding array in mutable fashion to populate its entries
     * @return Element of the array object
     */
    double& operator[](std::size_t index);
    /**
     * @brief Supplies index element  of the corresponding array in immutable fashion for reading purposes
     * @return Element of the array object
     */
    const double& operator[](std::size_t index) const;

private:
    std::size_t size_;
    std::unique_ptr<double[]> data_;
};
