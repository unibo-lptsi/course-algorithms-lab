import math # cf. math.pow, math.isclose

def pow(a, n):
    pass


def test(f, tests):
    for i, t in enumerate(tests):
        print(f"Test {i}) Testing {f.__name__}({t[0]})")
    pass

pow_tests = [((a, n), math.pow(a, n)) for a, n in [(0,5), (5,0), (3,3), (2,8), (2,-3)]]
test(pow, pow_tests)