/**
 * Module test_harness: a minimal test harness (interface).
 *
 * A test is a function void f(void), which performs some actions and checks their results with the CHECK* macros
 * (or with checks built on check_result, like those of hashtable_test.c). A failed check is reported, and the test
 * goes on: so the report lists all the failed checks (and run_tests all the failed tests), not just the first one.
 * A test passes if it performs at least one check, and none fails.
 *
 * NB: unlike assert, which stops the program at the first violation, a check does not stop the program.
 */
#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

// Makes the compiler check the arguments of a printf-like function against its format (gcc and clang only)
#if defined(__GNUC__)
#define PRINTF_LIKE(format_index, first_arg_index) __attribute__((format(printf, format_index, first_arg_index)))
#else
#define PRINTF_LIKE(format_index, first_arg_index)
#endif

/**
 * Verbosity of the report: a combination, with the bitwise or (|), of the following flags.
 * With 0 (no flags), the report shows only the failed tests, each with its failed checks (with their actual and
 * expected values), and the summary. E.g., VERBOSE_TRACE (4) adds the traces of the failed tests, while
 * VERBOSE_PASSED_TESTS | VERBOSE_TRACE (5) shows all the tests, with their traces.
 */
enum {
    VERBOSE_PASSED_TESTS = 1, // (a) report the passed tests too, not only the failed ones
    VERBOSE_ALL_CHECKS = 2,   // (b) for each reported test, show all its checks, with their actual and expected values,
                              //     not only the failed ones
    VERBOSE_TRACE = 4,        // (c) for each reported test, show its trace: the actions it performs and the states
                              //     they lead to
    VERBOSE_ALL = VERBOSE_PASSED_TESTS | VERBOSE_ALL_CHECKS | VERBOSE_TRACE
};

// Returns the verbosity given on the command line (argv[1], a number from 0 to VERBOSE_ALL, e.g. ./hashtable 5),
// or default_verbosity if there is none; on an invalid argument, it prints how to use the program and exits
int verbosity_from_args(int argc, char* argv[], int default_verbosity);

typedef struct {
    void (*run)(void);
    const char* name;
    const char* description;
} TestCase;
// NB: #f turns the name of the test function into a string
#define TEST_CASE(f, description) { f, #f, description }

/**
 * Runs tests[0..num_tests-1], printing (on stdout) a report as stated by verbosity, which ends with a summary
 * for the suite (whose name is suite); returns the number of failed tests.
 * If a test crashes (e.g., with a segmentation fault), the report says which one before the program terminates.
 */
int run_tests(const char* suite, const TestCase tests[], int num_tests, int verbosity);

/**
 * Trace of the running test: a line (formatted as by printf, without the final \n), shown with VERBOSE_TRACE.
 * trace_on() says whether the trace is shown, so that a test can skip preparing lines that would not be shown.
 */
void trace(const char* format, ...) PRINTF_LIKE(1, 2);
int trace_on(void);

/**
 * Checks. check_result reports the outcome (ok) of a check of the running test, made at file:line: what describes
 * what is checked; actual and expected (if not NULL) the actual and the expected values. It returns ok, so that
 * a test can stop early, e.g.: if(!CHECK(h != NULL)) return;
 */
int check_result(int ok, const char* file, int line, const char* what, const char* actual, const char* expected);
int check_int(long actual, long expected, const char* file, int line, const char* what);
// NB: #cond turns the condition into a string; __FILE__ and __LINE__ are the file and the line of the check
#define CHECK(cond) check_result((cond) != 0, __FILE__, __LINE__, #cond, NULL, NULL)
#define CHECK_INT(actual, expected) check_int((actual), (expected), __FILE__, __LINE__, #actual)
// A test still to be written: it fails, with the message "TODO: " what
#define TEST_TODO(what) check_result(0, __FILE__, __LINE__, "TODO: " what, NULL, NULL)

/**
 * StrBuf: a string that grows as needed (a dynamic array of chars, see the lab dynamic-arrays), e.g. to prepare
 * the actual and expected values of a check:
 *     StrBuf sb = STRBUF_INIT;
 *     strbuf_printf(&sb, "found %d", 42);   // appends, formatting as printf
 *     ... strbuf_str(&sb) ...                // the string
 *     strbuf_free(&sb);
 */
typedef struct {
    char* chars; // chars[0..len] (with the final '\0'), or NULL if nothing has been appended yet
    int len;
    int capacity;
} StrBuf;
#define STRBUF_INIT { NULL, 0, 0 }
void strbuf_printf(StrBuf* sb, const char* format, ...) PRINTF_LIKE(2, 3);
const char* strbuf_str(const StrBuf* sb); // "" if nothing has been appended yet
void strbuf_clear(StrBuf* sb);            // empties the string, keeping the memory
void strbuf_free(StrBuf* sb);             // frees the memory: sb is empty again (STRBUF_INIT)

#endif // TEST_HARNESS_H
