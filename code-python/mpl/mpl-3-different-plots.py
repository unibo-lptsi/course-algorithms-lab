import numpy as np
import matplotlib.pyplot as plt

n = 20
min_inc, max_inc = -10, +10
x = np.arange(n)
y = np.zeros(n)
increments = np.zeros(n)
for i in np.arange(1,n): # random walk
    increments[i] = np.random.randint(min_inc, max_inc)
    y[i] = y[i-1] + increments[i]

fig = plt.figure(figsize=(12,4))
ax = fig.add_subplot(141) # note: subplot(212) is the same as subplot(nrows=2,ncols=1,index=1)
ax.plot(x, y, color='r')

ax2 = fig.add_subplot(142)
ax2.plot(x, y, 'or')

ax3 = fig.add_subplot(143)
ax3.bar(x, y)

ax4 = fig.add_subplot(144)
ax4.bar(x, increments)

plt.show()
