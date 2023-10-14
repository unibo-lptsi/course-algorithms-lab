import random
import sys
# from tail_recursive import tail_recursive # https://pypi.org/project/tail-recursive/

# linear recursion (non-tail)
def fact(n):
    if n <= 0: return 1
    return n * fact(n-1)

for i in range(8):
    print(f"{i}! = {fact(i)}", end=' '*4)

# linear recursion (tail)
def fact_tailrec(n, acc=1):
    if n <= 1: return acc
    return fact_tailrec(n-1, n*acc)

print("\n")

for i in range(8):
    print(f"{i}! = {fact_tailrec(i)}", end=' '*4)

print("\n")

# linear recursion (tail)
def min_rec(lst):
    if len(lst) == 2: return min(lst[0], lst[1])
    if len(lst) == 1: return lst[0]
    if len(lst) <= 0: raise Exception("empty list")
    return min_rec(lst[1:] if lst[0] > lst[-1] else lst[0:-1])

vec = [random.randint(1, 1000) for i in range(10)]
print(vec)
print(f"MIN: {min_rec(vec)}\n")

# multiple recursion
def fib(n):
    if n <= 2: return n
    return fib(n-2) + fib(n-1)

def fib_iter(n):
    if n <= 2: return n
    a, b = 0, 1
    for i in range(0,n):
        a, b = b, a+b
    return b

for i in range(8):
    print(f"fib({i}) = {fib(i)}", end=' '*4)
    print(f"fib_iter({i}) = {fib_iter(i)}", end=' '*4)
    sys.stdout.flush()
    
print("\nfib(35) = ", fib(35))
print("fib_iter(35) = ", fib_iter(35))

print("\n")

# mutual recursion
def even(n):
    print(f"even({n})")
    if n==0: return True
    return odd(n+(-1 if n>0 else +1))

def odd(n):
    print(f"odd({n})")
    if n==0: return False
    return even(n + (-1 if n>0 else +1))

for i in range(-4,8):
    print(f"even({i}) = {even(i)}", end=' '*4)

print("\n")