from euler_decay import forward_euler_decay
import numpy as np
import matplotlib.pyplot as plt

u0 = 1.0
lam = 2.0
steps = 12
dts = np.array([0.2, 0.8, 1.0, 1.1])
sol = np.zeros(steps + 1)
time_index = np.arange(steps + 1)
plt.figure()
for dt in dts:
    sol = forward_euler_decay(u0, lam, dt, steps)
    G = sol[1] / sol[0]
    plt.plot(time_index, sol, label=f"G = {G}")
    plt.legend()

plt.savefig("/home/pushkar/computational_materials_learning/data/eular_stability.png")
