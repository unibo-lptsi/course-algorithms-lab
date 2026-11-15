#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * TODO: implementare le seguenti funzioni:
 * - void darray_append(DArray* da, TInfo value)
 * - void darray_insert(DArray* da, int insert_pos, TInfo value)
 * - TInfo darray_remove(DArray* da, int remove_pos)
 * - DArray darray_init(int initial_size, TInfo value)
 */

// Prints "<function name>: TO BE IMPLEMENTED"
// fflush(stdout) prints the pending output first, to keep the messages in order
#define NOT_IMPLEMENTED() do { \
        fflush(stdout); \
        fprintf(stderr, "    %s: TO BE IMPLEMENTED\n", __func__); \
    } while(0)

// NB: compile with gcc "-DDEBUG" to print log messages (on stderr)
#ifdef DEBUG
#define LOG_ENABLED 1
#else
#define LOG_ENABLED 0
#endif
// NB: "if(0)" is optimised away, but the compiler still checks the arguments;
// "do { ... } while(0)" makes the macro a single statement
#define LOG(...) do { if(LOG_ENABLED) fprintf(stderr, __VA_ARGS__); } while(0)

typedef int TInfo;
#define TINFO_FMT "%d" // printf format of TInfo: change it together with the typedef

/**
 * Invariant: 0 <= size <= capacity; item[0..size-1] are the elements, item[size..capacity-1] is free space.
 * NB: always pass a DArray by pointer. A copy (DArray b = a;) shares the buffer of a, which any resize of a
 * may move or free (realloc): pointers into item[] are invalidated by resizes.
 * NB: sizes are int rather than size_t (the idiomatic type for sizes) to keep the code simple:
 * e.g., with an unsigned i, a loop like for(i=size-1; i>=0; i--) would never end.
 */
struct SDArray {
    TInfo* item;
    int capacity;
    int size;
};
typedef struct SDArray DArray;

#define MIN_CAPACITY 1 // never allocate 0 bytes: malloc(0) and realloc(p, 0) may return NULL

/**
 * Parameters of the resize policies. Change them only through darray_set_linear_policy() and
 * darray_set_geometric_policy(), which check the following constraints (why are they needed?):
 * - linear:    1 <= GROWING_DELTA < SHRINKING_DELTA
 * - geometric: 1 < GROWING_FACTOR < SHRINKING_FACTOR
 * NB: the policy is global: it applies to all the arrays, including the existing ones.
 */
static int GROWING_DELTA = 10;
static int SHRINKING_DELTA = 20;
static double GROWING_FACTOR = 2.5;
static double SHRINKING_FACTOR = 3.0;

DArray darray_create(int initial_size);
DArray darray_create_capac(int initial_size, int initial_capacity);
DArray darray_init(int initial_size, TInfo value);
TInfo darray_get(const DArray* da, int pos);
void darray_set(DArray* da, int pos, TInfo value);
void darray_print(const DArray* da, const char* eol);
void darray_destroy(DArray* da);
void darray_realloc(DArray* da, int new_capacity);
void darray_resize_linear(DArray* da, int new_size);
void darray_resize_geometric(DArray* da, int new_size);
void darray_resize(DArray* da, int new_size);
void darray_set_linear_policy(int growing_delta, int shrinking_delta);
void darray_set_geometric_policy(double growing_factor, double shrinking_factor);
void darray_append(DArray* da, TInfo value);
void darray_expand(DArray* da, const TInfo* arr, int sz);
void darray_insert(DArray* da, int insert_pos, TInfo value);
TInfo darray_remove(DArray* da, int remove_pos);
int darray_assert_equals(const DArray* da, const TInfo* expected, int expected_len);

// The resize policy in use (see darray_resize)
static void (*f_resize)(DArray*, int) = &darray_resize_linear;


void darray_append(DArray* da, TInfo value) {
    // TODO
    NOT_IMPLEMENTED();
}

void darray_insert(DArray* da, int insert_pos, TInfo value) {
    // TODO (precondition: 0 <= insert_pos <= size)
    // step 1: resize to give room for the additional element (size+1)
    // step 2: shift the elements from the insert position to the right
    // step 3: set item at the insert position
    NOT_IMPLEMENTED();
}

TInfo darray_remove(DArray* da, int remove_pos) {
    // TODO (precondition: 0 <= remove_pos < size; returns the removed element)
    NOT_IMPLEMENTED();
    return (TInfo){0}; // dummy value
}

DArray darray_init(int initial_size, TInfo value) {
    // TODO
    DArray da = {0};
    NOT_IMPLEMENTED();
    return da;
}

DArray darray_create(int initial_size) {
    return darray_create_capac(initial_size, initial_size);
}

DArray darray_create_capac(int initial_size, int initial_capacity) {
    assert(initial_size >= 0);
    if(initial_capacity < initial_size) {
        fprintf(stderr, "WARNING: initial_capacity %d is less than initial_size %d; adjusting initial_capacity to initial_size.\n", initial_capacity, initial_size);
        initial_capacity = initial_size;
    }
    if(initial_capacity < MIN_CAPACITY) {
        initial_capacity = MIN_CAPACITY;
    }

    DArray a;
    // NB: calloc zero-initialises the elements (with malloc, their value would be indeterminate)
    a.item = calloc((size_t)initial_capacity, sizeof(TInfo));
    if(a.item == NULL) {
        fprintf(stderr, "ERROR: calloc failed; exiting.\n");
        exit(EXIT_FAILURE);
    }
    a.capacity = initial_capacity;
    a.size = initial_size;
    return a;
}

TInfo darray_get(const DArray* da, int pos) {
    assert(0 <= pos && pos < da->size);
    return da->item[pos];
}

void darray_set(DArray* da, int pos, TInfo value) {
    // Precondition: 0 <= pos < size. Violating it is a bug of the caller, hence the assert:
    // just printing a warning and going on would make the bug easy to miss.
    // Alternatives: returning an error code; or expanding the size (but then the user should be aware
    // that a O(n) cost may apply, and the name should say it, e.g. darray_set_or_expand):
    /**
    if(da->size <= pos) {
        darray_resize(da, pos+1);
    }
    */
    assert(0 <= pos && pos < da->size);
    da->item[pos] = value;
}

// Prints items[0..n-1] as {a_0, ..., a_{n-1}} on the given stream (e.g., stdout or stderr)
static void print_items(FILE* out, const TInfo* items, int n) {
    fprintf(out, "{");
    for(int i = 0; i < n; i++) {
        if(i > 0) fprintf(out, ", ");
        fprintf(out, TINFO_FMT, items[i]);
    }
    fprintf(out, "}");
}

void darray_print(const DArray* da, const char* eol) {
    printf("[%d/%d]", da->size, da->capacity);
    print_items(stdout, da->item, da->size);
    printf("%s", eol);
}

void darray_destroy(DArray* da) {
    free(da->item);
    da->item = NULL;
    da->capacity = 0;
    da->size = 0;
}

void darray_realloc(DArray* da, int new_capacity) {
    assert(new_capacity >= da->size); // keep the invariant size <= capacity
    if(new_capacity < MIN_CAPACITY) {
        new_capacity = MIN_CAPACITY;
    }
    LOG("[LOG] Requested reallocating and setting new_capacity=%d.\n", new_capacity);
    // NB: not "da->item = realloc(da->item, ...)": when realloc fails, it returns NULL but the old block
    // is still allocated, and we would lose the only pointer to it
    TInfo* new_item = realloc(da->item, sizeof(TInfo) * (size_t)new_capacity);
    if(new_item == NULL) {
        fprintf(stderr, "ERROR: realloc failed; exiting.\n");
        darray_destroy(da);
        exit(EXIT_FAILURE);
    }
    da->item = new_item;
    da->capacity = new_capacity;
}

void darray_resize_linear(DArray* da, int new_size) {
    assert(new_size >= 0);
    LOG("[LINEAR] Resize darray to new_size=%d.\n", new_size);
    da->size = new_size; // NB: before darray_realloc, which checks new_capacity >= size
    if(new_size > da->capacity || (da->capacity - new_size) > SHRINKING_DELTA) {
        const int new_capacity = new_size + GROWING_DELTA;
        LOG("[LINEAR] Setting capacity to %d.\n", new_capacity);
        darray_realloc(da, new_capacity);
    }
}

void darray_resize_geometric(DArray* da, int new_size) {
    assert(new_size >= 0);
    LOG("[GEOMETRIC] Resize darray to new_size=%d.\n", new_size);
    da->size = new_size; // NB: before darray_realloc, which checks new_capacity >= size
    // NB: "capacity > SHRINKING_FACTOR * new_size" rather than "capacity / new_size > SHRINKING_FACTOR":
    // no integer division (which truncates), and no division by zero when new_size == 0
    if(new_size > da->capacity || da->capacity > SHRINKING_FACTOR * new_size) {
        int new_capacity = (int)(new_size * GROWING_FACTOR);
        if(new_capacity <= new_size) {
            new_capacity = new_size + 1; // the truncation may leave no free slot (small sizes, factors near 1)
        }
        if(new_capacity != da->capacity) {
            LOG("[GEOMETRIC] Setting capacity to %d.\n", new_capacity);
            darray_realloc(da, new_capacity);
        }
    }
}

void darray_resize(DArray* da, int new_size) {
    const int old_size = da->size;
    f_resize(da, new_size);
    // the new elements (if any) are zero-initialised, so that item[0..size-1] never holds garbage
    for(int i = old_size; i < new_size; i++) {
        da->item[i] = (TInfo){0};
    }
}

void darray_set_linear_policy(int growing_delta, int shrinking_delta) {
    assert(1 <= growing_delta && growing_delta < shrinking_delta);
    GROWING_DELTA = growing_delta;
    SHRINKING_DELTA = shrinking_delta;
    f_resize = &darray_resize_linear;
}

void darray_set_geometric_policy(double growing_factor, double shrinking_factor) {
    assert(1 < growing_factor && growing_factor < shrinking_factor);
    GROWING_FACTOR = growing_factor;
    SHRINKING_FACTOR = shrinking_factor;
    f_resize = &darray_resize_geometric;
}

// Precondition: arr does not point into da->item, which the resize may free (e.g., not darray_expand(da, da->item, ...))
void darray_expand(DArray* da, const TInfo* arr, int sz) {
    assert(sz >= 0);
    int curr_size = da->size; // NB: keep track of current size as resize will move it
    darray_resize(da, curr_size + sz);
    memcpy(&da->item[curr_size], arr, sizeof(TInfo) * (size_t)sz);
}

/******************************************************************************
 * TESTS
 *
 * A minimal test harness. An assert (e.g., in darray_set) checks a precondition: a violation is a bug of
 * the caller, so the program stops at once. A test check instead reports a failure and lets the tests go
 * on, so that the final report lists all the failed tests, not just the first one.
 ******************************************************************************/

static const char* current_test = ""; // name of the running test (set by run_tests)
static int failed_checks = 0;         // number of failed checks in the running test

// Reports a failed check (on stderr, as the other error messages); returns ok, so a test can stop early
static int check_impl(int ok, const char* expr, int line) {
    if(!ok) {
        failed_checks++;
        fflush(stdout); // print the pending output first, to keep the messages in order
        fprintf(stderr, "    [CHECK FAILED] %s, line %d: %s\n", current_test, line, expr);
    }
    return ok;
}
// NB: #cond turns the condition into a string (e.g., "da.capacity < N"); __LINE__ is the line of the check
#define CHECK(cond) check_impl((cond) != 0, #cond, __LINE__)

/**
 * Checks that da holds exactly expected[0..expected_len-1] (expected may be NULL when expected_len is 0).
 * Returns 1 if so; otherwise, it reports the first difference as a failed check and returns 0.
 */
int darray_assert_equals(const DArray* da, const TInfo* expected, int expected_len) {
    const int broken_invariant = da->size < 0 || da->size > da->capacity;
    int diff_pos = -1; // first position where the elements differ, if any
    if(!broken_invariant && da->size == expected_len) {
        for(int i = 0; i < expected_len && diff_pos < 0; i++) {
            if(da->item[i] != expected[i]) {
                diff_pos = i;
            }
        }
        if(diff_pos < 0) {
            return 1;
        }
    }
    failed_checks++;
    fflush(stdout);
    fprintf(stderr, "    [CHECK FAILED] %s, darray_assert_equals: ", current_test);
    if(broken_invariant) {
        // NB: do not print the elements, it could read outside the buffer
        fprintf(stderr, "size %d and capacity %d break the invariant 0 <= size <= capacity\n",
                da->size, da->capacity);
        return 0;
    }
    if(da->size != expected_len) {
        fprintf(stderr, "size is %d, expected %d\n", da->size, expected_len);
    } else {
        fprintf(stderr, "item[%d] is " TINFO_FMT ", expected " TINFO_FMT "\n",
                diff_pos, da->item[diff_pos], expected[diff_pos]);
    }
    fprintf(stderr, "        actual:   [%d/%d]", da->size, da->capacity);
    print_items(stderr, da->item, da->size);
    fprintf(stderr, "\n        expected: ");
    print_items(stderr, expected, expected_len);
    fprintf(stderr, "\n");
    return 0;
}

// Creates a full array (capacity == size, so its next growth reallocates) holding items[0..n-1].
// NB: it uses provided functions only, so the setup of a test never depends on the exercise functions.
static DArray make_darray(const TInfo* items, int n) {
    DArray da = darray_create(n);
    for(int i = 0; i < n; i++) {
        darray_set(&da, i, items[i]);
    }
    return da;
}

// Prints the array, indented under the running test
static void trace(const DArray* da) {
    printf("  ");
    darray_print(da, "\n");
}

static void test_create_and_set(void) {
    printf("  [ACT] creating an array of size 5 (the elements are zero-initialised)\n");
    DArray da = darray_create(5);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){0, 0, 0, 0, 0}, 5);
    printf("  [ACT] setting d[i] to i+1, for i = 0, ..., 4\n");
    for(int i = 0; i < 5; i++) {
        darray_set(&da, i, i + 1);
    }
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){1, 2, 3, 4, 5}, 5);
    CHECK(darray_get(&da, 2) == 3);
    darray_destroy(&da);
}

static void test_realloc(void) {
    DArray da = make_darray((TInfo[]){1, 2, 3, 4, 5}, 5);
    trace(&da);
    printf("  [ACT] asking explicit realloc to capacity +3\n");
    const int old_capacity = da.capacity;
    darray_realloc(&da, old_capacity + 3);
    trace(&da);
    CHECK(da.capacity == old_capacity + 3);
    darray_assert_equals(&da, (TInfo[]){1, 2, 3, 4, 5}, 5);
    darray_destroy(&da);
}

static void test_append(void) {
    DArray da = make_darray((TInfo[]){1, 2, 3, 4, 5}, 5);
    trace(&da);
    printf("  [ACT] appending 88\n");
    darray_append(&da, 88);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){1, 2, 3, 4, 5, 88}, 6);
    darray_destroy(&da);

    printf("  [ACT] appending 7 to an empty array\n");
    da = darray_create(0);
    darray_append(&da, 7);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){7}, 1);
    darray_destroy(&da);
}

static void test_resize(void) {
    DArray da = make_darray((TInfo[]){1, 2, 3, 4, 5}, 5);
    trace(&da);
    printf("  [ACT] resizing to 17 (the new elements are zero-initialised)\n");
    darray_resize(&da, 17);
    trace(&da);
    TInfo expected[17] = {1, 2, 3, 4, 5}; // NB: the elements without an initialiser are set to 0
    darray_assert_equals(&da, expected, 17);
    printf("  [ACT] resizing back to 3\n");
    darray_resize(&da, 3);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){1, 2, 3}, 3);
    darray_destroy(&da);
}

static void test_resize_to_zero(void) {
    DArray da = make_darray((TInfo[]){1, 2, 3}, 3);
    trace(&da);
    printf("  [ACT] resizing to 0 (twice)\n");
    darray_resize(&da, 0);
    trace(&da);
    darray_resize(&da, 0);
    trace(&da);
    darray_assert_equals(&da, NULL, 0);
    printf("  [ACT] resizing to 2 (the new elements are zero-initialised)\n");
    darray_resize(&da, 2);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){0, 0}, 2);
    darray_destroy(&da);
}

static void test_expand(void) {
    DArray da = make_darray((TInfo[]){1, 2, 3}, 3);
    trace(&da);
    printf("  [ACT] expanding with 11 new elements 0, ..., 10\n");
    darray_expand(&da, (TInfo[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 11);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 14);
    darray_destroy(&da);
}

static void test_insert(void) {
    DArray da = make_darray((TInfo[]){1, 2, 3}, 3);
    trace(&da);
    printf("  [ACT] inserting 55 at position 2\n");
    darray_insert(&da, 2, 55);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){1, 2, 55, 3}, 4);
    printf("  [ACT] inserting 77 at position 0 (i.e., at the front)\n");
    darray_insert(&da, 0, 77);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){77, 1, 2, 55, 3}, 5);
    printf("  [ACT] inserting 99 at position size (i.e., at the end)\n");
    darray_insert(&da, da.size, 99);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){77, 1, 2, 55, 3, 99}, 6);
    darray_destroy(&da);
}

static void test_remove(void) {
    DArray da = make_darray((TInfo[]){77, 1, 2, 55, 3, 99}, 6);
    trace(&da);
    printf("  [ACT] removing 77 (position 0), 55 (position 2), 99 (position size-1)\n");
    const TInfo first = darray_remove(&da, 0);
    const TInfo middle = darray_remove(&da, 2);
    const TInfo last = darray_remove(&da, da.size - 1);
    printf("  removed: " TINFO_FMT ", " TINFO_FMT ", " TINFO_FMT "\n", first, middle, last);
    CHECK(first == 77);
    CHECK(middle == 55);
    CHECK(last == 99);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){1, 2, 3}, 3);
    darray_destroy(&da);
}

static void test_append_remove_many(void) {
    enum { N = 30 };
    printf("  [ACT] appending 0, ..., %d to an empty array\n", N - 1);
    DArray da = darray_create(0);
    TInfo expected[N];
    for(int i = 0; i < N; i++) {
        darray_append(&da, i);
        expected[i] = i;
    }
    trace(&da);
    // NB: stop at the first failure: going on could remove from an empty array
    if(darray_assert_equals(&da, expected, N)) {
        printf("  [ACT] removing all the elements from the end\n");
        for(int i = N - 1; i >= 0; i--) {
            darray_remove(&da, da.size - 1);
            if(i % 10 == 0) {
                trace(&da);
            }
            if(!darray_assert_equals(&da, expected, i)) {
                break;
            }
        }
        CHECK(da.capacity < N); // removing elements must release memory (through darray_resize)
    }
    darray_destroy(&da);
}

static void test_init(void) {
    printf("  [ACT] creating an array of size 7 with all values set to 42\n");
    DArray da = darray_init(7, 42);
    trace(&da);
    darray_assert_equals(&da, (TInfo[]){42, 42, 42, 42, 42, 42, 42}, 7);
    darray_destroy(&da);
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

// Runs all the tests with the resize policy in use, printing a report; returns the number of failed tests
static int run_tests(const char* policy) {
    printf("===== TESTS: %s =====\n\n", policy);
    const char* failed[NUM_TESTS];
    int num_failed = 0;
    for(int i = 0; i < NUM_TESTS; i++) {
        current_test = TESTS[i].name;
        failed_checks = 0;
        printf("[TEST] %s: %s\n", TESTS[i].name, TESTS[i].description);
        TESTS[i].run();
        if(failed_checks == 0) {
            printf("[PASS] %s\n\n", TESTS[i].name);
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
    return num_failed;
}

int main(void) {
    int num_failed = 0;
    darray_set_linear_policy(10, 12);
    num_failed += run_tests("linear policy (growing delta 10, shrinking delta 12)");
    darray_set_geometric_policy(4.5, 8.5);
    num_failed += run_tests("geometric policy (growing factor 4.5, shrinking factor 8.5)");
    printf("===== OVERALL: %d/%d tests passed =====\n", 2 * NUM_TESTS - num_failed, 2 * NUM_TESTS);
    return num_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
