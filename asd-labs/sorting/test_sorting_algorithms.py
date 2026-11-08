import sys
import unittest
from sorting import selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort


class SortingTests:
    """Tests shared by all the sorting algorithms.

    The specification: sort(a) sorts the list a IN PLACE, in non-decreasing order (and returns nothing).

    This class does not extend unittest.TestCase, so its tests are not run on their own: they are inherited
    and run by the TestCase subclasses below, each of which defines the algorithm under test (field `sort`).
    (Cf. lab search, where the same structure is used for the search algorithms.)
    """

    sort = None # the algorithm under test, i.e. a function sort(a): to be defined by subclasses

    def __init__(self, methodName="runTest"):
        # We rename each test by adding the name of the algorithm under test, e.g. test_empty[quick_sort],
        # so that the report tells which algorithm passes or fails each test (cf. lab search)
        super().__init__(methodName)
        name = f"{methodName}[{self.sort.__name__}]"
        setattr(self, name, getattr(self, methodName))
        self._testMethodName = name

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        print(f"\n=== {cls.sort.__name__} ===", file=sys.stderr)

    def assertSorts(self, a, expected):
        """ Checks that self.sort sorts (a copy of) list a in place, obtaining the list expected """
        actual = a.copy() # we work on a copy, so that the caller's list is left untouched
        self.sort(actual) # in place: the result is in `actual` itself
        self.assertEqual(expected, actual)

    def test_empty(self):
        self.assertSorts([], [])

    def test_singleton(self):
        self.assertSorts([1], [1])

    def test_small(self):
        self.assertSorts([7, 3, 5, 2, 1, 4, 6], [1, 2, 3, 4, 5, 6, 7])

    def test_sorted(self):
        self.assertSorts([1, 2, 3, 4, 5, 6, 7], [1, 2, 3, 4, 5, 6, 7])

    def test_reverse_sorted(self):
        self.assertSorts([7, 6, 5, 4, 3, 2, 1], [1, 2, 3, 4, 5, 6, 7])

    # TODO: add more tests. Some ideas:
    # - lists with two elements, with duplicates, with all equal elements
    # - lists of other types (e.g. negative numbers, floats, strings)
    # - the algorithm modifies the very list it receives (cf. assertIs)
    # - random inputs, using an "oracle" to compute the expected result (cf. lab testing and lab search)
    # - stability: elements with equal keys keep their relative order (hint: elements compared only by a key,
    #   but distinguishable by a label; which of the algorithms are stable?)


# staticmethod: the function is stored as it is, so self.sort(a) does not pass self as first argument
class TestSelectionSort(SortingTests, unittest.TestCase):
    sort = staticmethod(selection_sort)


class TestInsertionSort(SortingTests, unittest.TestCase):
    sort = staticmethod(insertion_sort)


class TestBubbleSort(SortingTests, unittest.TestCase):
    sort = staticmethod(bubble_sort)


class TestMergeSort(SortingTests, unittest.TestCase):
    sort = staticmethod(merge_sort)


class TestQuickSort(SortingTests, unittest.TestCase):
    sort = staticmethod(quick_sort)


if __name__ == "__main__":
    unittest.main(verbosity=2)
