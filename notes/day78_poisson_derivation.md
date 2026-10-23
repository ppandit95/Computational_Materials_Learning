# Day 78: Finite-Difference Derivation for the 1D Poisson Equation

Consider the one-dimensional Poisson equation

$$
-u''(x)=f(x), \qquad 0<x<L.
$$

## Uniform grid

Start with the uniform grid

$$
x_i=i\Delta x.
$$

If there are $N$ interior points and two boundary points, the grid indices are
$i=0,1,\ldots,N+1$. The interval is divided into $N+1$ equal subintervals, so

$$
\boxed{\Delta x=\frac{L}{N+1}}.
$$

## Taylor expansions about $x_i$

Assuming that $u$ is sufficiently smooth, expand the values at the neighboring
grid points about $x_i$:

$$
u_{i+1}
=u_i+\Delta x\,u_i'
+\frac{\Delta x^2}{2}u_i''
+\frac{\Delta x^3}{6}u_i'''
+\frac{\Delta x^4}{24}u_i^{(4)}
+O(\Delta x^5),
$$

and

$$
u_{i-1}
=u_i-\Delta x\,u_i'
+\frac{\Delta x^2}{2}u_i''
-\frac{\Delta x^3}{6}u_i'''
+\frac{\Delta x^4}{24}u_i^{(4)}
+O(\Delta x^5).
$$

Adding the two expansions gives

$$
u_{i+1}+u_{i-1}
=2u_i+\Delta x^2u_i''
+\frac{\Delta x^4}{12}u_i^{(4)}
+O(\Delta x^6).
$$

The first- and third-derivative terms cancel because they have opposite signs
in the forward and backward Taylor expansions. More generally, all odd-order
derivative terms cancel when the two symmetric expansions are added. Although
the displayed expansions collect the remainder as $O(\left(\Delta x^5\right))$,
expanding one term further shows that the fifth-derivative terms also have
opposite signs. Thus, for sufficiently smooth $u$, the next surviving term in
the symmetric sum is $O(\left(\Delta x^6\right))$.

Rearranging and dividing by $\Delta x^2$ yields

$$
\frac{u_{i-1}-2u_i+u_{i+1}}{\Delta x^2}
=u_i''+\frac{\Delta x^2}{12}u_i^{(4)}+O(\Delta x^4).
$$

Before division, the leading omitted contribution to the centered second
difference is proportional to $\Delta x^4$. Dividing the entire expression by
$\Delta x^2$ changes this contribution to one proportional to $\Delta x^2$:

$$
\frac{O(\Delta x^4)}{\Delta x^2}=O(\Delta x^2).
$$

Therefore, the centered finite-difference approximation has a second-order
truncation error:

$$
\boxed{
u''(x_i)=\frac{u_{i-1}-2u_i+u_{i+1}}{\Delta x^2}
+O(\Delta x^2)
}.
$$

## Discrete Poisson equation

Let $f_i=f(x_i)$. Substituting the centered approximation into
$-u''(x_i)=f_i$ and multiplying by $\Delta x^2$ gives

$$
\boxed{-u_{i-1}+2u_i-u_{i+1}=\Delta x^2 f_i},
\qquad i=1,2,\ldots,N.
$$
