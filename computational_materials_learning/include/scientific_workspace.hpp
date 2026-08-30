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
     * @brief Shallow Copy Constructor for an Scientific Workspace object through another Scientific Workspace Object
     * @param other - Scientific Workspace Object passed by reference
     * @throws std::invalid_argument If size = 0
     */
    Scientific_Workspace(const Scientific_Workspace& other);
    /**
     * @brief Deep Copy Assignment for Scientific_Workspace Objects
     * @return Scientific Workspace object with all the aspects same as input as well as the addresses do remain same
     */
    Scientific_Workspace& operator=(const Scientific_Workspace& other);
    /**
     * @brief Move-constructs a workspace by transferring ownership of its storage.
     * Transfers the dynamically allocated workspace data from @p other to the
     * newly constructed object without performing a deep copy.
     *
     * After the move, the new object owns the transferred storage. The moved-from
     * object remains valid but its state should not be relied upon except for
     * operations documented as valid on moved-from objects.
     *
     * @param other Workspace whose owned storage is transferred.
     *
     * @note This move constructor is compiler-generated.
     * @note The operation is noexcept because the owned std::unique_ptr can be
     *       moved without allocating new storage.
     */
    Scientific_Workspace(Scientific_Workspace&& other) noexcept = default;
    /**
     * @brief Move-assigns another workspace into this object.
     * Transfers ownership of the dynamically allocated workspace storage from
     * @p other into this object without performing a deep copy.
     *
     * Any storage previously owned by this object is released automatically by
     * its std::unique_ptr before ownership of the new storage is acquired.
     *
     * After the move, this object owns the transferred workspace data. The
     * moved-from object remains valid, but its state should not be relied upon
     * unless explicitly documented.
     *
     * @param other Workspace whose owned storage is transferred.
     * @return Reference to this workspace after assignment.
     *
     * @note This move-assignment operator is compiler-generated.
     * @note The operation is noexcept because ownership transfer of
     *       std::unique_ptr does not require allocation or element-wise copying.
     */
    Scientific_Workspace& operator=(Scientific_Workspace&& other) noexcept = default;
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
