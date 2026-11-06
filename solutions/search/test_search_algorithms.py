import sys
import unittest
import random
from search_linear_search import linear_search, linear_search_rec
from search_binary_search import binary_search, binary_search_rec, binary_search_rec_slicing


class SearchTests:
    """Tests shared by all the search algorithms.

    The specification: search(array, x) returns an index i such that array[i] == x, or -1 if x is not in array.
    All the inputs are SORTED arrays, since this is a precondition of binary search.

    This class does not extend unittest.TestCase, so its tests are not run on their own: they are inherited
    and run by the TestCase subclasses below, each of which defines the algorithm under test (field `search`).
    """

    search = None # the algorithm under test, i.e. a function search(array, x): to be defined by subclasses

    def __init__(self, methodName="runTest"):
        # unittest creates an object of the test class for each test method, whose name is passed as methodName.
        # super() is the next class in the MRO of the subclasses, i.e. unittest.TestCase
        super().__init__(methodName)
        # We rename the test by adding the name of the algorithm under test, e.g. test_empty[binary_search_rec],
        # so that the report tells which algorithm passes or fails each test.
        # unittest looks up the test method by its name (attribute _testMethodName), so we also add the method under the new name
        name = f"{methodName}[{self.search.__name__}]"
        setattr(self, name, getattr(self, methodName))
        self._testMethodName = name

    @classmethod
    def setUpClass(cls):
        # Run once before all the tests of a class ("before all"); tearDownClass is the corresponding "after all".
        # cls is the concrete TestCase subclass, so cls.search is its algorithm under test.
        super().setUpClass()
        # We print a header for the tests of the algorithm. We print on stderr, where the unittest runner writes its report,
        # so that the header and the report are not interleaved incorrectly when the output is redirected to a file
        print(f"\n=== {cls.search.__name__} ===", file=sys.stderr)

    def test_empty(self):
        self.assertEqual(-1, self.search([], 7))

    def test_singleton_present(self):
        self.assertEqual(0, self.search([7], 7))

    def test_singleton_absent(self):
        self.assertEqual(-1, self.search([7], 3)) # smaller than the only element
        self.assertEqual(-1, self.search([7], 9)) # greater than the only element

    def test_first_and_last(self):
        array = [1, 3, 5, 7, 9, 11]
        self.assertEqual(0, self.search(array, 1))
        self.assertEqual(5, self.search(array, 11))

    def test_absent(self):
        array = [1, 3, 5, 7, 9, 11]
        for x in [0, 4, 12]: # before the first element, in between, after the last element
            with self.subTest(x=x): # a failing subtest is reported (with x), and the others are still run
                self.assertEqual(-1, self.search(array, x))

    def test_every_position(self):
        # every element (and every gap) of arrays of both even and odd length:
        # a frequent source of off-by-one errors in binary search
        for n in range(1, 20):
            array = list(range(0, 2*n, 2)) # [0, 2, 4, ..., 2n-2]: the odd numbers are not in the array
            for i, x in enumerate(array):
                with self.subTest(n=n, x=x):
                    self.assertEqual(i, self.search(array, x))
            for x in range(-1, 2*n, 2):
                with self.subTest(n=n, x=x):
                    self.assertEqual(-1, self.search(array, x))

    def test_duplicates(self):
        # the specification requires *an* index of x, not the first one (binary search may return any of them)
        array = [1, 2, 2, 2, 2, 3, 5]
        self.assertIn(self.search(array, 2), [1, 2, 3, 4])
        self.assertEqual(-1, self.search(array, 4))

    def test_strings(self):
        # any type supporting == and < can be searched
        array = ["ape", "bue", "cane", "gatto", "topo"]
        self.assertEqual(3, self.search(array, "gatto"))
        self.assertEqual(-1, self.search(array, "lupo"))

    def test_input_not_modified(self):
        array = [1, 3, 5, 7]
        self.search(array, 5)
        self.assertEqual([1, 3, 5, 7], array)

    def test_random(self):
        rnd = random.Random(42) # random generator with a fixed seed: deterministic (reproducible) tests
        for _ in range(100):
            # random sorted arrays (possibly empty, possibly with duplicates)
            array = sorted(rnd.randint(0, 50) for _ in range(rnd.randint(0, 100)))
            x = rnd.randint(-5, 55)
            with self.subTest(array=array, x=x):
                i = self.search(array, x)
                if x in array: # our oracle is Python's `in` operator
                    self.assertTrue(0 <= i < len(array))
                    self.assertEqual(x, array[i])
                else:
                    self.assertEqual(-1, i)


class TestLinearSearch(SearchTests, unittest.TestCase):
    # staticmethod: the function is stored as it is, so self.search(array, x) does not pass self as first argument
    search = staticmethod(linear_search)

    # tests specific to linear search can be added here: e.g., it does not need a sorted array
    def test_unsorted(self):
        array = [7, 3, 9, 1, 4]
        self.assertEqual(3, self.search(array, 1))
        self.assertEqual(-1, self.search(array, 5))

    def test_first_occurrence(self):
        # scanning from left to right, linear search finds the first occurrence
        self.assertEqual(0, self.search([4, 2, 4, 4], 4))


class TestBinarySearch(SearchTests, unittest.TestCase):
    search = staticmethod(binary_search)

    def test_less_not_weak_order(self):
        # A "negative" test, i.e. on an input that violates the precondition. Binary search needs the array to be sorted
        # w.r.t. a strict weak order <, such that two elements are equal (==) iff neither is < than the other
        # (e.g., < on numbers or strings). For sets, < is the proper subset relation, which is not a weak order:
        # {2} is incomparable with both {1} and {1, 3}, but {1} < {1, 3} (incomparability is not transitive)
        array = [{1}, {2}, {1, 3}]
        # The array is sorted: no element is < than a previous one...
        self.assertFalse(any(array[j] < array[i] for i in range(len(array)) for j in range(i+1, len(array))))
        # ...but binary search misses some element of it: comparing x with the middle element {2} gives the same outcome
        # for x = {1} and x = {1, 3} (neither ==, nor <, nor >), so the search goes on the same side for both, missing one
        self.assertIn(-1, [self.search(array, x) for x in array])


# The recursive variants must pass the same tests as the iterative ones (including the specific ones)
class TestLinearSearchRec(TestLinearSearch):
    search = staticmethod(linear_search_rec)


class TestBinarySearchRec(TestBinarySearch):
    search = staticmethod(binary_search_rec)


class TestBinarySearchRecSlicing(TestBinarySearch):
    search = staticmethod(binary_search_rec_slicing)


if __name__ == "__main__":
    unittest.main(verbosity=2)
