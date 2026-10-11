# Jacobi Iterative Solver

Consider the matrix equation:
- **A** = `[[2,-1,0],[-1,2,-1],[0,-1,2]]`
- **b** = `[0,2,0]`
- Exact solution: **x*** = `[1,2,1]`

## Core Concept

The Jacobi iteration method splits matrix A into diagonal and non-diagonal components:

```
A = D + R
```

where D is the diagonal matrix and R contains all non-diagonal elements.

## Jacobi Iteration Formula

```
x^(k+1) = D^(-1)(b - R*x^k)
```

## Derivation

Starting with the residual:
```
r^k = b - A*x^k
```

Substitute A = D + R:
```
r^k = b - (D + R)*x^k
r^k + D*x^k = b - R*x^k
```

Therefore, the Jacobi iteration simplifies to:

```
x^(k+1) = D^(-1)*r^k + x^k
```

## Example Calculation

For the matrix A above:
- D = 0.5I

Manual solution with x₀ = [0,0,0]:
- x₁ = [0, 1, 0]
- x₂ = [0.5, 1, 0.5]
