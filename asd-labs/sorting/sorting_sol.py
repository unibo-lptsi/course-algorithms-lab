import sys

sizes = [10,100,1000,2000,5000]
sys.setrecursionlimit(2 * max(sizes))

def selection_sort(a):
    sorted_idx = -1
    unsorted_idx = 0
    arr_len = len(a)
    # Selection sort loop
    while sorted_idx < arr_len - 1:
        min_idx = unsorted_idx
        for i in range(unsorted_idx, arr_len):
            if a[i] < a[min_idx]:
                min_idx = i
        a[unsorted_idx], a[min_idx] = a[min_idx], a[unsorted_idx]
        sorted_idx = unsorted_idx
        unsorted_idx += 1

def insertion_sort(a):
    for i in range(1,len(a)):
        e = a[i]
        j = i-1
        while j >= 0 and a[j] > e:
            a[j+1] = a[j]
            j = j-1
        a[j+1] = e

def bubble_sort(a):
    swap = True
    n = len(a)
    i = 0
    while swap and i < n-1:
        swap = False
        for j in range(n-1-i):
            if a[j] > a[j+1]:
                swap = True
                a[j], a[j+1] = a[j+1], a[j]
        i += 1

# merge ordered sequences a and b into r
def merge(a, b, r):
    na = 0 
    nb = 0
    k = 0
    # print(f"merge {a} and {b} into {r}")
    while na < len(a) and nb < len(b):
        if a[na] <= b[nb]:
            r[k] = a[na]
            na = na + 1
        else:
            r[k] = b[nb]
            nb = nb + 1
        k = k + 1
    for i in range(na,len(a)):
        r[k] = a[i]
        k = k + 1
    for i in range(nb,len(b)):
        r[k] = b[i]
        k = k + 1

def merge_sort(a):
    if len(a)<=1: return
    m = len(a)//2
    left = a[:m]
    right = a[m:]
    merge_sort(left)
    merge_sort(right)
    merge(left, right, a)

# partition around a pivot: lhs are lte wrt pivot, rhs are gte wrt pivot; returns position of pivot
def partition(a,start,to):
    pivot = a[start]
    k = start+1
    for i in range(start+1,to):
        if a[i] < pivot:
            a[i], a[k] = a[k], a[i]
            k += 1
    a[start] = a[k-1]
    a[k-1] = pivot
    return k-1

# sort array
def quick_sort(array, start=None, to=None):
    start = 0 if start==None else start
    to = len(array) if to==None else to
    if start < to:
        pivot_index = partition(array,start,to)
        quick_sort(array,start,pivot_index)
        quick_sort(array,pivot_index+1,to)

if __name__ == '__main__':
    import test_utils
    tests = [
        (([7, 3, 5, 2, 1, 4, 6],), [1, 2, 3, 4, 5, 6, 7], 'random list'),
        (([1, 2, 3, 4, 5, 6, 7],), [1, 2, 3, 4, 5, 6, 7], 'sorted list'),
        (([7, 6, 5, 4, 3, 2, 1],), [1, 2, 3, 4, 5, 6, 7], 'reverse sorted list'),
        (([1],), [1], 'single element list'),
        (([],), [], 'empty list'),
    ]
    sorting_algorithms = [selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort]
    test_utils.test_all(tests, sorting_algorithms)