import big_o

def fib(n):
    if n <= 2: return n
    return fib(n-2) + fib(n-1)

def fib_iter(n):
    if n <= 2: return n
    a, b = 0, 1
    for i in range(0,n):
        a, b = b, a+b
    return b

# N.B.: the output of big_o() is sensible to parametrization; consider playing with the different parameters

rec_best, rec_others = big_o.big_o(fib, big_o.datagen.n_, n_repeats=5, min_n = 1, max_n = 25, n_measures = 5)
print("FIB_REC: ", rec_best) # should be: exponential

it_best, it_others = big_o.big_o(fib_iter, big_o.datagen.n_, n_repeats=50, min_n = 1, max_n = 1000, n_measures = 50)
print("FIB_ITER: ", it_best) # should be: linear
