import random
# import timeit

def linear_search(array, x, eq = lambda x, y: x == y):
    for i in range(len(array)):
        if(eq(array[i],x)): 
            return i
    return -1

def random_array(fromN, toN, n, seed = None):
    random.seed(seed)
    return [random.randint(fromN, toN) for i in range(n)]

# tests
for i in range(10):
    input = random_array(0,100, 10 * i)
    x = random.randint(0,100)
    pos = linear_search(input, x)
    print(f"---\nTEST {i}\n---\nINPUT: {input}\nElem to find: {x}\nOutput: {pos}\n")
    
