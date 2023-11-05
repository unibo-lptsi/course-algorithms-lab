import random
# import timeit

def binary_search_recur(array, x, start, to, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    if start - to == 0 or to < start: return -1
    if start == to:
        return start if eq(array[start], x)  else -1
    mid = start + (to - start)//2
    print(f"from={start}; to={to}; mid={mid}")
    if eq(array[mid], x): return mid
    elif less(array[mid], x): return binary_search_recur(array, x, mid+1, to)
    else: return binary_search_recur(array, x, start, mid-1)

def binary_search_iter(array, x, start, to, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    while start <= to:
        m = (start + to) // 2
        if eq(array[m], x): return m
        elif less(array[m], x): start = m+1
        else: to = m-1
    return -1

def binary_search(array, x):
    return binary_search_iter(array, x, 0, len(array)-1)

def random_array(fromN, toN, n, seed = None):
    random.seed(seed)
    return [random.randint(fromN, toN) for i in range(n)]

# tests
if __name__ == '__main__':
    for i in range(10):
        input = random_array(0,100, 10 * i)
        input.sort()
        x = random.randint(0,100)
        pos = binary_search_recur(input, x, 0, len(input)-1)
        print(f"---\nTEST (rec) {i}\n---\nINPUT: {input}\nElem to find: {x}\nOutput: {pos}\n")

        pos = binary_search_iter(input, x, 0, len(input)-1)
        print(f"---\nTEST (iter) {i}\n---\nINPUT: {input}\nElem to find: {x}\nOutput: {pos}\n")
        
