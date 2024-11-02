import random
import test_utils

def linear_search(array, x, eq = lambda x, y: x == y):
    for i in range(len(array)):
        if(eq(array[i],x)): 
            return i
    return -1

def linear_search_rec(array, x, start=None, to=None, eq = lambda x, y: x == y):
    if start is None or to is None: 
        start = 0
        to = len(array)-1
    if start > to: return -1
    if eq(array[start], x): return start
    return linear_search_rec(array, x, start+1, to, eq)

def random_array(fromN, toN, n, seed = None):
    random.seed(seed)
    return [random.randint(fromN, toN) for i in range(n)]

# tests
if __name__ == '__main__':
    for i in range(10):
        input = random_array(0,100, 10 * i)
        x = random.randint(0,100)
        pos = linear_search(input, x)
        pos_rec = linear_search_rec(input, x)
        print(f"---\nTEST {i}\n---\nINPUT: {input}\nElem to find: {x}\nOutput (iter): {pos}\nOutput (rec): {pos_rec}\n")

tests = [(([], 0), -1), 
         (([0], 0), 0), 
         (([0], 1), -1), 
         (([1,3,7], 4), -1), 
         (([1,3,7], 3), 1), 
         ((list(range(1,20)), 19), 18), 
         ((list(range(1,20)), 77), -1)]
print("\n*** LINEAR SEARCH TESTS ***\n")
test_utils.test(tests, linear_search)
print("\n*** LINEAR SEARCH REC TESTS ***\n")
test_utils.test(tests, linear_search_rec)