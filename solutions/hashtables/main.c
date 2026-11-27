/**
 * Main program: runs the tests of the hashtable module.
 *
 * Build (see README.md): gcc -Wall -o hashtable *.c
 * Run:                   ./hashtable [VERBOSITY]   (e.g., ./hashtable 5; ./hashtable help explains VERBOSITY)
 */
#include <stdlib.h>

#include "hashtable_test.h"
#include "test_harness.h"

int main(int argc, char* argv[]) {
    // Verbosity of the report (see test_harness.h): the one given on the command line, if any, or the default one.
    // With 0, only the failed tests are reported; e.g., with VERBOSE_PASSED_TESTS | VERBOSE_TRACE, all the tests are
    // reported, each with its trace.
    const int default_verbosity = 0;
    const int verbosity = verbosity_from_args(argc, argv, default_verbosity);

    int num_failed; // failed tests, written by hashtable_run_tests
    hashtable_run_tests("chained hash table, integer keys, h(k) = k mod m", verbosity, &num_failed);
    return num_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
