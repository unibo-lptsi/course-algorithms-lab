import sys

sizes = [10,100,1000,2000,5000]
sys.setrecursionlimit(2 * max(sizes))

def min_index(a, index_from, index_to): 
    min_idx = index_from
    for i in range(index_from+1, index_to):
        if a[i] < a[min_idx]:
            min_idx = i
    return min_idx

def selection_sort(a):
    for i in range(len(a)-1):
        min_idx = min_index(a, i, len(a))
        a[i], a[min_idx] = a[min_idx], a[i]

def selection_sort2(a):
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

def insert_in_order(arr, n, e):
    pos = n
    while pos > 0 and arr[pos-1] > e:
        arr[pos] = arr[pos-1]
        pos -= 1
    arr[pos] = e

def insertion_sort(a):
    for i in range(1,len(a)):
        insert_in_order(a, i, a[i])

def insertion_sort2(a):
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
def merge2(a, b, r, froma, toa, fromb, tob):
    i = froma
    j = fromb
    k = froma
    while i <= toa and j <= tob:
        if a[i] <= b[j]:
            r[k] = a[i]
            i = i + 1
        else:
            r[k] = b[j]
            j = j + 1
        k = k + 1
    for i in range(i,toa+1):
        r[k] = a[i]
        k = k + 1
    for i in range(j,tob+1):
        r[k] = b[i]
        k = k + 1

def merge_sort2(a, from_index=None, to_index_incl=None, temp=None):
    # Startup
    if from_index is None or to_index_incl is None: 
        from_index = 0
        to_index_incl = len(a)-1
        temp = list(range(0, len(a)))
    # print(f"merge_sort({a[from_index:to_index_incl+1]}, {from_index}, {to_index_incl})")
    # Base case
    alen = to_index_incl - from_index + 1
    if alen<=1: return
    # Recursive case
    m = (from_index + to_index_incl)//2
    merge_sort2(a, from_index, m, temp)
    merge_sort2(a, m+1, to_index_incl, temp)
    # print('Merging', a[from_index:m+1], 'and', a[m+1:to_index_incl+1], end=' ')
    merge2(a, a, temp, from_index, m, m+1, to_index_incl)
    # print('and got ', temp[from_index:to_index_incl+1])
    for i in range(from_index,to_index_incl+1):
        a[i] = temp[i]

def merge(a, b, r):
    i, j, k = 0, 0, 0
    # print(f"merge {a} and {b} into {r}")
    while i < len(a) and j < len(b):
        if a[i] <= b[j]:
            r[k] = a[i]
            i = i + 1
        else:
            r[k] = b[j]
            j = j + 1
        k = k + 1
    for i in range(i,len(a)):
        r[k] = a[i]
        k = k + 1
    for i in range(j,len(b)):
        r[k] = b[i]
        k = k + 1

def merge_sort(a):
    if len(a)<=1: return
    m = len(a)//2
    left = a[:m] # beware: it's a copy
    right = a[m:] # beware: it's a copy
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
    tests = {
        "small random list": (([7, 3, 5, 2, 1, 4, 6],), [1, 2, 3, 4, 5, 6, 7]),
        "sorted list": (([1, 2, 3, 4, 5, 6, 7],), [1, 2, 3, 4, 5, 6, 7]),
        "reverse sorted list": (([7, 6, 5, 4, 3, 2, 1],), [1, 2, 3, 4, 5, 6, 7]),
        "single element list": (([1],), [1]),
        "empty list": (([],), []),
    }
    sorting_algorithms = [selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort, 
                          selection_sort2, insertion_sort2, merge_sort2]
    test_utils.test_all_functions(tests, sorting_algorithms, in_place=True)
