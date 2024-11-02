import math
from typing import *

# Note: this is a higher-order function, since it takes a function as input (f is a Callable)
# Note: type hints can be checked via tool `mypy`
def test(tests: Sequence[Tuple[Tuple,Any]], f: Callable, tolerance: float = 0.) -> None:
  """Runs a set of tests over a function-under-test f.

  Arguments:
  tests -- the set of test specifications, as a sequence of tuples (args_tuple, expected_result)
  f -- the function to be tested
  """
  for k, test in enumerate(tests):
      actual = f(*test[0])
      expected = test[1]
      success = math.isclose(actual, expected, rel_tol=tolerance) if isinstance(actual, float) else actual==expected
      print(f"TEST {k}) ", end="")
      if success:
          print(f"PASSED: {test}")
      else:
          print(f"!!! FAILED: {test}\nGOT: {actual}")
