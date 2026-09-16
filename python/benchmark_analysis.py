import matplotlib.pyplot as plt
import numpy as np


def compute_scaling_ratios(rows, times):
    """
    Calculate scaling ratios for benchmark analysis.

    Parameters
    ----------
    arr1 : rows : array_like
        The first input array containing the number of rows in an array written to CSV.
    arr2 : times : array_like
        The second input array containing the time taken to write the corresponding number of rows to CSV.

    Returns
    -------
    numpy.ndarray,numpy.ndarray
        2 arrays containing the calculated scaling ratios as well as problem sizes during the benchmarking exercise

    Raises
    ------
    ValueError
        If `rows` and `times` do not have the same length

    Notes
    -----
    The input arrays must satisfy the following constraints:
    - **Equal Length:** `len(rows) == len(times)`
    """
    if len(rows) != len(times):
        raise ValueError("rows and times must have the same length")
    ratios_scaling = []
    ratios_psize = []
    for i in range(1, len(rows)):
        ratio = times[i] / times[i - 1]
        ratios_scaling.append(ratio)
        ratio = rows[i] / rows[i - 1]
        ratios_psize.append(ratio)
    return ratios_scaling, ratios_psize

def scaling_exponent(n1,n2,t1,t2):
    """
        Calculate scaling ratios for benchmark analysis.
    
        Parameters
        ----------
        n1 : int
            The first input integer representing the number of rows in an array written to CSV.
        n2 : int
            The second input integer representing the number of rows in an array written to CSV.
        t1 : float
            The first input float representing the time taken to write the corresponding number of rows to CSV.
        t2 : float
            The second input float representing the time taken to write the corresponding number of rows to CSV.

        Returns
        -------
        float
            The calculated scaling exponent

        Raises
        ------
        ValueError
            If `n1`, `n2`, `t1`, or `t2` are not positive

        Notes
        -----
        The input arrays must satisfy the following constraints:
        - **Equal Length:** `len(rows) == len(times)`
        """
    if n1 <= 0 or n2 <= 0 or t1 <= 0 or t2 <= 0:
        raise ValueError("All inputs must be positive")
    return np.log(t2/t1)/np.log(n2/n1)