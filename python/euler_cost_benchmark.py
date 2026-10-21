import numpy as np
import pandas as pd

from pathlib import Path
from time import perf_counter

from python.euler_forced import (
    forward_euler_forced,
    exact_forced_decay,
)


def run_benchmark():
    """
    Benchmark Forward Euler accuracy and execution time.

    Returns
    -------
    pandas.DataFrame
        Benchmark results containing timestep counts,
        timestep sizes, absolute errors, and median runtimes.
    """

    u0 = 1.0
    lam = 2.0
    source = 1.0
    T = 1.0

    step_counts = [2000, 4000, 8000, 16000, 32000]

    data = []

    # Calculate analytical reference solution once.
    exact_sol = exact_forced_decay(
        u0=u0,
        lam=lam,
        source=source,
        times=np.array([T]),
    )

    exact_final = exact_sol[-1]

    for N in step_counts:

        dt = T / N

        # Warm-up execution.
        forward_euler_forced(
            u0=u0,
            lam=lam,
            source=source,
            dt=dt,
            steps=N,
        )

        # Independent timings for this problem size.
        timings = []

        for _ in range(7):

            start = perf_counter()

            approx_sol = forward_euler_forced(
                u0=u0,
                lam=lam,
                source=source,
                dt=dt,
                steps=N,
            )

            elapsed = perf_counter() - start

            timings.append(elapsed)

        # Median of seven independent measurements.
        median_runtime = float(np.median(timings))

        # Final-time global numerical error.
        abs_error = float(abs(approx_sol[-1] - exact_final))

        data.append(
            {
                "steps": N,
                "dt": dt,
                "absolute_error": abs_error,
                "median_seconds": median_runtime,
            }
        )

    return pd.DataFrame(data)


if __name__ == "__main__":

    df = run_benchmark()

    # Repository-relative output directory.
    repo_root = Path(__file__).resolve().parent.parent

    output_dir = repo_root / "data"
    output_dir.mkdir(parents=True, exist_ok=True)

    output_file = output_dir / "day76_euler_cost.csv"

    df.to_csv(output_file, index=False)

    print(df.to_string(index=False))

    print(f"\nBenchmark results saved to: {output_file}")
