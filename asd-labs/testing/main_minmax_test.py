import test_utils
import random
from min_max import min_max


# main
if __name__ == "__main__":
    random_list = [random.randint(0, 100) for _ in range(10)]
    tests = {
        "singleton element": (([77],), (77, 77)), 
        "random list of 100 ints": ((random_list,), (min(random_list), max(random_list))), # our oracle is Python's min/max functions
    }
    test_utils.test(tests, min_max)