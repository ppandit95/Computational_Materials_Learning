# Day 69 — Why Jacobi Convergence Slows Down with Grid Refinement

## 1. Problem Background

Consider the one-dimensional steady-state diffusion equation:

$$
-\frac{d^2u}{dx^2}=f(x),
\qquad x\in(0,1)
$$

with homogeneous Dirichlet boundary conditions:

$$
u(0)=u(1)=0.
$$

Discretizing the domain using $n$ interior grid points and the
second-order central-difference approximation gives

$$
A_n\mathbf{u}=\mathbf{b},
$$

where

$$
A_n=\frac{1}{h^2}
\begin{bmatrix}
2 & -1 & 0 & \cdots & 0\\
-1 & 2 & -1 & \cdots & 0\\
0 & -1 & 2 & \ddots & \vdots\\
\vdots & \ddots & \ddots & \ddots & -1\\
0 & \cdots & 0 & -1 & 2
\end{bmatrix},
$$

and

$$
h=\frac{1}{n+1}.
$$

Here, $n$ represents the number of interior grid points and,
consequently, the number of unknowns.

As $n$ increases, the grid spacing $h$ decreases, providing a finer
spatial discretization.

The question is:

**How does increasing the number of grid points affect the
convergence rate of Jacobi iteration?**

---

## 2. Jacobi Iteration and Its Iteration Matrix

For a linear system

$$
A\mathbf{u}=\mathbf{b},
$$

decompose the coefficient matrix as

$$
A=D+L+U,
$$

where:

- $D$ contains the diagonal entries.
- $L$ contains the strictly lower-triangular entries.
- $U$ contains the strictly upper-triangular entries.

Jacobi iteration is

$$
\mathbf{u}^{(k+1)}
=
D^{-1}
\left[
\mathbf{b}-(L+U)\mathbf{u}^{(k)}
\right].
$$

Rearranging,

$$
\mathbf{u}^{(k+1)}
=
\underbrace{(I-D^{-1}A)}_{B}
\mathbf{u}^{(k)}
+
D^{-1}\mathbf{b}.
$$

The matrix

$$
\boxed{B=I-D^{-1}A}
$$

is called the **Jacobi iteration matrix**.

The convergence of Jacobi iteration is governed by the spectral
radius of this matrix:

$$
\rho(B)=\max_i|\lambda_i(B)|.
$$

Jacobi converges from every initial guess if and only if

$$
\boxed{\rho(B)<1}.
$$

---

## 3. Spectral Radius for the 1D Diffusion Matrix

For the standard one-dimensional diffusion matrix with homogeneous
Dirichlet boundary conditions, the eigenvalues of the Jacobi
iteration matrix are

$$
\lambda_j(B_n)
=
\cos\left(\frac{j\pi}{n+1}\right),
\qquad j=1,2,\ldots,n.
$$

Therefore, its spectral radius is

$$
\boxed{
\rho(B_n)=
\cos\left(\frac{\pi}{n+1}\right)
}.
$$

This result is specific to the standard 1D diffusion discretization;
it does not apply to arbitrary coefficient matrices.

The spectral radius depends on the number of interior grid points.

As the grid becomes finer, $n$ increases and the argument of the
cosine decreases.

---

## 4. Why Does the Spectral Radius Approach One?

For sufficiently small $x$, the Taylor expansion of cosine is

$$
\cos x
=
1-\frac{x^2}{2}
+\frac{x^4}{24}
-\cdots.
$$

Neglecting higher-order terms gives

$$
\cos x\approx1-\frac{x^2}{2}.
$$

Substituting

$$
x=\frac{\pi}{n+1}
$$

into the spectral-radius expression,

$$
\rho(B_n)
=
\cos\left(\frac{\pi}{n+1}\right),
$$

we obtain

$$
\boxed{
\rho(B_n)
\approx
1-\frac{\pi^2}{2(n+1)^2}
}.
$$

As the number of grid points approaches infinity,

$$
n\rightarrow\infty,
$$

the second term approaches zero:

$$
\frac{\pi^2}{2(n+1)^2}\rightarrow0.
$$

Consequently,

$$
\boxed{
\lim_{n\rightarrow\infty}\rho(B_n)=1
}.
$$

Importantly, the spectral radius approaches one **from below**.

Therefore, Jacobi remains mathematically convergent for every
finite grid size, but its convergence rate becomes progressively
slower as the grid is refined.

---

## 5. Why Does a Spectral Radius Close to One Cause Slow Convergence?

Let $\mathbf{u}^*$ denote the exact solution of the discrete linear
system.

Define the error at iteration $k$ as

$$
\mathbf{e}^{(k)}
=
\mathbf{u}^{(k)}-\mathbf{u}^*.
$$

Since the exact solution satisfies the Jacobi fixed-point equation,
subtracting that equation from the iterative update gives

$$
\mathbf{e}^{(k+1)}
=
B_n\mathbf{e}^{(k)}.
$$

Applying this relationship repeatedly,

$$
\boxed{
\mathbf{e}^{(k)}
=
B_n^k\mathbf{e}^{(0)}
}.
$$

Suppose the initial error contains a component aligned with an
eigenvector $\mathbf{v}_j$ of $B_n$.

Since

$$
B_n\mathbf{v}_j
=
\lambda_j\mathbf{v}_j,
$$

that error component evolves as

$$
\mathbf{e}_j^{(k)}
=
\lambda_j^k\mathbf{e}_j^{(0)}.
$$

Its magnitude therefore decreases by a factor of

$$
|\lambda_j|
$$

during each iteration.

For the slowest-decaying error component, this factor equals the
spectral radius:

$$
\boxed{
\|\mathbf{e}_{\mathrm{slow}}^{(k)}\|
=
\rho(B_n)^k
\|\mathbf{e}_{\mathrm{slow}}^{(0)}\|
}.
$$

### Physical interpretation

Consider two hypothetical convergence factors.

**Case A:**

$$
\rho(B)=0.5.
$$

The corresponding error component is halved during every iteration.

After 10 iterations,

$$
0.5^{10}\approx9.77\times10^{-4}.
$$

Thus, the error component becomes less than 0.1% of its initial
magnitude.

**Case B:**

$$
\rho(B)=0.99.
$$

The error component decreases by only 1% per iteration.

After 10 iterations,

$$
0.99^{10}\approx0.904.
$$

Approximately 90.4% of the original error component remains.

Hence, even though both iterations satisfy $\rho(B)<1$, the second
case converges much more slowly.

**Key observation:** A spectral radius close to one means that
certain error components are eliminated very slowly.

---

## 6. Deriving the Number of Iterations Required for Convergence

Suppose we want to reduce the slowest-decaying error component by a
factor $\varepsilon$, where

$$
0<\varepsilon<1.
$$

We require

$$
\rho(B_n)^k\leq\varepsilon.
$$

Taking the natural logarithm,

$$
k\ln(\rho(B_n))
\leq
\ln(\varepsilon).
$$

Since

$$
0<\rho(B_n)<1,
$$

we have

$$
\ln(\rho(B_n))<0.
$$

Dividing by this negative quantity reverses the inequality:

$$
\boxed{
k\geq
\frac{\ln(\varepsilon)}
{\ln(\rho(B_n))}
}.
$$

Therefore, the minimum integer iteration count is

$$
\boxed{
k_{\min}
=
\left\lceil
\frac{\ln(\varepsilon)}
{\ln(\rho(B_n))}
\right\rceil
}.
$$

For large $n$,

$$
\rho(B_n)
\approx
1-\frac{\pi^2}{2(n+1)^2}.
$$

Using

$$
\ln(1-x)\approx-x
$$

for small positive $x$,

$$
\ln(\rho(B_n))
\approx
-\frac{\pi^2}{2(n+1)^2}.
$$

Substituting into the iteration-count expression,

$$
k
\approx
\frac{\ln(\varepsilon)}
{-\pi^2/[2(n+1)^2]}.
$$

Therefore,

$$
\boxed{
k
\approx
\frac{2(n+1)^2}{\pi^2}
\ln\left(\frac{1}{\varepsilon}\right)
}.
$$

For a fixed error-reduction factor $\varepsilon$,

$$
\boxed{k=O(n^2)}.
$$

### Interpretation

If the number of interior grid points doubles,

$$
n\rightarrow2n,
$$

then, for sufficiently large $n$,

$$
k(2n)\approx4k(n).
$$

Therefore, doubling the spatial resolution approximately quadruples
the number of Jacobi iterations required to achieve the same
reduction in the slowest-decaying error component.

---

## 7. Computational Cost of Grid Refinement

For the one-dimensional diffusion problem, the coefficient matrix
is tridiagonal.

Consequently, the number of nonzero matrix entries scales as

$$
\operatorname{nnz}(A_n)=3n-2.
$$

One Jacobi iteration requires $O(n)$ operations.

Since the required iteration count scales as

$$
k=O(n^2),
$$

the total computational work scales approximately as

$$
\boxed{
W(n)=O(n)\times O(n^2)=O(n^3)
}.
$$

Thus, increasing the number of unknowns affects computational cost
in two ways:

1. Each iteration becomes more expensive because there are more
   unknowns.
2. More iterations are required because the spectral radius
   approaches one.

This explains why Jacobi becomes computationally inefficient for
large diffusion problems.

The $O(n^3)$ estimate describes the standard 1D Jacobi method
under a fixed asymptotic error-reduction requirement. It is not
a universal complexity estimate for all iterative solvers.

---

## 8. Important Distinction: Convergence and Efficiency

The conditions

$$
\rho(B)<1
$$

and

$$
\rho(B)\approx1
$$

describe different aspects of iterative solution methods.

**Convergence** answers:

> Will the iterative method eventually approach the exact solution
> of the discrete linear system?

**Convergence rate** answers:

> How quickly does the error decrease as iterations proceed?

**Computational efficiency** answers:

> How much computational work is required to reach a prescribed
> accuracy?

For the 1D diffusion problem, Jacobi converges for every finite
$n$, but its convergence rate deteriorates as the grid is refined.

Therefore, mathematical convergence does not necessarily imply
computational efficiency.

---

## 9. Relevance to Phase-Field and KKS Simulations

In phase-field simulations, spatial discretization produces large
systems of algebraic equations.

Examples include:

- Diffusion equations for conserved composition fields.
- Linearized equations arising from nonlinear phase-field models.
- Mechanical equilibrium equations for elastic displacement fields.
- Coupled systems involving composition, phase variables, and
  elasticity.

As the computational mesh becomes finer, the number of unknowns
increases.

If a stationary iterative solver such as Jacobi is used without
effective acceleration, the number of iterations required for
convergence can become prohibitively large.

This motivates more advanced methods, including:

- Krylov-subspace solvers such as CG and GMRES.
- Preconditioning techniques.
- Multigrid and algebraic multigrid (AMG) methods.

Interestingly, Jacobi iteration remains useful within multigrid
algorithms because it can efficiently reduce certain
high-frequency error components, even though it is inefficient at
eliminating the smooth error components responsible for slow
standalone convergence.

---

## 10. Final Conclusion

For the standard one-dimensional Dirichlet diffusion problem, the
spectral radius of the Jacobi iteration matrix is

$$
\rho(B_n)=\cos\left(\frac{\pi}{n+1}\right).
$$

Using the Taylor expansion of cosine,

$$
\rho(B_n)
\approx
1-\frac{\pi^2}{2(n+1)^2}.
$$

As the number of grid points increases,

$$
\boxed{\rho(B_n)\rightarrow1^-}.
$$

Consequently, the slowest-decaying error components are reduced by
progressively smaller fractions during each iteration.

The number of iterations required for a fixed asymptotic error
reduction scales approximately as

$$
\boxed{k=O(n^2)}.
$$

Because each iteration requires $O(n)$ operations for the
tridiagonal diffusion matrix, the total computational work scales
approximately as

$$
\boxed{W(n)=O(n^3)}.
$$

**Central takeaway:**

Jacobi iteration remains mathematically convergent for every
finite grid size in this problem, but its computational efficiency
deteriorates rapidly as the grid is refined. This illustrates why
the choice of a linear solver is as important as the correctness
of the spatial discretization when developing scalable numerical
methods for scientific computing.