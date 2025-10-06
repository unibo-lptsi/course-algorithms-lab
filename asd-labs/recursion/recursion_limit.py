import sys
# sys.setrecursionlimit(1005)

def fact(n, acc=1):
    if n <= 1: return acc
    return fact(n-1, n*acc)

if __name__ == "__main__":
    print("Factorial of 1000: ", fact(1000))