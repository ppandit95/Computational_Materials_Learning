import marimo

__generated_with = "0.23.16"
app = marimo.App(width="medium")


@app.cell
def _():
    import marimo as mo
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    from pathlib import Path
    from time import perf_counter
    from python.poisson_diagnostics import diagnose_poisson

    return Path, diagnose_poisson, mo, np, pd, perf_counter, plt


@app.cell
def _(mo):
    mo.md(r"""
    # Day 83 — Verification of the 1D Poisson solver

    Manufactured solution: $u(x)=1+2x+\sin(\pi x)$ on $[0,1]$.
    Therefore $-u''(x)=\pi^2\sin(\pi x)$, with $u(0)=1$ and $u(1)=3$.

    We measure the **algebraic residual**, **discretization error**, and
    **exact-solution residual** on successively refined grids. We expect
    $\|u_h-u\|_\infty=O(h^2)$; a small algebraic residual alone does not
    establish continuum accuracy.
    """)
    return


@app.cell
def _(diagnose_poisson, np, pd, perf_counter):
    rows = []
    for n_interior in (15, 31, 63):
        # A warm-up avoids timing the first call in the reported median.
        diagnose_poisson(n_interior)
        durations = []
        for _ in range(10):
            start = perf_counter()
            diagnose_poisson(n_interior)
            durations.append(perf_counter() - start)

        row = diagnose_poisson(n_interior).copy()
        row["median_runtime_ms"] = float(np.median(durations)) * 1000.0
        rows.append(row)

    diagnostics = pd.DataFrame(rows)
    return (diagnostics,)


@app.cell
def _(diagnostics, np):
    # Build a fresh DataFrame rather than mutating an upstream cell's object.
    results = diagnostics.copy()
    spacings = results["h"].to_numpy(dtype=float)
    errors = results["discretization_error_linf"].to_numpy(dtype=float)
    orders = np.full(len(results), np.nan)
    for idx in range(1, len(results)):
        orders[idx] = np.log(errors[idx - 1] / errors[idx]) / np.log(
            spacings[idx - 1] / spacings[idx]
        )
    results["observed_order"] = orders
    return errors, results, spacings


@app.cell
def _(mo, results):
    mo.vstack([mo.md("## Convergence and timing results"), mo.ui.table(results)])
    return


@app.cell
def _(errors, spacings):
    reference = errors[0] * (spacings / spacings[0]) ** 2
    return (reference,)


@app.cell
def _(errors, plt, reference, spacings):
    fig, ax = plt.subplots(figsize=(7, 5))
    ax.loglog(spacings, errors, "o-", label="Numerical discretization error")
    ax.loglog(spacings, reference, "--", label=r"Reference $O(h^2)$")
    ax.set_xlabel("Grid spacing $h$")
    ax.set_ylabel(r"$\|u_h-u_{\mathrm{exact}}\|_\infty$")
    ax.set_title("1D Poisson: second-order grid convergence")
    ax.grid(True, which="both", linestyle=":")
    ax.legend()
    fig.tight_layout()
    return (fig,)


@app.cell
def _(fig, mo):
    mo.as_html(fig)
    return


@app.cell
def _(Path, fig, mo, results):
    # Run Marimo from the repository root so relative paths resolve there.
    output_dir = Path("data")
    output_dir.mkdir(parents=True, exist_ok=True)
    csv_path = output_dir / "day83_poisson_convergence.csv"
    png_path = output_dir / "day83_poisson_convergence.png"
    results.to_csv(csv_path, index=False)
    fig.savefig(png_path, dpi=200, bbox_inches="tight")
    mo.md(f"**Exported:** `{csv_path}` and `{png_path}`")
    return


@app.cell
def _(mo):
    mo.md("""
    ## Interpretation and limitations

    - Compare the measured `observed_order` values with the theoretical order 2.
    - The algebraic residual measures the linear-system solve, **not** mesh accuracy.
    - Timings include grid creation, matrix assembly, sparse solve and diagnostics.
      These tiny grids do not establish a solver complexity law.
    - Run `python -m pytest python/test_poisson_diagnostics.py -v` and
      `ctest --test-dir build --output-on-failure` in a terminal; record actual results.
    - Write `docs/day83_poisson_verification.md` using the **measured** values.
    """)
    return


if __name__ == "__main__":
    app.run()
