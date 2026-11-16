/**
 * Module darray_test: tests of the darray module (implementation).
 *
 * A minimal test harness. A REQUIRE (e.g., in darray_set; see darray.c) checks a precondition: a violation is
 * a bug of the caller, so the program stops at once, with no final report. A test check instead reports a
 * failure and lets the tests go on, so that the final report lists all the failed tests, not just the first one.
 * The report can show all the tests, with their trace, or only the failed ones (see darray_run_tests).
 *
 * NB: the tests use the darray module only through its interface (darray.h), as any other user would:
 * DArray being an opaque type, the compiler rejects any access to its fields (e.g., da->size) outside darray.c.
 */
#include "darray_test.h"

#include <stdio.h>

#include "darray.h"

// Set by darray_run_tests
static int verbose_mode = 1;                 // 1: print all the tests, with their trace; 0: only the failed ones
static const char* current_test = "";        // name of the running test
static const char* current_description = ""; // description of the running test
static int failed_checks = 0;                // number of failed checks in the running test

static void print_test_header(void) {
    printf("[TEST] %s: %s\n", current_test, current_description);
}

// Counts a failed check, before reporting it; if not verbose, the header of the test is printed only now,
// at its first failed check (until then, the test could still pass)
static void begin_failure_report(void) {
    if(failed_checks++ == 0 && !verbose_mode) {
        print_test_header();
    }
    fflush(stdout); // print the pending output first, to keep the messages (on stdout and stderr) in order
}

// Reports a failed check (on stderr, as the other error messages); returns ok, so a test can stop early
static int check_impl(int ok, const char* expr, const char* file, int line) {
    if(!ok) {
        begin_failure_report();
        fprintf(stderr, "    [CHECK FAILED] %s (%s:%d): %s\n", current_test, file, line, expr);
    }
    return ok;
}
// NB: #cond turns the condition into a string (e.g., "darray_capacity(da) < N");
// __FILE__ and __LINE__ are the file and the line of the check
#define CHECK(cond) check_impl((cond) != 0, #cond, __FILE__, __LINE__)

/**
 * Checks that da holds exactly expected[0..expected_len-1] (expected may be NULL when expected_len is 0).
 * Returns 1 if so; otherwise, it reports the first difference as a failed check and returns 0.
 */
static int darray_assert_equals(const DArray* da, const TInfo* expected, int expected_len) {
    const int size = darray_size(da);
    const int capacity = darray_capacity(da);
    const int broken_invariant = size < 0 || size > capacity;
    int diff_pos = -1; // first position where the elements differ, if any
    if(!broken_invariant && size == expected_len) {
        for(int i = 0; i < expected_len && diff_pos < 0; i++) {
            if(darray_get(da, i) != expected[i]) {
                diff_pos = i;
            }
        }
        if(diff_pos < 0) {
            return 1;
        }
    }
    begin_failure_report();
    fprintf(stderr, "    [CHECK FAILED] %s, darray_assert_equals: ", current_test);
    if(broken_invariant) {
        // NB: do not print the elements, it could read outside the buffer
        fprintf(stderr, "size %d and capacity %d break the invariant 0 <= size <= capacity\n",
                size, capacity);
        return 0;
    }
    if(size != expected_len) {
        fprintf(stderr, "size is %d, expected %d\n", size, expected_len);
    } else {
        fprintf(stderr, "item[%d] is " TINFO_FMT ", expected " TINFO_FMT "\n",
                diff_pos, darray_get(da, diff_pos), expected[diff_pos]);
    }
    fprintf(stderr, "        actual:   ");
    darray_print(stderr, da, "\n");
    fprintf(stderr, "        expected: ");
    tinfo_print_array(stderr, expected, expected_len);
    fprintf(stderr, "\n");
    return 0;
}

// Creates a full array (capacity == size, so its next growth reallocates) holding items[0..n-1].
// NB: it uses provided functions only, so the setup of a test never depends on the exercise functions.
static DArray* make_darray(const TInfo* items, int n) {
    DArray* da = darray_create(n);
    for(int i = 0; i < n; i++) {
        darray_set(da, i, items[i]);
    }
    return da;
}

// Prints, as printf, a line of the trace of the running test (what it does), only if verbose.
// NB: __VA_ARGS__ stands for all the arguments of the macro: the format string and its values
#define TRACE_PRINTF(...) (verbose_mode ? printf(__VA_ARGS__) : 0)

// Prints the array, indented under the running test, only if verbose
static void trace(const DArray* da) {
    if(verbose_mode) {
        printf("  ");
        darray_print(stdout, da, "\n");
    }
}

static void test_create_and_set(void) {
    TRACE_PRINTF("  [ACT] creating an array of size 5 (the elements are zero-initialised)\n");
    DArray* da = darray_create(5);
    trace(da);
    darray_assert_equals(da, (TInfo[]){0, 0, 0, 0, 0}, 5);
    TRACE_PRINTF("  [ACT] setting d[i] to i+1, for i = 0, ..., 4\n");
    for(int i = 0; i < 5; i++) {
        darray_set(da, i, i + 1);
    }
    trace(da);
    darray_assert_equals(da, (TInfo[]){1, 2, 3, 4, 5}, 5);
    CHECK(darray_get(da, 2) == 3);
    darray_destroy(da);
}

static void test_realloc(void) {
    DArray* da = make_darray((TInfo[]){1, 2, 3, 4, 5}, 5);
    trace(da);
    TRACE_PRINTF("  [ACT] asking explicit realloc to capacity +3\n");
    const int old_capacity = darray_capacity(da);
    darray_realloc(da, old_capacity + 3);
    trace(da);
    CHECK(darray_capacity(da) == old_capacity + 3);
    darray_assert_equals(da, (TInfo[]){1, 2, 3, 4, 5}, 5);
    darray_destroy(da);
}

static void test_append(void) {
    DArray* da = make_darray((TInfo[]){1, 2, 3, 4, 5}, 5);
    trace(da);
    TRACE_PRINTF("  [ACT] appending 88\n");
    darray_append(da, 88);
    trace(da);
    darray_assert_equals(da, (TInfo[]){1, 2, 3, 4, 5, 88}, 6);
    darray_destroy(da);

    TRACE_PRINTF("  [ACT] appending 7 to an empty array\n");
    da = darray_create(0);
    darray_append(da, 7);
    trace(da);
    darray_assert_equals(da, (TInfo[]){7}, 1);
    darray_destroy(da);
}

static void test_resize(void) {
    DArray* da = make_darray((TInfo[]){1, 2, 3, 4, 5}, 5);
    trace(da);
    TRACE_PRINTF("  [ACT] resizing to 17 (the new elements are zero-initialised)\n");
    darray_resize(da, 17);
    trace(da);
    TInfo expected[17] = {1, 2, 3, 4, 5}; // NB: the elements without an initialiser are set to 0
    darray_assert_equals(da, expected, 17);
    TRACE_PRINTF("  [ACT] resizing back to 3\n");
    darray_resize(da, 3);
    trace(da);
    darray_assert_equals(da, (TInfo[]){1, 2, 3}, 3);
    darray_destroy(da);
}

static void test_resize_to_zero(void) {
    DArray* da = make_darray((TInfo[]){1, 2, 3}, 3);
    trace(da);
    TRACE_PRINTF("  [ACT] resizing to 0 (twice)\n");
    darray_resize(da, 0);
    trace(da);
    darray_resize(da, 0);
    trace(da);
    darray_assert_equals(da, NULL, 0);
    TRACE_PRINTF("  [ACT] resizing to 2 (the new elements are zero-initialised)\n");
    darray_resize(da, 2);
    trace(da);
    darray_assert_equals(da, (TInfo[]){0, 0}, 2);
    darray_destroy(da);
}

static void test_expand(void) {
    DArray* da = make_darray((TInfo[]){1, 2, 3}, 3);
    trace(da);
    TRACE_PRINTF("  [ACT] expanding with 11 new elements 0, ..., 10\n");
    darray_expand(da, (TInfo[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 11);
    trace(da);
    darray_assert_equals(da, (TInfo[]){1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 14);
    darray_destroy(da);
}

static void test_insert(void) {
    DArray* da = make_darray((TInfo[]){1, 2, 3}, 3);
    trace(da);
    TRACE_PRINTF("  [ACT] inserting 55 at position 2\n");
    darray_insert(da, 2, 55);
    trace(da);
    darray_assert_equals(da, (TInfo[]){1, 2, 55, 3}, 4);
    TRACE_PRINTF("  [ACT] inserting 77 at position 0 (i.e., at the front)\n");
    darray_insert(da, 0, 77);
    trace(da);
    darray_assert_equals(da, (TInfo[]){77, 1, 2, 55, 3}, 5);
    TRACE_PRINTF("  [ACT] inserting 99 at position size (i.e., at the end)\n");
    darray_insert(da, darray_size(da), 99);
    trace(da);
    darray_assert_equals(da, (TInfo[]){77, 1, 2, 55, 3, 99}, 6);
    darray_destroy(da);
}

static void test_remove(void) {
    DArray* da = make_darray((TInfo[]){77, 1, 2, 55, 3, 99}, 6);
    trace(da);
    TRACE_PRINTF("  [ACT] removing 77 (position 0), 55 (position 2), 99 (position size-1)\n");
    const TInfo first = darray_remove(da, 0);
    const TInfo middle = darray_remove(da, 2);
    const TInfo last = darray_remove(da, darray_size(da) - 1);
    TRACE_PRINTF("  removed: " TINFO_FMT ", " TINFO_FMT ", " TINFO_FMT "\n", first, middle, last);
    CHECK(first == 77);
    CHECK(middle == 55);
    CHECK(last == 99);
    trace(da);
    darray_assert_equals(da, (TInfo[]){1, 2, 3}, 3);
    darray_destroy(da);
}

static void test_append_remove_many(void) {
    enum { N = 30 };
    TRACE_PRINTF("  [ACT] appending 0, ..., %d to an empty array\n", N - 1);
    DArray* da = darray_create(0);
    TInfo expected[N];
    for(int i = 0; i < N; i++) {
        darray_append(da, i);
        expected[i] = i;
    }
    trace(da);
    // NB: stop at the first failure: going on could remove from an empty array
    if(darray_assert_equals(da, expected, N)) {
        TRACE_PRINTF("  [ACT] removing all the elements from the end\n");
        for(int i = N - 1; i >= 0; i--) {
            darray_remove(da, darray_size(da) - 1);
            if(i % 10 == 0) {
                trace(da);
            }
            if(!darray_assert_equals(da, expected, i)) {
                break;
            }
        }
        CHECK(darray_capacity(da) < N); // removing elements must release memory (through darray_resize)
    }
    darray_destroy(da);
}

static void test_init(void) {
    TRACE_PRINTF("  [ACT] creating an array of size 7 with all values set to 42\n");
    DArray* da = darray_init(7, 42);
    trace(da);
    darray_assert_equals(da, (TInfo[]){42, 42, 42, 42, 42, 42, 42}, 7);
    darray_destroy(da);
}

typedef struct {
    void (*run)(void);
    const char* name;
    const char* description;
} TestCase;
// NB: #f turns the name of the test function into a string
#define TEST_CASE(f, description) { f, #f, description }

static const TestCase TESTS[] = {
    TEST_CASE(test_create_and_set, "create an array, set and get its elements"),
    TEST_CASE(test_realloc, "an explicit realloc keeps the elements"),
    TEST_CASE(test_append, "append to a non-empty and to an empty array"),
    TEST_CASE(test_resize, "grow (zero-filling) and shrink, keeping the first elements"),
    TEST_CASE(test_resize_to_zero, "empty the array, then grow it again"),
    TEST_CASE(test_expand, "append all the elements of a C array"),
    TEST_CASE(test_insert, "insert in the middle, at the front and at the end"),
    TEST_CASE(test_remove, "remove from the front, the middle and the end"),
    TEST_CASE(test_append_remove_many, "many appends, then remove all: memory is released"),
    TEST_CASE(test_init, "create an array with all the elements set to a value"),
};
enum { NUM_TESTS = (int)(sizeof(TESTS) / sizeof(TESTS[0])) };

int darray_run_tests(const char* policy, int verbose, int* num_failed_out) {
    verbose_mode = verbose;
    printf("===== TESTS: %s =====\n\n", policy);
    const char* failed[NUM_TESTS];
    int num_failed = 0;
    for(int i = 0; i < NUM_TESTS; i++) {
        current_test = TESTS[i].name;
        current_description = TESTS[i].description;
        failed_checks = 0;
        if(verbose_mode) {
            print_test_header();
        }
        TESTS[i].run();
        if(failed_checks == 0) {
            if(verbose_mode) {
                printf("[PASS] %s\n\n", TESTS[i].name);
            }
        } else {
            printf("[FAIL] %s (%d failed check%s)\n\n", TESTS[i].name, failed_checks, failed_checks == 1 ? "" : "s");
            failed[num_failed++] = TESTS[i].name;
        }
    }
    printf("----- %s: %d/%d tests passed", policy, NUM_TESTS - num_failed, NUM_TESTS);
    for(int i = 0; i < num_failed; i++) {
        printf("%s%s", i == 0 ? "; failed: " : ", ", failed[i]);
    }
    printf(" -----\n\n");
    *num_failed_out = num_failed;
    return NUM_TESTS;
}
