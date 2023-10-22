import numpy as np
import matplotlib.pyplot as plt

x = np.arange(200)
y1 = np.zeros(200)
y2 = np.zeros(200)
for i in np.arange(1,200): # random walk
    y1[i] = y1[i-1] + np.random.randint(-20,20)
    y2[i] = y2[i-1] + np.random.randint(-20,20)
fig = plt.figure(figsize=(9,6))
ax = fig.add_subplot(211) # note: subplot(212) is the same as subplot(nrows=2,ncols=1,index=1)
ax.plot(x, y1, color='r')
ax2 = fig.add_subplot(212)
ax2.plot(x, y2, linewidth=3.0)
plt.show()