/**
 * Module hashtable_test: tests of the hashtable module (interface).
 */
#ifndef HASHTABLE_TEST_H
#define HASHTABLE_TEST_H

/**
 * Runs all the tests (with the hash function in use), printing a report as stated by verbosity (see test_harness.h);
 * suite names the run in the report. Returns the number of tests run, and writes the number of failed tests
 * to *num_failed.
 */
int hashtable_run_tests(const char* suite, int verbosity, int* num_failed);

#endif // HASHTABLE_TEST_H
