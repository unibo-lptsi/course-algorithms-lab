/**
 * Module darray_test: tests of the darray module (interface).
 */
#ifndef DARRAY_TEST_H
#define DARRAY_TEST_H

/**
 * Runs all the tests with the resize policy in use (described by `policy`), printing a report;
 * returns the number of tests run, and writes the number of failed tests to *num_failed.
 * If verbose is non-zero, the report shows all the tests, each with its trace (the operations and the arrays);
 * otherwise, only the failed tests, with their failed checks, and the final summary.
 * NB: if a test crashes (e.g., a failed assert), run the tests verbose to see which one and where.
 */
int darray_run_tests(const char* policy, int verbose, int* num_failed);

#endif // DARRAY_TEST_H
