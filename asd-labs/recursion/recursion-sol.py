import math
from typing import Callable, Tuple, Sequence, Any # note: Sequence is covariant, while List is invariant

# Note: this is a higher-order function, since it takes a function as input (f is a Callable)
# Note: type hints can be checked via tool `mypy`
def test(tests: Sequence[Tuple[Tuple,Any]], f: Callable, tolerance: float = 0.) -> None:
  """Runs a set of tests over a function-under-test f.

  Arguments:
  tests -- the set of test specifications, as a sequence of tuples (args_tuple, expected_result)
  f -- the function to be tested
  """
  for test in tests:
      actual = f(*test[0])
      expected = test[1]
      success = math.isclose(actual, expected, rel_tol=tolerance) if isinstance(actual, float) else actual==expected
      if success:
          print(f"PASSED: {test}")
      else:
          print(f"!!! FAILED: {test}\nGOT: {actual}")
      print("---")

# Implementare `sum_numbers(a,b)` (somma di tutti i numeri interi compresi tra `a` e `b`) in modo *ricorsivo*
def sum_numbers(a,b):
    if a==b: return a
    if a > b: 
        return a + sum_numbers(a-1, b)
    else:
        return b + sum_numbers(a, b-1)

# Implementare `pow(a,n)` (elevamento a potenza) in modo *ricorsivo*
def pow(a, n):
    if n==0: return 1
    if n < 0:
        return pow(a, n+1) / a
    else:
        return a * pow(a, n-1)

# Implementare `list_contains(lst,elem)` (funzione che restituisce `True` 
# se `elem` è contenuto nella lista `lst` o `False` altrimenti) in modo *ricorsivo*
def list_contains(lst, elem):
    if len(lst) == 0: return False
    if lst[0] == elem: return True
    return list_contains(lst[1:], elem)

# Implementare `palindrome(string)` (funzione che restituisce `True` se `string` è una stringa palindroma) 
# in modo *ricorsivo*
def palindrome(s):
    if len(s)==0: return True
    return False if s[0]!=s[-1] else palindrome(s[1:-1])

# Implementare `filter(lst,pred)` (funzione che restituisce una nuova lista con soli gli element idi `lst` 
# che soddisfano la funzione predicato `pred`) in modo ricorsivo
def list_filter(lst,pred):
    if len(lst)==0: return []
    return ([lst[0]] if pred(lst[0]) else []) + list_filter(lst[1:], pred)

print("\n*** SUM_NUMBERS TESTS ***\n")
sum_numbers_tests = [((1, n), n*(n+1)//2) for n in range(1,10)]
test(sum_numbers_tests, sum_numbers)

print("\n*** POW TESTS ***\n")
pow_tests = [((a, n), math.pow(a, n)) for a, n in [(0,5), (5,0), (3,3), (2,8), (2,-3)]]
test(pow_tests, pow)

print("\n*** LIST_CONTAINS TESTS ***\n")
contains_tests = [(([],0), False), (([0],0), True), (([0],1), False), 
                  (([1,3,7],4), False), (([1,3,7],3), True), ((list(range(1,20)),19), True), ((list(range(1,20)),77), False)]
test(contains_tests, list_contains)

print("\n*** PALINDROME TESTS ***\n")
palindrome_tests = [(("emme",), True), (("emmE",), False), (("emm e",), False), (("siris",), True), (("siri",), False)]
test(palindrome_tests, palindrome)

print("\n*** LIST FILTER TESTS ***\n")
filter_tests = [((list(range(0,10)),lambda x: x%2==0), [0,2,4,6,8]),
                ((list(range(0,10)),lambda x: x%3==0), [0,3,6,9]),
                ((list(range(0,10)),lambda x: x>5), [6,7,8,9]),
                ((list(range(0,10)),lambda x: x<0), [])]
test(filter_tests, list_filter)
