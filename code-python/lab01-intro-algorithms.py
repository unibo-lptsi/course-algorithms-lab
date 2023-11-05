from math import sqrt
from typing import Callable, Tuple, Sequence, Any # note: Sequence is covariant, while List is invariant

# Note: this is a higher-order function, since it takes a function as input (f is a Callable)
# Note: type hints can be checked via tool `mypy`
def test(tests: Sequence[Tuple[Tuple,Any]], f: Callable) -> None:
  """Runs a set of tests over a function-under-test f.

  Arguments:
  tests -- the set of test specifications, as a sequence of tuples (args_tuple, expected_result)
  f -- the function to be tested
  """
  for test in tests:
      actual = f(*test[0])
      expected = test[1]
      if actual == expected:
          print(f"PASSED: {test}")
      else:
          print(f"!!! FAILED: {test}\nGOT: {actual}")
      print("---")

def mul(a, b):
    result = 0
    for times in range(0, a):
        result += b
    return result

def mcd(a, b):
    while a != b:
        if a > b: a = a - b
        else: b = b - a
    return a

def mcd2(a, b):
    while b:
        a, b = b, a % b
    return a

def prime(n):
    for i in range(2,n//2+1):
        if n % i == 0: return False 
    return True

def prime_better(n):
    # idea: it is sufficient to check divisibility for all odd numbers up to sqrt(n)
    if n<=3: return True
    if n%2 == 0 or n%3 == 0: return False
    for i in range(5,int(sqrt(n))):
        if n % i == 0: return False 
    return True

print("*** MUL TESTS ***")
mul_tests = [((1, 3), 1*3), ((3, 1), 1*3), ((0, 8), 0), ((8, 0), 0), ((6, 4), 6*4)]
test(mul_tests, mul)

print("\n*** MCD TESTS ***\n")
mcd_tests = [((1, 3), 1), ((4, 6), 2), ((12, 6), 6), ((12, 8), 4), ((8, 12), 4)]
test(mcd_tests, mcd)

print("\n*** PRIME TESTS ***\n")
prime_tests = [((1,), True), ((2,), True), ((3,), True), ((4,), False), ((5,), True), ((6,), False), ((13,), True), ((17,), True)]
test(prime_tests, prime)
print("###\n")
test(prime_tests, prime_better)