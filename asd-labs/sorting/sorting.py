import sys

sizes = [10,100,1000,2000,5000]
sys.setrecursionlimit(2 * max(sizes))

def selection_sort(a):
    pass

def insertion_sort(a):
    pass

def bubble_sort(a):
    pass

def merge_sort(a):
    pass

def quick_sort(array):
    pass

if __name__ == '__main__':
    import test_utils
    tests = [
        (([7, 3, 5, 2, 1, 4, 6],), [1, 2, 3, 4, 5, 6, 7], 'random list'),
    ]
    sorting_algorithms = [selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort]
    test_utils.test_all(tests, sorting_algorithms)