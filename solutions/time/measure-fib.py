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

def measure_pc(f, n_times=10, n_repeats = 5):
    measures = []
    for _ in range(n_repeats):
        time_start = time.perf_counter()
        for _ in range(n_times):
            f()
        time_end = time.perf_counter()
        measures.append(time_end - time_start)
    return min(measures) / n_times

def measure_timeit(f, n_times = 10, n_repeats = 5):
    exec_times = timeit.repeat(f, number=n_times, repeat=n_repeats)
    return min(exec_times) / n_times

# Measure via time.perf_counter and timeit.repeat
for n in range(5,35,5):
    t_rec = measure_pc(lambda: fib(n))
    t_iter = measure_pc(lambda: fib_iter(n))
    tit_rec = measure_timeit(lambda: fib(n))
    tit_iter = measure_timeit(lambda: fib_iter(n))
    print(f"[perf_counter ] fib_rec({n:2}) took {t_rec:10.4g} sec \t fib_iter({n}) took {t_iter:10.4g} sec")
    print(f"[timeit.repeat] fib_rec({n:2}) took {tit_rec:10.4g} sec \t fib_iter({n}) took {tit_iter:10.4g} sec\n")
