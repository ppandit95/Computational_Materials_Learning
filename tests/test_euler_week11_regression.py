from test_euler_forced import forward_euler_forced, exact_forced_decay
import numpy as np


def test_equilibrium_preservation():
    """
    Verify that the Forward Euler scheme preserves the steady-state solution.

    This test initializes a forced differential equation solver at its
    analytical equilibrium point :math:`u_0 = \\text{source}/\\lambda`. Because
    the system starts exactly at its steady state, the derivative should remain
    zero, and the solution should stay constant across all time steps.

    See Also
    --------
    forward_euler_forced : The function being tested for stability.

    Notes
    -----
    The test simulates a linear forced ODE system of the form:

    .. math:\(: \frac{du}{dt} = \text{source} - \lambda\) u

    When :math:`u(0) = \\frac{\\text{source}}{\\lambda}`, the theoretical
    solution is invariant in time, i.e., :math:`u(t) = u_0` for all :math:`t`.

    The test ensures that numerical drift introduced by the finite time step
    (:math:`dt = 0.1`) remains within strict absolute tolerances.

    Raises
    ------
    AssertionError
        If the numerical solution deviates from the analytical steady-state
        value by more than the specified absolute tolerance of 1.0e-3.
    """
    lam = 2.0
    source = 1.0
    u0 = source / lam
    dt = 0.1
    steps = 100
    approx_sol = forward_euler_forced(u0, lam, source, dt, steps)
    np.testing.assert_allclose(approx_sol, u0, atol=1.0e-3)


def test_difference_propagation():
    """
    Verify the theoretical error and difference propagation of the numerical scheme.

    This test checks that the distance between two trajectories starting from
    different initial conditions, :math:`u_{0,a}` and :math:`u_{0,b}`, matches
    the analytical difference propagation dictated by the linear stability
    factor :math:`(1 - \lambda \Delta t)^n`.

    See Also
    --------
    forward_euler_forced : The numerical solver evaluated for trajectory divergence.

    Notes
    -----
    For a linear forced ODE of the form:

    .. math:\(: \frac{du}{dt} = \text{source} - \lambda\) u

    The Forward Euler updates for two solutions :math:`u^n_a` and :math:`u^n_b`
    satisfy:

    .. math:: u^{n+1}_a - u^{n+1}_b = (\(1 - \lambda \Delta\) t) (u^n_a - u^n_b)

    By induction, the difference at step :math:`n` must satisfy:

    .. math:: u^n_a - u^n_b = (\(1 - \lambda \Delta\) t)^n (u_{0,a} - u_{0,b})

    This test confirms that the implementation preserves this exact discrete
    algebraic relationship across all steps, demonstrating that the forcing
    term cancels out perfectly as expected by linearity.

    Raises
    ------
    AssertionError
        If the computed difference between the two trajectories deviates
        from the expected geometric contraction/propagation formula.
    """
    lam = 2.0
    source = 0.5
    dt = 0.1
    steps = 20
    u0_a = 1.2
    u0_b = 0.3
    approx_sola = forward_euler_forced(u0_a, lam, source, dt, steps)
    approx_solb = forward_euler_forced(u0_b, lam, source, dt, steps)
    n = np.arange(steps + 1)
    expected_difference = (1.0 - lam * dt) ** n * (u0_a - u0_b)
    np.testing.assert_allclose(
        approx_sola - approx_solb, expected_difference, rtol=1e-11, atol=1e-12
    )
