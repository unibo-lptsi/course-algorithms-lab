import unittest
import random
from min_max import min_max

class TestMinMax(unittest.TestCase):
    def test_singleton(self):
        actual_min, actual_max = min_max([77])
        self.assertEqual(77, actual_min)
        self.assertEqual(77, actual_max)
    
    def test_random(self):
        random.seed(42) # make it deterministic
        random_list = [random.randint(0, 100) for _ in range(10)]
        result = min_max(random_list)
        self.assertEqual((min(random_list), max(random_list)), result)

    def test_empty(self):
        with self.assertRaises(ValueError):
            min_max([])

# main
if __name__ == "__main__":
    unittest.main(verbosity=2)