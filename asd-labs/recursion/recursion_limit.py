import sys
# sys.setrecursionlimit(1000)

def fact(n, acc=1):
    if n <= 1: return acc
    return fact.tail_call(n-1, n*acc)

if __name__ == "__main__":
    print("Factorial of 1000: ", fact(1000))