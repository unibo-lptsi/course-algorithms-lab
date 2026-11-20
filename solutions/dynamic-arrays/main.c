/**
 * Main program: runs the tests of the darray module with both resize policies.
 *
 * Build (see README.md): gcc -Wall -o darray main.c darray.c darray_test.c
 */
#include <stdio.h>
#include <stdlib.h>

#include "darray.h"
#include "darray_test.h"

int main(void) {
    const int verbose = 0; // 1: report all the tests, with their trace; 0: only the failed ones
    int num_tests = 0, num_failed = 0;
    int failed; // failed tests of a run, written by darray_run_tests

    darray_set_linear_policy(10, 12);
    num_tests += darray_run_tests("linear policy (growing delta 10, shrinking delta 12)", verbose, &failed);
    num_failed += failed;

    darray_set_geometric_policy(4.5, 8.5);
    num_tests += darray_run_tests("geometric policy (growing factor 4.5, shrinking factor 8.5)", verbose, &failed);
    num_failed += failed;

    printf("===== OVERALL: %d/%d tests passed =====\n", num_tests - num_failed, num_tests);
    return num_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
