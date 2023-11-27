import unittest 
import numpy as np
import time
import sys 

sizes = [10,100,1000]
sys.setrecursionlimit(2 * max(sizes))

def selection_sort(a):
    pass

def insertion_sort(a):
    pass

def bubble_sort(a):
    pass

def merge_sort(a):
    pass

def quick_sort(a):
    pass

sorting_algorithms = [selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort]

class TestSortingAlgorithms(unittest.TestCase):

    def test_on_empty_list(self):
        """
        Test sorting on empty list.
        """
        for f in sorting_algorithms:
            with self.subTest(algo=f.__name__):
                a = []
                f(a)
                self.assertEqual([], a)

    def test_on_singleton_list(self):
        """
        Test sorting on singleton list.
        """
        for f in sorting_algorithms:
            with self.subTest(algo=f.__name__):
                a = [7]
                f(a)
                self.assertEqual([7], a)

    def test_on_random_lists(self):
        """
        Test sorting on random lists of increasing size.
        """
        for n in sizes:
            for f in sorting_algorithms:
                with self.subTest(algo=f.__name__):
                    a = np.random.randint(0, 1_000_000, n)
                    expected = np.sort(a).tolist() # return a sorted copy
                    a = a.tolist()
                    start_time = time.perf_counter()
                    f(a)
                    end_time = time.perf_counter()
                    print(f"Algorithm {f.__name__:20} on random array of {n:12} elements took: {end_time-start_time:20.2} s")
                    self.assertEqual(expected, a)
            print("---")

    def test_on_sorted_lists(self):
        """
        Test sorting on sorted lists of increasing size (best case).
        """
        for n in sizes:
            for f in sorting_algorithms:
                with self.subTest(algo=f.__name__):
                    a = list(range(0,n))
                    expected = a.copy()
                    expected.sort() 
                    start_time = time.perf_counter()
                    f(a)
                    end_time = time.perf_counter()
                    print(f"Algorithm {f.__name__:20} on *sorted* array of {n:20} elements took: {end_time-start_time:20} s")
                    self.assertEqual(expected, a)
            print("---")

    def test_on_reverse_ordered_lists(self):
        """
        Test sorting on reverse-ordered lists of increasing size (worst case).
        """
        for n in sizes:
            for f in sorting_algorithms:
                with self.subTest(algo=f.__name__):
                    a = list(range(n,0,-1))
                    expected = a.copy()
                    expected.sort() 
                    start_time = time.perf_counter()
                    f(a)
                    end_time = time.perf_counter()
                    print(f"Algorithm {f.__name__:20} on *sorted* array of {n:20} elements took: {end_time-start_time:20} s")
                    self.assertEqual(expected, a)
            print("---")

if __name__ == '__main__':
    unittest.main()