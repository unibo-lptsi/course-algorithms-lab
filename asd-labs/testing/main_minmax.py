import random
from min_max import min_max

def test(lst, name="untitled"):
    print(f"### Test {name}")
    print(f"INPUT: {str(lst)}")
    min, max = min_max(lst)
    print(f"OUTPUT: Min = {min}, Max = {max}")

# main
if __name__ == "__main__":
    # generate random list
    random_list = [random.randint(0, 100) for _ in range(10)]
    test(random_list, "Test on random list")