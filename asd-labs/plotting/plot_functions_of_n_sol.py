import math # for math.log etc.
import matplotlib.pyplot as plt
import numpy as np
import os

funs_labels = ["n" , "log_e(n)", "nlog(n)", "n^2", "n^3", "gamma(n)" , "n^n"]
# note: The gamma function is an extension of the factorial function to real numbers.
functions = [float, math.log, lambda n: n*math.log(n) , lambda n: n*n, lambda n: n*n*n, math.gamma, lambda n: math.pow(n,n)]
label_pos = [2, 2, 2, -4, 2, 2, 2]

fig, axes = plt.subplots()
fig.set_tight_layout(True)
axes.set_xlabel("n")
axes.set_ylabel("f(n)")
xlimit = 16
ylimit = 80
axes.set_xscale("log")  
axes.set_ylim(0,ylimit)

X = np.linspace(start=1, stop=xlimit, num=100, endpoint=True) # Return evenly spaced numbers over a specified interval.
print(X)

for f_index in range(len(functions)):
    #vfun = np.vectorize(functions[f])
    #y = vfun(X)
    fname = funs_labels[f_index]
    f = functions[f_index]
    y = np.array(list(map(f, X)))
    #plt.yscale("log")  
    #print('\n', fname, '\n', np.array((X,y)).T) # shows all pairs (x,y)
    idx = (np.abs(y - ylimit)).argmin()
    axes.text(X[idx], (ylimit if y[-1] > ylimit else y[-1]) + label_pos[f_index], fname, )
    axes.plot(X, y, label=fname)

axes.legend(loc='upper left')
os.makedirs("./gen", exist_ok=True)
plt.savefig('./gen/functions.pdf', bbox_inches='tight', pad_inches = 0, format='pdf')
plt.show()