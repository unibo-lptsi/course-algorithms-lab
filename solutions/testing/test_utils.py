import math
from typing import *

# Note: this is a higher-order function, since it takes a function as input (f is a Callable)
# Note: type hints can be checked via tool `mypy`
type TestName = str
type FunArgs = Tuple  # the arguments to a function under test
type FunResult = Any  # the expected result of a test
type FunctionUnderTest = Callable[..., FunResult]  # a function that takes any number of arguments and returns any type
type TestCase = Tuple[FunArgs, FunResult]  # (args_tuple, expected_result)
type TestSpecs = Dict[TestName, TestCase]

def test(tests: TestSpecs, f: FunctionUnderTest, tolerance: float = 0.) -> None:
    """Runs a set of tests over a function-under-test f.

    Arguments:
    tests -- the set of test specifications, as a dict where keys are test names and values are input/output tuples of format (args_tuple, expected_result)
    f -- the function to be tested
    """
    ntests = 0
    nsuccess = 0
    for k, test_name in enumerate(tests):
        test = tests[test_name]
        actual = f(*test[0])
        expected = test[1]
        success = math.isclose(actual, expected, rel_tol=tolerance) if isinstance(actual, float) else actual==expected
        label = '### [PASS]' if success else '!!! [FAIL]'
        print(f"\n{label} TEST {k}: {test_name}\n\n\tSpecification: {test}")
        if not success:
            print(f"\tActual: {actual}")
        ntests += 1
        nsuccess += success
    print(f"\nSUMMARY: {nsuccess}/{ntests} successful tests")
