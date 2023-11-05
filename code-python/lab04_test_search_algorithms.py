import unittest
from unittest import TestCase
from lab04_search_binary_search import binary_search
from lab04_search_linear_search import linear_search

TAG_LINEAR_SEARCH = "linear search"
TAG_BINARY_SEARCH = "binary search"

class TestSearchAlgorithms(TestCase):
    not_found_pos = -1

    def test_empty(self):
        input = []
        arbitrary_value = 55
        self.subTest(search=TAG_LINEAR_SEARCH)
        self.assertEqual(self.not_found_pos, linear_search(input, arbitrary_value))
        self.subTest(search=TAG_BINARY_SEARCH)
        self.assertEqual(self.not_found_pos, binary_search(input, arbitrary_value))

    def test_singleton(self):
        single_value = 77
        input = [single_value]
        self.subTest(search=TAG_LINEAR_SEARCH)
        self.assertEqual(0, linear_search(input, single_value))
        self.subTest(search=TAG_BINARY_SEARCH)
        self.assertEqual(0, binary_search(input, single_value))

    def test_simple_unordered(self):
        input = [3, 7, -1, 55, 12, 0]
        search_value = 55
        expected_pos = 3
        self.assertEqual(expected_pos, linear_search(input, search_value), f"{input}")

    def test_simple_ordered(self):
        input = [-1, 0, 3, 7, 12, 55]
        search_value = 12
        expected_pos = 4
        self.subTest(search=TAG_LINEAR_SEARCH)
        self.assertEqual(expected_pos, linear_search(input, search_value), f"{input}")
        self.subTest(search=TAG_BINARY_SEARCH)
        self.assertEqual(expected_pos, binary_search(input, search_value), f"{input}")

if __name__ == '__main__': 
    unittest.main(verbosity=2)