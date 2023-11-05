import unittest
import test_numbers
import test_strings

def suite():
    suite = unittest.TestSuite()
    suite.addTest(test_strings.TestStringMethods("test_upper"))
    suite.addTest(test_numbers.TestNumbers("test_even"))
    return suite

test_cases = (test_strings.TestStringMethods, test_numbers.TestNumbers)

def load_tests(loader):
    suite = unittest.TestSuite()
    for test_class in test_cases:
        tests = loader.loadTestsFromTestCase(test_class)
        suite.addTests(tests)
    return suite

if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    # runner.run(suite())
    runner.run(load_tests(unittest.defaultTestLoader))