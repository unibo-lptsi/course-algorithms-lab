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

# tests: see test_search_algorithms.py
