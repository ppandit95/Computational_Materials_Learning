import numpy as np
import python.jacobi_grid_analysis as jca


def test_jacobi_iterations_increase_with_grid_size():
    sizes = [8, 16, 32, 64]

    iterations = [jca.predicted_iteration(n, 1e-4) for n in sizes]

    assert iterations == sorted(iterations)
    assert len(set(iterations)) == len(iterations)
