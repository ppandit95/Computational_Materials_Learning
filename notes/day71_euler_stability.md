Let us consider a differential equation:

$$\frac{du}{dt} = -\lambda u, \quad \lambda > 0$$

Using the **Forward Euler formula** for the time derivative:

$$\frac{du}{dt} \approx \frac{u^{n+1} - u^n}{\Delta t}$$

Substituting this back into the differential equation gives:

$$\frac{u^{n+1} - u^n}{\Delta t} = -\lambda u^n$$

Rearranging the equation to solve for $u^{n+1}$:

$$u^{n+1} = (1 - \lambda \Delta t)u^n$$

From this, we identify the **amplification factor** ($G$):

$$G = 1 - \lambda \Delta t$$

The solution will be stable and decay (tend to zero) over time if $|G| < 1$. Therefore:

$$|1 - \lambda \Delta t| < 1$$

This inequality expands to:

$$-1 < 1 - \lambda \Delta t < 1$$

Subtracting $1$ from all parts:

$$-2 < -\lambda \Delta t < 0$$

Multiplying by $-1$ (which flips the inequality signs):

$$0 < \lambda \Delta t < 2$$
