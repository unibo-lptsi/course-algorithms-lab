import unittest

class TestStringMethods(unittest.TestCase):
    def __init__(self, methodName):
        super().__init__(methodName)
        print("__init()__ of " + str(self))

    def setUp(self):
        print("setup")
        self.foo_lc = 'foo'
        
    def tearDown(self):
        print("tear down")
        pass

    def test_upper(self):
        self.assertEqual(self.foo_lc.upper(), 'FOO')

    def test_isupper(self):
        self.assertFalse(self.foo_lc.isupper())
        self.assertTrue('FOO'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

    @unittest.expectedFailure
    def test_fail(self):
        self.assertEqual(1, 0, "broken")

    @unittest.skip("demonstrating skipping")
    def test_nothing(self):
        self.fail("shouldn't happen")

if __name__ == '__main__':
    unittest.main()