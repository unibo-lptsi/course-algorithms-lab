import sys
import unittest
import random
from sorting import selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort
from sorting import selection_sort2, insertion_sort2, merge_sort2


class Item:
    """An element with a sort key and a label: items are compared ONLY by key (the label is ignored).

    Useful to test stability: items with equal keys are indistinguishable for the algorithm,
    but we can tell them apart (and thus check their relative order) by their label.
    """
    def __init__(self, key, label):
        self.key = key
        self.label = label

    # the algorithms only use <, <=, >, >= (not ==)
    def __lt__(self, other): return self.key < other.key
    def __le__(self, other): return self.key <= other.key
    def __gt__(self, other): return self.key > other.key
    def __ge__(self, other): return self.key >= other.key

    def __repr__(self):
        return f"{self.key}{self.label}"


class SortingTests:
    """Tests shared by all the sorting algorithms.

    The specification: sort(a) sorts the list a IN PLACE, in non-decreasing order (and returns nothing).

    This class does not extend unittest.TestCase, so its tests are not run on their own: they are inherited
    and run by the TestCase subclasses below, each of which defines the algorithm under test (field `sort`).
    (Cf. lab search, where the same structure is used for the search algorithms.)
    """

    sort = None # the algorithm under test, i.e. a function sort(a): to be defined by subclasses
    stable = True # whether the algorithm under test is stable: subclasses may override it

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

    def test_two_elements(self):
        self.assertSorts([2, 1], [1, 2])
        self.assertSorts([1, 2], [1, 2])

    def test_duplicates(self):
        self.assertSorts([3, 1, 2, 3, 1, 2, 3], [1, 1, 2, 2, 3, 3, 3])

    def test_all_equal(self):
        self.assertSorts([5, 5, 5, 5], [5, 5, 5, 5])

    def test_negative_and_float(self):
        self.assertSorts([0.5, -3, 2, -1.5, 0], [-3, -1.5, 0, 0.5, 2])

    def test_strings(self):
        # any type supporting comparisons can be sorted
        self.assertSorts(["topo", "cane", "ape", "gatto", "bue"], ["ape", "bue", "cane", "gatto", "topo"])

    def test_in_place(self):
        # the algorithm must modify the list it receives (the same object), not a copy of it
        a = [3, 1, 2]
        alias = a
        self.sort(a)
        self.assertIs(alias, a)
        self.assertEqual([1, 2, 3], alias)

    def test_random(self):
        rnd = random.Random(42) # random generator with a fixed seed: deterministic (reproducible) tests
        for _ in range(100):
            # random lists (possibly empty, possibly with duplicates)
            a = [rnd.randint(-50, 50) for _ in range(rnd.randint(0, 100))]
            with self.subTest(a=a):
                self.assertSorts(a, sorted(a)) # our oracle is Python's sorted()

    def test_stable(self):
        # A sorting algorithm is STABLE if elements with equal keys keep their original relative order.
        # We sort items compared only by key; labels tell equal-key items apart.
        # Python's sorted() is stable, so it is our oracle (also for the order of the labels).
        if not self.stable:
            self.skipTest(f"{self.sort.__name__} is not stable")
        rnd = random.Random(42)
        cases = [[Item(2, "a"), Item(1, "b"), Item(2, "c"), Item(1, "d")]]
        # random lists with few distinct keys, hence many duplicates
        cases += [[Item(rnd.randint(0, 5), i) for i in range(rnd.randint(0, 50))] for _ in range(100)]
        for items in cases:
            with self.subTest(items=items):
                actual = items.copy()
                self.sort(actual)
                # we compare (key, label) pairs: Item does not define ==
                self.assertEqual([(x.key, x.label) for x in sorted(items)],
                                 [(x.key, x.label) for x in actual])


# staticmethod: the function is stored as it is, so self.sort(a) does not pass self as first argument
class TestSelectionSort(SortingTests, unittest.TestCase):
    sort = staticmethod(selection_sort)
    stable = False # the swap may move an element past others equal to it, e.g. [2a, 2b, 1] -> [1, 2b, 2a]


class TestInsertionSort(SortingTests, unittest.TestCase):
    sort = staticmethod(insertion_sort)


class TestBubbleSort(SortingTests, unittest.TestCase):
    sort = staticmethod(bubble_sort)


class TestMergeSort(SortingTests, unittest.TestCase):
    sort = staticmethod(merge_sort)


class TestQuickSort(SortingTests, unittest.TestCase):
    sort = staticmethod(quick_sort)
    stable = False # partitioning swaps elements over long distances, e.g. [1, 2a, 2b, 0] -> [0, 1, 2b, 2a]


# The variants must pass the same tests
class TestSelectionSort2(SortingTests, unittest.TestCase):
    sort = staticmethod(selection_sort2)
    stable = False


class TestInsertionSort2(SortingTests, unittest.TestCase):
    sort = staticmethod(insertion_sort2)


class TestMergeSort2(SortingTests, unittest.TestCase):
    sort = staticmethod(merge_sort2)


if __name__ == "__main__":
    unittest.main(verbosity=2)
