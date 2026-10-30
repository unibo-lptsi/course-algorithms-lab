import timeit # for timeit.repeat
import matplotlib.pyplot as plt
import numpy as np
import os

def fib(n):
    if n <= 2: return n
    return fib(n-2) + fib(n-1)

def fib_iter(n):
    if n <= 2: return n
    a, b = 0, 1
    for i in range(0,n):
        a, b = b, a+b
    return b

def measure_ms(f, n_times = 3, n_repeats = 5):
    """ Returns the running time of f() in milliseconds (minimum over n_repeats measures, each averaged over n_times runs) """
    exec_times = timeit.repeat(f, number=n_times, repeat=n_repeats)
    return min(exec_times) / n_times * 1000

N = list(range(1, 35))
t_rec = [measure_ms(lambda: fib(n)) for n in N]
t_iter = [measure_ms(lambda: fib_iter(n)) for n in N]
for n, tr, ti in zip(N, t_rec, t_iter):
    print(f"n={n:2}  fib: {tr:10.4g} ms \t fib_iter: {ti:10.4g} ms")

fig, (ax_lin, ax_log) = plt.subplots(1, 2, figsize=(11, 4.5))
fig.set_tight_layout(True)
fig.suptitle("Tempo d'esecuzione di Fibonacci: ricorsivo vs. iterativo")
for axes, scale in [(ax_lin, "linear"), (ax_log, "log")]:
    axes.plot(N, t_rec, marker="o", markersize=4, label="fib (ricorsivo)")
    axes.plot(N, t_iter, marker="s", markersize=4, label="fib_iter (iterativo)")
    axes.set_yscale(scale)
    axes.set_title(f"Asse y in scala {'lineare' if scale == 'linear' else 'logaritmica'}")
    axes.set_xlabel("n")
    axes.set_ylabel("tempo (ms)")
    axes.grid(True, which="major", alpha=0.3)
    axes.legend(loc="upper left")

os.makedirs("./gen", exist_ok=True)
plt.savefig('./gen/fib_times.pdf', bbox_inches='tight', format='pdf')
plt.show()
