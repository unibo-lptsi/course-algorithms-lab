import math # cf. math.pow, math.isclose

# Implementare `pow(a,n)` (elevamento a potenza) in modo *ricorsivo*
def pow(a, n):
    if n==0: return 1
    if n < 0:
        return pow(a, n+1) / a
    else:
        return a * pow(a, n-1)

from typing import Callable, Tuple, Sequence, Any # note: Sequence is covariant, while List is invariant

# Note: this is a higher-order function, since it takes a function as input (f is a Callable)
# Note: type hints can be checked via tool `mypy`
def test(f: Callable, tests: Sequence[Tuple[Tuple,Any]], tolerance: float = 0.) -> None:
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

pow_tests = [((a, n), math.pow(a, n)) for a, n in [(0,5), (5,0), (3,3), (2,8), (2,-3)]]
test(pow, pow_tests)