import time   # for time.perf_counter
import timeit # for timeit.repeat

def fib(n):
    if n <= 2: return n
    return fib(n-2) + fib(n-1)

def fib_iter(n):
    if n <= 2: return n
    a, b = 0, 1
    for i in range(0,n):
        a, b = b, a+b
    return b

def measure_pc(f, n_times=10):
    cum_time = 0
    for _ in range(n_times):
        time_start = time.perf_counter()
        f()
        time_end = time.perf_counter()
        cum_time += time_end - time_start
    return cum_time / n_times

def measure_timeit(f, n_times=10):
    exec_times = timeit.repeat(f, number=n_times, repeat=5)
    return min(exec_times) / n_times

# Measure via time.perf_counter
for n in range(5,35,5):
    t_rec = measure_pc(lambda: fib(n))
    t_iter = measure_pc(lambda: fib_iter(n))
    tit_rec = measure_timeit(lambda: fib(n))
    tit_iter = measure_timeit(lambda: fib_iter(n))
    print(f"[perf_counter ] fib_rec({n}) took {t_rec:10} sec \t fib_iter({n}) took {t_iter:10} sec")
    print(f"[timeit.repeat] fib_rec({n}) took {tit_rec:10} sec \t fib_iter({n}) took {tit_iter:10} sec\n")

