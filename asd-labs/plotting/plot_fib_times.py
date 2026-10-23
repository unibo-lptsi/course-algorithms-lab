import timeit # for timeit.repeat
import matplotlib.pyplot as plt

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
    pass # TODO

# TODO HERE

plt.show()
