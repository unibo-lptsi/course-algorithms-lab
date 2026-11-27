/**
 * Module test_harness: a minimal test harness (implementation). The interface is in test_harness.h.
 *
 * The report of a test (its header, trace and checks) is printed as the test runs if the passed tests are reported
 * too (VERBOSE_PASSED_TESTS); otherwise, it is kept in a buffer (StrBuf, defined in the header), 
 * and printed at the end of the test only if the test fails (until then, the test could still pass).
 */
#include "test_harness.h"

#include <signal.h> // for custom handling of signals 
#include <stdarg.h> 
#include <stdio.h>
#include <stdlib.h>

/*
 * Structure of this harness:
 *     - StrBuf: a string buffer, used to keep the report of a test until its end
 *     - State of the harness, set by run_tests: verbosity, current_test, buffered, report_buffer, num_checks, num_failed_checks
 *     - Trace: trace_on and trace (it handles the trace of the running test, printing if VERBOSE_TRACE or keeping in the buffer)
 *     - Checks: check_result and check_int (they handle the checks of the running test, counting and reporting them)
 *     - Crashes: on_crash and handle_crashes (it handles the signals that terminate the program, printing the report of the running test so far)
 */

/* StrBuf */

// Appends to sb, formatting as vprintf (NB: a va_list holds the arguments of a variadic function, i.e. its "...")
static void strbuf_vprintf(StrBuf* sb, const char* format, va_list args) {
    // 1. compute the number of chars to append, and ensure that sb->chars can hold them (with a final '\0')
    va_list args_copy;
    va_copy(args_copy, args); // NB: a va_list can be used only once, and vsnprintf is called twice
    // vsnprintf(char* buffer, size_t size, const char* format, va_list args) formats the arguments in args according to format
    //  and writes the result to buffer, which can hold at most size chars (including the final '\0')
    const int n = vsnprintf(NULL, 0, format, args_copy); // the number of chars to append (without the final '\0')
    va_end(args_copy);
    
    if(n < 0) {
        return; // formatting error: nothing to append
    }

    // 2. if enough room is available, format the arguments in args and append them to sb->chars 
    if(sb->len + n + 1 > sb->capacity) {
        int new_capacity = sb->capacity > 0 ? sb->capacity : 64;
        while(new_capacity < sb->len + n + 1) {
            new_capacity *= 2; // geometric growth: appending costs O(1) amortized time per char
        }
        char* new_chars = realloc(sb->chars, (size_t)new_capacity);
        if(new_chars == NULL) {
            fprintf(stderr, "ERROR: realloc failed; exiting.\n");
            exit(EXIT_FAILURE);
        }
        sb->chars = new_chars;
        sb->capacity = new_capacity;
    }

    // 3. format the arguments in args and append them to sb->chars, increasing sb->len
    vsnprintf(sb->chars + sb->len, (size_t)(sb->capacity - sb->len), format, args);
    sb->len += n;
}

void strbuf_printf(StrBuf* sb, const char* format, ...) {
    // classic handling for variadic functions: va_list, va_start, va_end (see stdarg.h)
    va_list args;
    va_start(args, format);
    // to use args, we need to pass it to vprintf functions, which take a va_list instead of "..."
    strbuf_vprintf(sb, format, args);
    va_end(args);
}

const char* strbuf_str(const StrBuf* sb) {
    return sb->chars != NULL ? sb->chars : "";
}

void strbuf_clear(StrBuf* sb) {
    sb->len = 0;
    if(sb->chars != NULL) {
        sb->chars[0] = '\0';
    }
}

void strbuf_free(StrBuf* sb) {
    free(sb->chars);
    *sb = (StrBuf)STRBUF_INIT;
}

/* State of the harness, set by run_tests */

static int verbosity = 0;
static const TestCase* current_test = NULL; // the running test (NULL between tests)
static int buffered = 0;                    // 1: the report of the running test goes to report_buffer; 0: printed as test runs
static StrBuf report_buffer = STRBUF_INIT;
static int num_checks = 0;                  // number of checks performed by the running test
static int num_failed_checks = 0;           // number of failed checks of the running test

// Adds to the report of the running test, formatting as vprintf: printed at once, or kept in the buffer
static void report_vprintf(const char* format, va_list args) {
    // If the test is buffered, append to the report buffer; otherwise, print directly
    if(buffered) {
        strbuf_vprintf(&report_buffer, format, args);
    } else {
        vprintf(format, args);
    }
}

static void report_printf(const char* format, ...) PRINTF_LIKE(1, 2);
static void report_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    report_vprintf(format, args);
    va_end(args);
}

/* Trace */

int trace_on(void) {
    return current_test != NULL && (verbosity & VERBOSE_TRACE);
}

void trace(const char* format, ...) {
    if(!trace_on()) {
        return;
    }
    report_printf("  ");
    va_list args;
    va_start(args, format);
    report_vprintf(format, args);
    va_end(args);
    report_printf("\n");
}

/* Checks */

int check_result(int ok, const char* file, int line, const char* what, const char* actual, const char* expected) {
    num_checks++;
    if(!ok) {
        num_failed_checks++;
    }
    // a failed check is always shown (its test is reported), a passed check only with VERBOSE_ALL_CHECKS
    if(!ok || (verbosity & VERBOSE_ALL_CHECKS)) {
        report_printf("  [%s] %s (%s:%d)\n", ok ? " OK " : "FAIL", what, file, line);
        if(actual != NULL) {
            report_printf("         actual:   %s\n", actual);
        }
        if(expected != NULL) {
            report_printf("         expected: %s\n", expected);
        }
    }
    return ok;
}

int check_int(long actual, long expected, const char* file, int line, const char* what) {
    char actual_str[32], expected_str[32]; // NB: large enough for any long (at most 20 chars)
    // int snprintf( char* restrict buffer, size_t bufsz, const char* restrict format, ... );
    snprintf(actual_str, sizeof(actual_str), "%ld", actual);
    snprintf(expected_str, sizeof(expected_str), "%ld", expected);
    return check_result(actual == expected, file, line, what, actual_str, expected_str);
}

/* Crashes */

// Called when the program receives the signal sig (e.g., SIGSEGV for an invalid memory access) during a test:
// it prints the report of the test so far (if buffered) and which test crashed, then lets the program terminate.
// NB: strictly, the C standard allows a signal handler to call printf & co. only for the signals raised by abort
// or raise (e.g., SIGABRT, after a failed assert): after an invalid memory access, this is a best effort.
static void on_crash(int sig) {
    signal(sig, SIG_DFL); // the default action again, for the next time sig is received 
    if(current_test != NULL) {
        if(buffered) {
            // the report of the running test is kept in the buffer, so print it now
            fputs(strbuf_str(&report_buffer), stdout);
        }
        printf("[CRASH] %s: %s\n", current_test->name,
               sig == SIGSEGV ? "segmentation fault (SIGSEGV), i.e. an invalid memory access" :
               sig == SIGFPE ? "arithmetic error (SIGFPE), e.g. an integer division by zero" :
               sig == SIGABRT ? "abort (SIGABRT), e.g. after a failed assert" : "illegal instruction (SIGILL)");
    }
    fflush(stdout); // the pending output would be lost otherwise (e.g., when stdout is redirected to a file)
    // Returning, abort terminates the program; after an invalid memory access (or a division by zero), the
    // instruction that caused it runs again, now with the default action, which terminates the program.
}

// NB: AddressSanitizer (gcc/clang -fsanitize=address) handles SIGSEGV and SIGFPE itself, with a detailed report
#if defined(__SANITIZE_ADDRESS__)
#define ASAN_ENABLED 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define ASAN_ENABLED 1
#endif
#endif

static void handle_crashes(void) {
    // void (*signal( int sig, void (*handler) (int))) (int);
    // Changes handling of the signal sig
    signal(SIGABRT, on_crash);
#ifndef ASAN_ENABLED
    signal(SIGSEGV, on_crash);
    signal(SIGFPE, on_crash);
    signal(SIGILL, on_crash);
#endif
}

/* Running the tests */

int verbosity_from_args(int argc, char* argv[], int default_verbosity) {
    if(argc < 2) {
        return default_verbosity;
    }
    char* end;
    const long v = strtol(argv[1], &end, 10);
    if(argc > 2 || end == argv[1] || *end != '\0' || v < 0 || v > VERBOSE_ALL) {
        fprintf(stderr,
                "Usage: %s [VERBOSITY]\n"
                "VERBOSITY is the sum of the flags to enable (default: %d; 0 = only the failed tests, with their failed checks):\n"
                "  %d  report the passed tests too\n"
                "  %d  show all the checks of the reported tests (not only the failed ones), with actual and expected values\n"
                "  %d  show the trace of the reported tests (actions and states)\n"
                "e.g., %d = the traces of the failed tests; %d = all the tests, with their traces; %d = everything\n",
                argv[0], default_verbosity, VERBOSE_PASSED_TESTS, VERBOSE_ALL_CHECKS, VERBOSE_TRACE,
                VERBOSE_TRACE, VERBOSE_PASSED_TESTS | VERBOSE_TRACE, VERBOSE_ALL);
        exit(2); // the usual exit status for a wrong usage
    }
    return (int)v;
}

int run_tests(const char* suite, const TestCase tests[], int num_tests, int verbosity_flags) {
    verbosity = verbosity_flags;
    handle_crashes(); // registers the signal handlers for the signals that terminate the program (SIGABRT, SIGSEGV, SIGFPE, SIGILL)
    printf("===== TESTS: %s =====\n\n", suite);
    StrBuf failed_tests = STRBUF_INIT; // names of the failed tests, for the summary
    int num_failed = 0;
    for(int i = 0; i < num_tests; i++) {
        current_test = &tests[i]; // a test has a name, a description and a function that runs it
        num_checks = 0;
        num_failed_checks = 0;
        // if only the failed tests are reported, the report of a test can be printed only at its end
        buffered = !(verbosity & VERBOSE_PASSED_TESTS);
        strbuf_clear(&report_buffer);
        report_printf("[TEST] %s: %s\n", current_test->name, current_test->description);
        current_test->run();
        if(num_checks > 0 && num_failed_checks == 0) {
            if(verbosity & VERBOSE_PASSED_TESTS) {
                printf("[PASS] %s (%d check%s)\n\n", current_test->name, num_checks, num_checks == 1 ? "" : "s");
            }
        } else {
            if(buffered) {
                fputs(strbuf_str(&report_buffer), stdout);
            }
            if(num_checks == 0) {
                printf("[FAIL] %s (no checks: a test must check something)\n\n", current_test->name);
            } else {
                printf("[FAIL] %s (%d of %d checks failed)\n\n", current_test->name, num_failed_checks, num_checks);
            }
            strbuf_printf(&failed_tests, "%s%s", num_failed == 0 ? "; failed: " : ", ", current_test->name);
            num_failed++;
        }
        current_test = NULL;
    }
    printf("----- %s: %d/%d tests passed%s -----\n\n", suite, num_tests - num_failed, num_tests,
           strbuf_str(&failed_tests));
    fflush(stdout);
    strbuf_free(&failed_tests);
    strbuf_free(&report_buffer);
    return num_failed;
}
