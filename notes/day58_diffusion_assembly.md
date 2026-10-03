# Manual CSR Representation Generation For 4*4 matrix
For n=4 and dx = 0.5
                    1/(dx)^2 = 4
CSR arrays are ,
values = [8,-4,-4,8,-4,-4,8,-4,-4,8]
col_indices = [0,1,0,1,2,1,2,3,2,3]
row_ptr = [0,2,5,8,10];

We can verify that nnz is 10 as nnz = 3n-2 = 10 which is the last element of row-ptr array.

Due to discretised description of governing equation -d²u/dx² = f(x) using Finite Difference form on a 1D grid with 4 nodes can be represented as:

                 u₀--dx--u₁--dx--u₂--dx--u₃

Using central difference formula for second derivative:

                d²u/dx² = (u[i+1] - 2*u[i] + u[i-1]) / (dx)²

Considering Dirichlet boundary conditions u₀=C₁ and u₃=C₂, the discretized system becomes:

For i=0: 8*u[0] - 4*u[1] = 4*f(u[0]) + 4*C₁
For i=1: -4*u[0] + 8*u[1] - 4*u[2] = 4*f(u[1])
For i=2: -4*u[1] + 8*u[2] - 4*u[3] = 4*f(u[2])
For i=3: -4*u[2] + 8*u[3] = 4*f(u[3]) + 4*C₂

**Matrix form: A*u = b**

Where the coefficient matrix A is:
```
[  8  -4   0   0 ]
[ -4   8  -4   0 ]
[  0  -4   8  -4 ]
[  0   0  -4   8 ]
```



## Matrix Assembly vs. Matrix Storage

**Matrix Assembly** refers to the process of constructing the coefficient matrix **A** from the discretized governing equations (as shown above for i=0,1,2,3).

**Matrix Storage** refers to how the assembled matrix is stored in memory. CSR (Compressed Sparse Row) format is one efficient storage scheme for sparse matrices, as demonstrated with the values, col_indices, and row_ptr arrays above.

In summary: Assembly creates the matrix; storage defines how it's represented in memory.



