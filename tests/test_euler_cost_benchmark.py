import numpy as np
import pytest
import pandas as pd
from python.euler_cost_benchmark import run_benchmark


# Test A : Error Decreases with increasing timestep count
def test_error_decreases_monotonically():
    benchmark_dataframe = run_benchmark()
    N = np.array(benchmark_dataframe["steps"])
    E = np.array(benchmark_dataframe["absolute_error"])

    assert np.all(E[1:] < E[:-1])


# Test B : Error Approximately halves when timestep doubles
def test_error_halves():
    benchmark_dataframe = run_benchmark()
    N = np.array(benchmark_dataframe["steps"])
    E = np.array(benchmark_dataframe["absolute_error"])
    assert E[1] / E[0] == pytest.approx(0.5, abs=0.05)
