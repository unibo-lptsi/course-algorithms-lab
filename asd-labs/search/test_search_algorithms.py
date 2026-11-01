import sys
import unittest
from search_linear_search import linear_search, linear_search_rec
from search_binary_search import binary_search, binary_search_rec


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

    # TODO: add more tests. Some ideas:
    # - every element (and every missing element) of arrays of both even and odd length
    # - arrays with duplicate elements: what result should we expect?
    # - arrays of other types (e.g. strings)
    # - the input array is not modified
    # - random inputs, using an "oracle" to compute the expected result (cf. lab testing)


class TestLinearSearch(SearchTests, unittest.TestCase):
    # staticmethod: the function is stored as it is, so self.search(array, x) does not pass self as first argument
    search = staticmethod(linear_search)

    # tests specific to linear search can be added here: e.g., it does not need a sorted array
    def test_unsorted(self):
        array = [7, 3, 9, 1, 4]
        self.assertEqual(3, self.search(array, 1))
        self.assertEqual(-1, self.search(array, 5))


class TestBinarySearch(SearchTests, unittest.TestCase):
    search = staticmethod(binary_search)

    # TODO (optional): a "negative" test, showing that binary search may fail if the relation < between the elements
    #      is not a strict weak order, even if the array is sorted (hint: Python's < on sets is the proper subset relation)


# TODO (part 3): test linear_search_rec and binary_search_rec


if __name__ == "__main__":
    unittest.main(verbosity=2)
