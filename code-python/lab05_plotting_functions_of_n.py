import math # for math.log etc.
import matplotlib.pyplot as plt
import numpy as np
import os

funs_labels = ["n" , "log_e(n)", "nlog(n)", "n^2", "n^3", "gamma(n)" , "n^n"]
# note: The gamma function is an extension of the factorial function to real numbers.
functions = [float, math.log, lambda n: n*math.log(n) , lambda n: n*n, lambda n: n*n*n, math.gamma, lambda n: math.pow(n,n)]
label_pos = [90, 80, 70, 20, 15, 10, 8]

fig, axes = plt.subplots()
fig.set_tight_layout(True)
axes.set_xlabel("n")
axes.set_ylabel("f(n)")

xlimit = 16
ylimit = 80

X = np.linspace(start=1, stop=xlimit, num=100, endpoint=True) # Return evenly spaced numbers over a specified interval.
print(X)
for f in range(len(functions)):
    #vfun = np.vectorize(functions[f])
    #y = vfun(X)
    y = np.array(list(map(functions[f],X)))
    print(y)
    #plt.yscale("log")  
    axes.set_xscale("log")  
    axes.set_ylim(0,ylimit)
    print(funs_labels[f])
    if label_pos[f] != math.nan:
        idx = (np.abs(y - ylimit)).argmin()
        axes.text(X[idx], (ylimit if y[-1] > ylimit else y[-1]) - f*2, funs_labels[f], )
    axes.plot(X,y,label=funs_labels[f])

axes.legend(loc='upper left')
os.mkdir("./gen")
plt.savefig('./gen/functions.pdf',bbox_inches='tight', pad_inches = 0, format='pdf')
plt.show()

