# Day 99: Allen--Cahn equation from free energy

**Week 15, Day 1 -- 13 November 2026**

## Aim

Derive the one-dimensional Allen--Cahn equation for a *nonconserved* order
parameter, then identify the free-energy dissipation property that a later
numerical solver must respect.  This note does not prescribe an implementation.

Let \(\eta(x,t)\) be an order parameter on \(0\leq x\leq L\).  For example,
\(\eta\approx -1\) and \(\eta\approx 1\) may represent two locally stable
phases.  Unlike concentration, \(\eta\) need not have a fixed spatial integral:
local phase conversion is allowed.

## Free-energy functional

Use the Ginzburg--Landau free energy

\[
F[\eta]=\int_0^L\left[
\frac{W}{4}(\eta^2-1)^2+
\frac{\kappa}{2}(\eta_x)^2
\right]dx,
\]

where \(W>0\) is the double-well barrier height and \(\kappa>0\) is the
gradient-energy coefficient.  The bulk-energy density is minimized at
\(\eta=\pm1\); the gradient term makes abrupt phase changes energetically
costly and therefore gives an interface a finite width.

The boundary conditions considered here are either:

- periodic: \(\eta(0,t)=\eta(L,t)\) and \(\eta_x(0,t)=\eta_x(L,t)\), or
- homogeneous Neumann: \(\eta_x(0,t)=\eta_x(L,t)=0\).

Both make the boundary term in the variational derivation vanish.

## Functional derivative

Perturb the field by an arbitrary smooth test function \(v(x)\):

\[
\eta\;\mapsto\;\eta+\epsilon v.
\]

The first variation is

\[
\left.\frac{d}{d\epsilon}F[\eta+\epsilon v]\right|_{\epsilon=0}
=\int_0^L\left[
W(\eta^3-\eta)v+\kappa\eta_xv_x
\right]dx.
\]

Integrate the second term by parts:

\[
\int_0^L\kappa\eta_xv_x\,dx
=\left[\kappa\eta_xv\right]_0^L
-\int_0^L\kappa\eta_{xx}v\,dx.
\]

The bracketed boundary contribution is zero for periodic boundaries or for
homogeneous Neumann boundaries.  Hence

\[
\delta F
=\int_0^L
\left[W(\eta^3-\eta)-\kappa\eta_{xx}\right]v\,dx.
\]

By the definition of a functional derivative,

\[
\delta F=\int_0^L\frac{\delta F}{\delta\eta}v\,dx,
\]

so the chemical driving force is

\[
\boxed{\frac{\delta F}{\delta\eta}
=W(\eta^3-\eta)-\kappa\eta_{xx}.}
\]

## Allen--Cahn kinetics

Allen--Cahn dynamics is gradient descent of this free energy in the \(L^2\)
metric.  With a positive kinetic coefficient \(M>0\), posit

\[
\eta_t=-M\frac{\delta F}{\delta\eta}.
\]

Substitution gives

\[
\boxed{
\eta_t=-M\left[W(\eta^3-\eta)-\kappa\eta_{xx}\right]
=M\kappa\eta_{xx}-MW(\eta^3-\eta).
}
\]

The first form emphasizes descent in free energy; the second separates a
diffusive smoothing term from the local double-well reaction term.

## Free-energy dissipation

Differentiate the functional along a sufficiently smooth solution:

\[
\frac{dF}{dt}
=\int_0^L\frac{\delta F}{\delta\eta}\eta_t\,dx.
\]

Inserting the Allen--Cahn law gives

\[
\frac{dF}{dt}
=-\int_0^L M\left(\frac{\delta F}{\delta\eta}\right)^2dx\leq0.
\]

Thus the continuum free energy cannot increase.  Equality occurs only when
the variational derivative is zero everywhere (or more generally where its
product with positive \(M\) is zero), i.e. at a stationary state.

This is a continuous-time statement.  A discrete explicit time integrator
can increase a discrete energy if its time step is too large, so energy
decrease is a verification target within a suitable stable time-step regime,
not an unconditional guarantee of Forward Euler.

## Nonconserved versus conserved dynamics

Integrating the Allen--Cahn equation gives

\[
\frac{d}{dt}\int_0^L\eta\,dx
=-\int_0^L M\frac{\delta F}{\delta\eta}\,dx,
\]

which is not generally zero.  Therefore total order parameter is not an
invariant.  This distinguishes Allen--Cahn dynamics from Cahn--Hilliard
dynamics, where a conservation law written as the divergence of a flux
preserves the integral under zero-flux or periodic boundaries.

## Pytest checks to write later

These are specifications for your own tests, not supplied test code.

1. **Bulk derivative check.**  For representative scalar values of
   \(\eta\), verify analytically that the derivative of
   \(W(\eta^2-1)^2/4\) is \(W(\eta^3-\eta)\).  Check in particular that
   it is zero at \(-1\), \(0\), and \(1\), with \(\pm1\) identified as
   bulk minima and \(0\) as the barrier top.

2. **Uniform-state RHS check.**  A spatially uniform field has
   \(\eta_{xx}=0\).  Therefore its computed right-hand side must reduce to
   \(-MW(\eta^3-\eta)\).  Uniform states \(\eta=-1\), \(0\), and \(1\)
   must have zero right-hand side.  A uniform value strictly between zero and
   one should evolve toward \(+1\); one strictly between \(-1\) and zero
   should evolve toward \(-1\).

3. **Discrete Laplacian of constants.**  Under each supported boundary
   condition, a constant array must have a zero discrete second derivative.
   This isolates boundary handling before testing the full right-hand side.

4. **Periodic/Neumann compatibility.**  Use a smooth field compatible with
   the chosen boundary condition.  Verify the boundary treatment does not
   introduce a spurious boundary source.  For a periodic grid, shifted copies
   of the same sampled field should give correspondingly shifted RHS values.

5. **Free-energy decrease for a stable step.**  Starting from a nonuniform,
   boundary-compatible field, take one or several sufficiently small explicit
   steps.  Compute a discrete energy using the same spatial convention as the
   solver.  It should be no greater after each step, allowing only a
   documented floating-point tolerance.  If this fails, first reduce the time
   step; do not infer that the continuum derivation is wrong.

6. **Stationary-state energy check.**  The uniform fields \(\eta=\pm1\)
   have zero bulk and gradient energy, so their discrete free energy should be
   zero up to roundoff.  Their update should leave them unchanged.

7. **Nonconservation check.**  Select a spatially uniform value that is not
   \(-1\), \(0\), or \(1\).  After a nonzero time step, its discrete mean
   should change.  This guards against accidentally implementing a conserved
   Cahn--Hilliard-like evolution.

8. **Parameter validation.**  Reject nonpositive \(M\), \(W\), or
   \(\kappa\), nonpositive grid spacing, and a negative time step.  Test the
   error behavior specified by the interface you choose.

## Practical verification notes

When a solver is written, define the discrete energy and discrete RHS
consistently.  In particular, use the same grid spacing, gradient stencil,
and boundary convention in both.  Energy tests based on mismatched formulas
can fail even if each formula is individually reasonable.

Record the grid size, \(M\), \(W\), \(\kappa\), boundary condition, initial
field, time step, number of steps, and energy tolerance used by each numerical
test.  These details make a claimed energy decrease reproducible.

## Self-check

Why is conservation of \(\int\eta\,dx\) not expected here, even for periodic
or homogeneous Neumann boundaries?

Because Allen--Cahn kinetics is a local relaxation law,
\(\eta_t=-M\,\delta F/\delta\eta\), rather than the divergence of a flux.
The boundary conditions remove the variational boundary term and support
energy dissipation, but they do not turn the local source/sink term into a
conservative flux.

