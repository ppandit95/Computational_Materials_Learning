# Comparing Residual Error and Solution Error in Matrix-Vector Linear Systems

Consider the matrix:
```
A = [[2, -1, 0],
    [-1, 2, -1],
    [0, -1, 2]]
```

with exact solution **x*** = [1, 2, 1]. Then **b** = A**x*** = [0, 2, 0].

## Approximate Solutions

| Solution | Vector |
|----------|--------|
| x₀ | [0, 0, 0] |
| x₁ | [1, 1, 1] |

## Residual Vectors and Error Norms

| Solution | Residual Vector | L₂ Norm |
|----------|-----------------|---------|
| x₀ | [0, 2, 0] | 2.000 |
| x₁ | [-1, 2, 1] | 2.449 |