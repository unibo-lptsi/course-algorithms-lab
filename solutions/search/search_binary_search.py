def _binary_search_iter(array, x, start, to, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    while start <= to:
        m = (start + to) // 2
        if eq(array[m], x): return m
        elif less(array[m], x): start = m+1
        else: to = m-1
    return -1

def binary_search_iter(array, x, eq = lambda x, y: x == y, less = lambda x, y: x < y):
   return _binary_search_iter(array, x, 0, len(array)-1, eq, less)

def binary_search(array, x):
    return binary_search_iter(array, x)

def _binary_search_rec(array, x, start, to, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    if to < start: return -1
    if start == to:
        return start if eq(array[start], x)  else -1
    mid = start + (to - start)//2
    # print(f"from={start}; to={to}; mid={mid}")
    if eq(array[mid], x): return mid
    elif less(array[mid], x): return _binary_search_rec(array, x, mid+1, to, eq, less)
    else: return _binary_search_rec(array, x, start, mid-1, eq, less)

def binary_search_rec(array, x, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    return _binary_search_rec(array, x, 0, len(array)-1, eq, less)

# Naive recursive variant: instead of narrowing the bounds [start, to] of the portion to examine, it recurs on a slice of the array.
# N.B.: a slice is a NEW list, i.e. a copy of the elements, which takes time (and memory) proportional to its length
def binary_search_rec_slicing(array, x, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    if len(array) == 0: return -1
    mid = len(array) // 2
    if eq(array[mid], x): return mid
    elif less(array[mid], x):
        i = binary_search_rec_slicing(array[mid+1:], x, eq, less)
        return -1 if i == -1 else mid+1 + i # i is an index in the slice, which starts at index mid+1 of array
    else: return binary_search_rec_slicing(array[:mid], x, eq, less) # the slice starts at index 0 of array: same indices

# tests: see test_search_algorithms.py
