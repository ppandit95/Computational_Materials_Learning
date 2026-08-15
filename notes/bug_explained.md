std::vector<double> result;
result[0] = value;

Fails because result.size() == 0. No element at index 0 exists.
operator[] accesses an existing element; it does not create one.

std::vector<double> result;
result.push_back(value);

Works because push_back constructs a new element and increases the
vector size from 0 to 1. It also allocates more storage automatically
if the current capacity is insufficient.

std::vector<double> result;
result.reserve(100);
result[0] = value;

Still invalid because reserve changes capacity, not size.
The vector has storage available for at least 100 elements, but contains
zero constructed elements. Therefore index 0 still does not exist.
