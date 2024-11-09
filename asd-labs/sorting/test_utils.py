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
    passed = 0
    for k, test in enumerate(tests):
        # copy input
        inputs = [x.copy() for x in test[0]] # shallow copy of arguments
        f(*inputs) # sorting algorithms work in place
        actual = inputs[0] # the result is stored in the first argument
        expected = test[1]
        name = (test[2]+' ') if len(test) > 2 else ""
        success = math.isclose(actual, expected, rel_tol=tolerance) if isinstance(actual, float) else actual==expected
        print(f"- TEST {k}) {name}", end="")
        if success:
            print(f"\n\tPASSED: {test}")
            passed += 1
        else:
            print(f"\n\t!!! FAILED: {test}\n\tGOT: {actual}")
    print(f"\nSUMMARY: {passed}/{len(tests)} tests passed")

def test_all(tests: Sequence[Tuple[Tuple,Any]], fs: List[Callable], tolerance: float = 0.) -> None:
    for f in fs:
        print(f"\n### Testing function {f.__name__}\n")
        test(tests, f, tolerance)
