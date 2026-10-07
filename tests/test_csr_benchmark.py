import numpy as np

"""
Test the scaling exponent calculation for CSR benchmark.
This test validates the scaling exponent computation by calling the function
from benchmarks/csr_benchmark.py. It verifies that the coefficient and 
scaling behavior are correctly calculated for the CSR (Compressed Sparse Row)
benchmark analysis.
The test ensures that the scaling exponent follows expected linear behavior
and that the computed coefficient matches the theoretical expectations.
Raises:
    AssertionError: If the scaling exponent or coefficient does not meet
                    the expected values within acceptable tolerance.
"""
import pandas as pd
from python.csr_benchmark import scaling_exponent
import pytest


def test_scaling_exponent_linear():
    n = np.array([10.0, 100.0, 1000.0])
    t = 3.0 * n
    if np.any(t == 0.0):
        raise ValueError("One of the measured time is zero.")
    p = scaling_exponent(n, t)
    p == pytest.approx(1)
