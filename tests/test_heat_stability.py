import pytest
import numpy as np
from python.heat_stability import amplification_factor


def test_cornercase_amplification():
    assert amplification_factor(0, 0.25) == pytest.approx(1.0)
    assert amplification_factor(np.pi, 0.5) == pytest.approx(-1.0)
    assert amplification_factor(np.pi, 0.6) == pytest.approx(-1.4)
