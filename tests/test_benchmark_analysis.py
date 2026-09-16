from python.benchmark_analysis import compute_scaling_ratios, scaling_exponent
import pytest


def test_compute_scaling_ratios():
    rows = [10_000, 100_000, 1_000_000]
    times = [0.01, 0.10, 1.00]
    Scaling, PSize = compute_scaling_ratios(rows, times)
    size_ratios = [10, 10]
    time_ratios = [10, 10]
    assert Scaling == pytest.approx(time_ratios)
    assert PSize == pytest.approx(size_ratios)
def test_scaling_exponent_for_linear_sublinear_superlinear_exponent():
    rows = [10_000, 100_000, 1_000_000]
    times = [0.01, 0.10, 1.00]
    super = 0
    sub = 0
    linear = 0
    for i in range(1, len(rows)):
        n1 = rows[i - 1]
        n2 = rows[i]
        t1 = times[i - 1]
        t2 = times[i]
        exponent = scaling_exponent(n1, n2, t1, t2)
        if exponent > 1:
            super+=1
        elif exponent < 1:
            sub+=1
        else:
            linear+=1
    assert super == 0
    assert sub == 0
    assert linear == 2



