import numpy as np
import jax.numpy as jnp
a = np.array([1,2,3,4])
b = np.array([4,3,2,1])
c = a*b+a**2
sum_c = np.sum(c)

A = jnp.array([1,2,3,4])
B = jnp.array([4,3,2,1])
C = A*B+A**2
sum_C = jnp.sum(C)

print("-----------------------------NumPy Computation-----------------------------------------------")
print(f"Value of c vector:{c}")
print(f"Summation of elements of c vector:{sum_c}")
print("-----------------------------JAX Numpy Computation-----------------------------------------------")
print(f"Value of c vector:{C}")
print(f"Summation of elements of c vector:{sum_C}")
